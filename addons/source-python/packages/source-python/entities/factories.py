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
from _entities import EntityFactory
from _entities import EntityFactoryDictionary


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('factory_dictionary',
           'EntityFactory',
           'EntityFactoryDictionary',
           )


# ============================================================================
# >> GLOBAL VARIABLES
# ============================================================================
# Get the factory_dictionary instance...
try:
    from _entities import factory_dictionary
except ImportError:
    manager.create_global_pointers_from_file(SP_DATA_PATH.joinpath(
        'entities', 'factories', SOURCE_ENGINE + '.ini'))
    factory_dictionary = manager.get_global_pointer('EntityFactoryDictionary')
