# ../entities/__init__.py

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
from _entities import BaseEntityHandle
from _entities import Edict
from _entities import EntityGenerator
from _entities import HandleEntity
from _entities import ServerEntity
from _entities import ServerNetworkable
from _entities import ServerUnknown
from loggers import _SPLogger


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
# Set all to an empty list
__all__ = [
    'BaseEntityHandle',
    'Edict',
    'EntityGenerator',
    'HandleEntity',
    'ServerEntity',
    'ServerNetworkable',
    'ServerUnknown',
]


# =============================================================================
# >> GLOBAL VARIABLES
# =============================================================================
# Get the sp.entities logger
EntitiesLogger = _SPLogger.entities
