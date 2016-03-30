# ../auth/backends/sql.py

"""Provides the SQL backend."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Python Imports
#   Thread
from contextlib import contextmanager
from threading import Thread

# Source.Python Imports
#   Auth
from auth.base import Backend
from auth.manager import auth_manager
from auth.manager import PlayerPermissions
from auth.manager import GroupPermissions
#   Paths
from paths import SP_DATA_PATH
#   Listeners
from listeners.tick import GameThread

# Site-Packges Imports
#   SQL Alechemy
from sqlalchemy import Column
from sqlalchemy import String
from sqlalchemy import Integer
from sqlalchemy import ForeignKey
from sqlalchemy import Enum
from sqlalchemy import create_engine
from sqlalchemy import Table
from sqlalchemy import UniqueConstraint
from sqlalchemy.orm import relationship
from sqlalchemy.orm import sessionmaker
from sqlalchemy.exc import IntegrityError
from sqlalchemy.ext.declarative import declarative_base


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('SQLBackend',
           'source',
           )


# =============================================================================
# >> GLOBAL VARIABLES
# =============================================================================
Base = declarative_base()
Session = sessionmaker()
groups_table = Table(
    'groups',
    Base.metadata,
    Column('group_id', Integer, ForeignKey('objects.id'), primary_key=True),
    Column('child_id', Integer, ForeignKey('objects.id'), primary_key=True)
)


# =============================================================================
# >> HELPER FUNCTIONS
# =============================================================================
@contextmanager
def session_scope():
    """Provide a transactional scope around a series of operations."""
    # http://docs.sqlalchemy.org/en/latest/orm/session_basics.html
    session = Session()
    try:
        yield session
        session.commit()
    except:
        session.rollback()
        raise
    finally:
        session.close()

def get_or_create(session, model, **kwargs):
    """Get or create a row."""
    instance = session.query(model).filter_by(**kwargs).first()
    if not instance:
        instance = model(**kwargs)
        session.add(instance)
        session.commit()

    return instance


# =============================================================================
# >> CLASSES
# =============================================================================
class Permission(Base):
    __tablename__ = 'permissions'
    __table_args__ = (UniqueConstraint('object_id', 'server_id', 'node',
        name='object_server_node_uc'),)

    id = Column(Integer, primary_key=True)
    object_id = Column(Integer, ForeignKey('objects.id'), nullable=False)
    server_id = Column(Integer, default=-1, nullable=False)
    node = Column(String(255), nullable=False)


class PermissionObject(Base):
    __tablename__ = 'objects'

    id = Column(Integer, primary_key=True)
    identifier = Column(String(64), nullable=False, unique=True)
    type = Column(Enum('Group', 'Player'), name='object_type')

    permissions = relationship('Permission', backref='object')
    children = relationship(
        'PermissionObject',
        secondary=groups_table,
        primaryjoin=id == groups_table.c.group_id,
        secondaryjoin=id == groups_table.c.child_id,
        backref='groups'
    )


class LoadThread(GameThread):
    def __init__(self):
        super().__init__()
        self._running = True

    def stop(self):
        self._running = False

    def run(self):
        with session_scope() as session:
            for node in session.query(PermissionObject).all():
                if not self._running:
                    break

                if node.type == 'Group':
                    store = auth_manager.groups[node.identifier]
                else:
                    store = auth_manager.players[node.identifier]

                for permission in node.permissions:
                    store.add(permission.node, update_backend=False)

                for group in node.groups:
                    store.add_group(group.identifier, update_backend=False)


class SQLBackend(Backend):
    """A backend that provides players and groups from an SQL database."""

    name = 'sql'
    options = {'uri': 'sqlite:///' + SP_DATA_PATH.joinpath('permissions.db')}

    def __init__(self):
        self.engine = None
        self.thread = None

    def load(self):
        self.engine = create_engine(self.options['uri'])
        Base.metadata.create_all(self.engine)
        Session.configure(bind=self.engine)
        self.thread = LoadThread()
        self.thread.start()

    def unload(self):
        self.thread.stop()
        self.thread.join()

    def permission_added(self, node, permission, server_id):
        try:
            with session_scope() as session:
                node_type, identifier = self.get_node_type_and_identifier(node)
                permission_obj = get_or_create(session, PermissionObject,
                    identifier=identifier, type=node_type)

                instance = Permission(
                    object_id=permission_obj.id,
                    server_id=server_id,
                    node=permission
                )
                session.add(instance)
        except IntegrityError:
            pass

    def permission_removed(self, node, permission, server_id):
        with session_scope() as session:
            node_type, identifier = self.get_node_type_and_identifier(node)
            permission_obj = session.query(PermissionObject).filter_by(
                identifier=identifier, type=node_type).one()

            session.query(Permission).filter_by(
                object_id=permission_obj.id,
                server_id=server_id,
                node=permission
            ).delete(False)

    def group_added(self, node, group_name):
        try:
            with session_scope() as session:
                node_type, identifier = self.get_node_type_and_identifier(node)
                child = get_or_create(session, PermissionObject,
                    identifier=identifier, type=node_type)

                group = get_or_create(session, PermissionObject,
                    identifier=group_name, type='Group')

                group_insert = groups_table.insert().values(
                    group_id=group.id,
                    child_id=child.id)
                session.execute(group_insert)
        except IntegrityError:
            pass

    def group_removed(self, node, group_name):
        with session_scope() as session:
            node_type, identifier = self.get_node_type_and_identifier(node)
            child = session.query(PermissionObject).filter_by(
                identifier=identifier, type=node_type).one()

            group = session.query(PermissionObject).filter_by(
                identifier=group_name, type='Group').one()

            session.query(groups_table).filter_by(
                child_id=child.id,
                group_id=group.id
            ).delete(False)

    @staticmethod
    def get_node_type_and_identifier(node):
        if isinstance(node, PlayerPermissions):
            result = ('Player', node.steamid64)
        elif isinstance(node, GroupPermissions):
            result = ('Group', node.name)
        else:
            raise TypeError(
                'Unexpected type "{}".'.format(type(node).__name__))

        return result

source = SQLBackend()
