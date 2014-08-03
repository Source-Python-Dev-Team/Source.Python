# ../entities/offsets.py

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
from core import PLATFORM
#   Entities
from entities.attributes import EntityAttributes


# =============================================================================
# >> CLASSES
# =============================================================================
class _OffsetInstance(object):
    """Class used to store an offset by its value and type"""

    def __init__(self, offset):
        """Stores the offset and type"""

        self.offset = offset.get(PLATFORM, offset['linux'])
        self.type = offset['type']


class _EntityOffsets(EntityAttributes):
    """Dictionary that stores all entities with their offsets"""

    type = 'offsets'
    unrepr = True
    instance = _OffsetInstance

# Get the _EntityOffsets instance
EntityOffsets = _EntityOffsets()
