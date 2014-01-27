# ../entities/keyvalues.py

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
#   Entities
from entities.attributes import EntityAttributes


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
# Set all to an empty list
__all__ = []


# =============================================================================
# >> CLASSES
# =============================================================================
class _EntityKeyValues(EntityAttributes):
    '''Dictionary that stores all entities with their keyvalues'''

    type = 'keyvalues'
    unrepr = False
    instance = staticmethod(lambda value: value)

# Get the _EntityKeyValues instance
EntityKeyValues = _EntityKeyValues()
