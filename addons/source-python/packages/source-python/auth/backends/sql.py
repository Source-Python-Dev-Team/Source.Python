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
from auth.base import PermissionSource
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
from sqlalchemy.exc import SQLAlchemyError
from sqlalchemy.ext.declarative import declarative_base


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('SQLPermissionSource',
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
    __table_args__ = (UniqueConstraint(
        'identifier', 'object_type', name='identifier_type_uc'),)

    id = Column(Integer, primary_key=True)
    identifier = Column(String(64), nullable=False)
    type = Column(Enum('Group', 'Player'), name='object_type')

    permissions = relationship('Permission', backref='object')
    children = relationship('PermissionObject',
                            secondary=parents_table,
                            primaryjoin=id == parents_table.c.parent_id,
                            secondaryjoin=id == parents_table.c.child_id,
                            backref='parents'
                            )


class SQLPermissionSource(PermissionSource):
    """A backend that provides admins and groups from an SQL database."""

    name = 'sql'
    options = {
        'uri': 'sqlite:///' + SP_DATA_PATH.joinpath('permissions.db'),
        'server_id': -1
    }
    engine = None

    def load(self):
        self.engine = create_engine(self.options['uri'])
        Base.metadata.create_all(self.engine)
        Session.configure(bind=self.engine)

        # TODO:
        # Currently, we can't use threading for this task, because it will be
        # executed after the active backend has been set. And this results in
        # triggering the permission_added/removed, etc. callbacks. This is
        # wrong and causes exception, because the permissions are saved to the
        # database.
        #GameThread(target=self._do_load).start()
        self._do_load()

    def permission_added(self, node, permission):
        server_id = int(self.options['server_id'])
        with session_scope() as session:
            permission_obj = get_or_create(session, PermissionObject,
                identifier=node.name, type=self.get_node_type(node))

            instance = Permission(
                object_id=permission_obj.id,
                server_id=server_id, # TODO: What if someone wants to pass -1?
                node=permission
            )
            session.add(instance)

    def permission_removed(self, node, permission):
        server_id = int(self.options['server_id'])
        with session_scope() as session:
            permission_obj = session.query(PermissionObject).filter_by(
                identifier=node.name, type=self.get_node_type(node)).one()

            session.query(Permission).filter_by(
                object_id=permission_obj.id,
                server_id=server_id, # TODO: What if someone wants to pass -1?
                node=permission
            ).delete(False)

    def parent_added(self, node, parent_name):
        with session_scope() as session:
            child = get_or_create(session, PermissionObject,
                identifier=node.name, type=self.get_node_type(node))

            parent = get_or_create(session, PermissionObject,
                identifier=parent_name, type='Group')

            parent_insert = parents_table.insert().values(
                parent_id=parent.id,
                child_id=child.id)
            session.execute(parent_insert)

    def parent_removed(self, node, parent_name):
        with session_scope() as session:
            child = session.query(PermissionObject).filter_by(
                identifier=node.name, type=self.get_node_type(node)).one()

            parent = session.query(PermissionObject).filter_by(
                identifier=parent_name, type='Group').one()

            session.query(parents_table).filter_by(
                child_id=child.id,
                parent_id=parent.id
            ).delete(False)

    @staticmethod
    def get_node_type(node):
        if isinstance(node, PlayerPermissions):
            node_type = 'Player'
        elif isinstance(node, GroupPermissions):
            node_type = 'Group'
        else:
            raise TypeError(
                'Unexpected type "{}".'.format(type(node).__name__))

        return node_type

    def _do_load(self):
        server_id = int(self.options['server_id'])
        with session_scope() as session:
            for node in session.query(PermissionObject).all():
                if node.type == 'Group':
                    store = auth_manager.groups[node.identifier]
                else:
                    store = auth_manager.players[node.identifier]

                for permission in node.permissions:
                    if permission.server_id in (-1, server_id):
                        store.add(permission.node)

                for parent in node.parents:
                    store.add_parent(parent.identifier)

source = SQLPermissionSource()
