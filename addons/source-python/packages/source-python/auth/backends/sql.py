# ../auth/backends/flatfile.py

"""Provides the flat file backend."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
#   Auth
from auth.base import PermissionSource
from auth.manager import auth_manager
#   Paths
from paths import SP_DATA_PATH

from sqlalchemy import Column, String, Integer, ForeignKey, Enum, create_engine

from sqlalchemy.orm import relationship, sessionmaker

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

class Permission(Base):
    __tablename__ = "permissions"
    id = Column(Integer, primary_key=True)
    object_id = Column(Integer, ForeignKey("objects.id"))
    server_id = Column(Integer, default=-1)
    node = Column(String)


class PermissionObject(Base):
    __tablename__ = "objects"
    id = Column(Integer, primary_key=True)
    identifier = Column(String)
    type = Column(Enum("Group", "Player"), name="object_type")
    permissions = relationship("Permission")


class SQLPermissionSource(PermissionSource):
    """A backend that provides an admin and group file in JSON format and a
    simple text file.
    """
    name = 'sql'
    options = {
        'uri': 'sqlite:///' + SP_DATA_PATH.joinpath('permissions.db'),
        "server_id": -1
    }
    engine = None

    def load(self):
        self.engine = create_engine(self.options['uri'])
        Base.metadata.create_all(self.engine)
        Session.configure(bind=self.engine)
        session = Session()
        obj = session.query(PermissionObject).first()
        print([permission.node for permission in obj.permissions if
               (permission.server_id == -1 or permission.server_id == int(self.options["server_id"]))])


source = SQLPermissionSource()
