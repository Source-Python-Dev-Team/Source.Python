# ../entities/factories.py

"""Provides access to entity factories."""

# ============================================================================
# >> IMPORTS
# ============================================================================
# Source.Python Imports
#   Memory
from memory.manager import manager


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


# =============================================================================
# >> GLOBAL VARIABLES
# =============================================================================
# The singleton object of the :class:`EntityFactoryDictionary` class.
factory_dictionary = None
