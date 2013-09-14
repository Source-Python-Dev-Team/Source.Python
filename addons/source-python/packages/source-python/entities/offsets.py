# ../entities/offsets.py

# =============================================================================
# >> IMPORTS
# =============================================================================
# Python Imports
#   OS
from os import name as os_name

# Site Package Imports
#   ConfigObj
from configobj import ConfigObj

# Source.Python Imports
from core import GAME_NAME
from paths import SP_DATA_PATH


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
# Set all to an empty list
__all__ = []


# =============================================================================
# >> GLOBAL VARIABLES
# =============================================================================
# Store the base "offsets" path
_basepath = SP_DATA_PATH.joinpath('offsets')


# =============================================================================
# >> CLASSES
# =============================================================================
class _Offsets(dict):
    '''Dictionary that stores all entities with their offsets'''

    def __missing__(self, item):
        '''Called the first time an entity is added to the dictionary'''

        # Get all offsets for the given entity
        value = self[item] = _get_all_entity_offsets(item)

        # Return the offsets
        return value

    def get_entity_offsets(self, args):
        '''Returns all offsets for the given entities'''

        # Create an empty dictionary
        values = dict()

        # Loop through all given entities
        for arg in args:

            # Add the entities to the dictionary
            values.update(self[arg])

        # Return all offsets for the given entities
        return values

# Get the _Offsets instance
Offsets = _Offsets()


class _OffsetInstance(object):
    '''Class used to store an offset by its value and type'''

    def __init__(self, offset, offset_type):
        '''Stores the offset and type'''

        self.offset = offset
        self.type = offset_type


# =============================================================================
# >> FUNCTIONS
# =============================================================================
def _get_all_entity_offsets(entity):
    '''Retrieves all offsets for the given entity'''

    # Create an empty dictionary to pass
    game_offsets = {}

    # Get the path to the entity's offset file
    inifile = _basepath.joinpath(entity, GAME_NAME + '.ini')

    # Does the file exist?
    if not inifile.isfile():

        # Return the empty dictionary
        return game_offsets

    # Get the file's contents
    ini = ConfigObj(inifile, unrepr=True)

    # Loop through all items in the file
    for key in ini:

        # Get the offset for the Operating System
        offset = ini[key].get(os_name, ini[key]['linux'])

        # Add the item to the dictionary
        game_offsets[key] = _OffsetInstance(offset, ini[key]['type'])

    # Return the dictionary
    return game_offsets
