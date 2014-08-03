# ../entities/properties.py

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
#   Entities
from entities.attributes import EntityAttributes


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ()


# =============================================================================
# >> CLASSES
# =============================================================================
class _PropertyInstance(dict):
    '''Stores properties as a dictionary and
        allows access to them via attributes'''

    def __getattr__(self, attr):
        '''Override the __getattr__ method to
            return the item within the dictionary'''

        # Return the item for the given attribute
        return self.__getitem__(attr)


class _EntityProperties(EntityAttributes):
    '''Dictionary that stores all entities with their properties'''

    type = 'properties'
    unrepr = True
    instance = _PropertyInstance

# Get the _EntityProperties instance
EntityProperties = _EntityProperties()
