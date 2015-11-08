# ../entities/__init__.py

"""Provides entity based functionality."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
#   Loggers
from loggers import _sp_logger


# =============================================================================
# >> FORWARD IMPORTS
# =============================================================================
# Source.Python Imports
#   Entities
from _entities import BaseEntityGenerator
from _entities import BaseEntityHandle
from _entities import CheckTransmitInfo
from _entities import Edict
from _entities import EntityGenerator
from _entities import EntityListener
from _entities import GlobalEntityList
from _entities import HandleEntity
from _entities import ServerEntity
from _entities import ServerNetworkable
from _entities import ServerUnknown
from _entities import TakeDamageInfo


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('BaseEntityGenerator',
           'BaseEntityHandle',
           'CheckTransmitInfo',
           'Edict',
           'EntityGenerator',
           'EntityListener',
           'GlobalEntityList',
           'HandleEntity',
           'ServerEntity',
           'ServerNetworkable',
           'ServerUnknown',
           'TakeDamageInfo',
           )


# =============================================================================
# >> GLOBAL VARIABLES
# =============================================================================
# Get the sp.entities logger
entities_logger = _sp_logger.entities
