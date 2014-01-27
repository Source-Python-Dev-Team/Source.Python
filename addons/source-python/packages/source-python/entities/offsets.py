# ../entities/offsets.py

# =============================================================================
# >> IMPORTS
# =============================================================================
# Python Imports
#   OS
from os import name as os_name

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
class _OffsetInstance(object):
    '''Class used to store an offset by its value and type'''

    def __init__(self, offset):
        '''Stores the offset and type'''

        self.offset = offset.get(os_name, offset['linux'])
        self.type = offset['type']


class _EntityOffsets(EntityAttributes):
    '''Dictionary that stores all entities with their offsets'''

    type = 'offsets'
    unrepr = True
    instance = _OffsetInstance

# Get the _EntityOffsets instance
EntityOffsets = _EntityOffsets()
