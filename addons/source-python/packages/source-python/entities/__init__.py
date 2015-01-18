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
from _entities import BaseEntityHandle
from _entities import Edict
from _entities import EntityGenerator
from _entities import HandleEntity
from _entities import ServerEntity
from _entities import ServerNetworkable
from _entities import ServerUnknown


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('BaseEntityHandle',
           'Edict',
           'EntityGenerator',
           'HandleEntity',
           'ServerEntity',
           'ServerNetworkable',
           'ServerUnknown',
           )


# =============================================================================
# >> GLOBAL VARIABLES
# =============================================================================
# Get the sp.entities logger
entities_logger = _sp_logger.entities
