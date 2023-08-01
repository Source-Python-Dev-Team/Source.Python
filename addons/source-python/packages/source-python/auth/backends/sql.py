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
from auth.manager import ParentPermissions
#   Paths
from paths import SP_DATA_PATH
#   Threads
from threads import GameThread

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
parents_table = Table(
    'parents',
    Base.metadata,
    Column('parent_id', Integer, ForeignKey('objects.id'), primary_key=True),
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
    type = Column(Enum('Parent', 'Player'), name='object_type')

    permissions = relationship('Permission', backref='object')
    children = relationship(
        'PermissionObject',
        secondary=parents_table,
        primaryjoin=id == parents_table.c.parent_id,
        secondaryjoin=id == parents_table.c.child_id,
        backref='parents'
    )


class LoadThread(GameThread):
    def __init__(self):
        super().__init__()
        self._running = True

    def stop(self):
        self._running = False

    def run(self):
        with session_scope() as session:
            try:
                query = session.query(PermissionObject).all()
            except:
                raise
            else:
                # Only clear the manager if the query finished without any
                # errors
                auth_manager.parents.clear()
                auth_manager.players.clear()

            for node in query:
                if not self._running:
                    break

                if node.type == 'Parent':
                    store = auth_manager.parents[node.identifier]
                else:
                    store = auth_manager.players[node.identifier]

                for permission in node.permissions:
                    store.add(permission.node, update_backend=False)

                for parent in node.parents:
                    store.add_parent(parent.identifier, update_backend=False)


class SQLBackend(Backend):
    """A backend that provides players and parents from an SQL database."""

    name = 'sql'
    options = {'uri': 'sqlite:///' + SP_DATA_PATH.joinpath('permissions.db')}

    def __init__(self):
        self.engine = None
        self.thread = None

    def load(self):
        self.engine = create_engine(self.options['uri'])
        Base.metadata.create_all(self.engine)
        Session.configure(bind=self.engine)
        self.start_sync()

    def unload(self):
        self.stop_sync()

    def sync(self):
        self.stop_sync()
        self.start_sync()

    def start_sync(self):
        self.thread = LoadThread()
        self.thread.start()

    def stop_sync(self):
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

    def parent_added(self, node, parent_name):
        try:
            with session_scope() as session:
                node_type, identifier = self.get_node_type_and_identifier(node)
                child = get_or_create(session, PermissionObject,
                    identifier=identifier, type=node_type)

                parent = get_or_create(session, PermissionObject,
                    identifier=parent_name, type='Parent')

                parent_insert = parents_table.insert().values(
                    parent_id=parent.id,
                    child_id=child.id)
                session.execute(parent_insert)
        except IntegrityError:
            pass

    def parent_removed(self, node, parent_name):
        with session_scope() as session:
            node_type, identifier = self.get_node_type_and_identifier(node)
            child = session.query(PermissionObject).filter_by(
                identifier=identifier, type=node_type).one()

            parent = session.query(PermissionObject).filter_by(
                identifier=parent_name, type='Parent').one()

            session.query(parents_table).filter_by(
                child_id=child.id,
                parent_id=parent.id
            ).delete(False)

    @staticmethod
    def get_node_type_and_identifier(node):
        if isinstance(node, PlayerPermissions):
            result = ('Player', node.steamid64)
        elif isinstance(node, ParentPermissions):
            result = ('Parent', node.name)
        else:
            raise TypeError(
                'Unexpected type "{}".'.format(type(node).__name__))

        return result

source = SQLBackend()
