# ../auth/backends/sql.py

"""Provides the SQL backend."""

# =============================================================================
# >> IMPORTS
# =============================================================================
from threading import Thread
# Source.Python Imports
#   Auth
from auth.base import PermissionSource
from auth.manager import auth_manager
#   Paths
from paths import SP_DATA_PATH
from listeners import on_tick_listener_manager

from sqlalchemy import Column, String, Integer, ForeignKey, Enum, create_engine, Table, UniqueConstraint

from sqlalchemy.orm import relationship, sessionmaker

from sqlalchemy.exc import SQLAlchemyError

from sqlalchemy.ext.declarative import declarative_base

Base = declarative_base()
Session = sessionmaker()


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('SQLPermissionSource',
           'source',
           )


# =============================================================================
# >> CLASSES
# =============================================================================
class SPThread(Thread):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        on_tick_listener_manager.register_listener(self._tick)

    def _tick(self):
        pass

parents_table = Table('parents', Base.metadata,
                      Column('parent_id', Integer, ForeignKey('objects.id'), primary_key=True),
                      Column('child_id', Integer, ForeignKey('objects.id'), primary_key=True)
                      )


class Permission(Base):
    __tablename__ = 'permissions'
    id = Column(Integer, primary_key=True)
    object_id = Column(Integer, ForeignKey('objects.id'), nullable=False)
    server_id = Column(Integer, default=-1, nullable=False)
    node = Column(String(255), nullable=False)
    __table_args__ = (UniqueConstraint('object_id', 'server_id', 'node', name='object_server_node_uc'),)


class PermissionObject(Base):
    __tablename__ = 'objects'
    id = Column(Integer, primary_key=True)
    identifier = Column(String(64), nullable=False, unique=True)
    type = Column(Enum('Group', 'Player'), name='object_type')
    permissions = relationship('Permission', backref='object')
    children = relationship('PermissionObject',
                            secondary=parents_table,
                            primaryjoin=id == parents_table.c.parent_id,
                            secondaryjoin=id == parents_table.c.child_id,
                            backref='parents'
                            )


class SQLPermissionSource(PermissionSource):
    """A backend that provides admins and groups from an SQL database.
    """
    name = 'sql'
    options = {
        'uri': 'sqlite:///' + SP_DATA_PATH.joinpath('permissions.db'),
        'server_id': -1
    }
    engine = None

    def load(self):
        try:
            self.engine = create_engine(self.options['uri'])
            Base.metadata.create_all(self.engine)
            Session.configure(bind=self.engine)
        except SQLAlchemyError as e:
            print(e)
            return
        SPThread(target=self._do_load).start()

    def _do_load(self):
        session = Session()
        for node in session.query(PermissionObject).all():
            if node.type == 'Group':
                store = auth_manager.groups[node.identifier]
            else:
                store = auth_manager.players[node.identifier]
            for permission in node.permissions:
                if permission.server_id == -1 or permission.server_id == int(self.options['server_id']):
                    store.add(permission.node)
        session.close()

source = SQLPermissionSource()
