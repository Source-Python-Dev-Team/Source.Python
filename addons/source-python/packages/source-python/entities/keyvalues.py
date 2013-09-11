# ../entities/keyvalues.py

# =============================================================================
# >> IMPORTS
# =============================================================================
# Site Package Imports
#   ConfigObj
from configobj import ConfigObj

# Source.Python Imports
from core import GAME_NAME
from paths import DATA_PATH


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
# Set all to an empty list
__all__ = []


# =============================================================================
# >> GLOBAL VARIABLES
# =============================================================================
# Store the base "keyvalues" path
_basepath = DATA_PATH.joinpath('keyvalues')


# =============================================================================
# >> CLASSES
# =============================================================================
class _KeyValues(dict):
    '''Dictionary that stores all entities with their keyvalues'''

    def __missing__(self, item):
        '''Called the first time an entity is added to the dictionary'''

        # Get all keyvalues for the given entity
        value = self[item] = _get_all_entity_keyvalues(item)

        # Return the keyvalues
        return value

    def get_entity_keyvalues(self, args):
        '''Returns all keyvalues for the given entities'''

        # Create an empty dictionary
        values = dict()

        # Loop through all given entities
        for arg in args:

            # Add the entities to the dictionary
            values.update(self[arg])

        # Return all keyvalues for the given entities
        return values

# Get the _KeyValues instance
KeyValues = _KeyValues()


# =============================================================================
# >> FUNCTIONS
# =============================================================================
def _get_all_entity_keyvalues(entity):
    '''Retrieves all keyvalues for the given entity'''

    # Create an empty dictionary to pass
    game_keyvalues = {}

    # Get the path to the entity's keyvalue file
    inifile = _basepath.joinpath(entity, GAME_NAME + '.ini')

    # Does the file exist?
    if not inifile.isfile():

        # Return the empty dictionary
        return game_keyvalues

    # Get the file's contents
    ini = ConfigObj(inifile)

    # Loop through all items in the file
    for key in ini:

        # Add the item to the dictionary
        game_keyvalues[key] = ini[key]

    # Return the dictionary
    return game_keyvalues
