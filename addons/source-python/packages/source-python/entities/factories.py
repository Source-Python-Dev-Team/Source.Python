# ../entities/factories.py

"""Provides access to entity factories."""

# ============================================================================
# >> IMPORTS
# ============================================================================
# Source.Python Imports
#   Core
from core import SOURCE_ENGINE
#   Memory
from memory.manager import manager
#   Paths
from paths import SP_DATA_PATH


# =============================================================================
# >> FORWARD IMPORTS
# =============================================================================
# Source.Python Imports
#   Entities
from _entities._factories import EntityFactory
from _entities._factories import EntityFactoryDictionary


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('EntityFactory',
           'EntityFactoryDictionary',
           'factory_dictionary',
           )


# ============================================================================
# >> GLOBAL VARIABLES
# ============================================================================
# Get the factory_dictionary instance...
try:
    from _entities._factories import factory_dictionary
except ImportError:
    factory_dictionary = manager.get_global_pointer('EntityFactoryDictionary')
