# ../entities/factories.py

"""Provides access to entity factories."""

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
# Get the temp_entities instance...
try:
    from _entities import factory_dictionary
except ImportError:
    # TODO: Sigscan "s_EntityFactory" for engines that doesn't support
    #   IServerTools::GetEntityFactoryDictionary such as CS:GO...
    factory_dictionary = None
