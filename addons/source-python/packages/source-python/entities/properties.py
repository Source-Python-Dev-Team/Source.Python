# ../entities/properties.py

# =============================================================================
# >> IMPORTS
# =============================================================================
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
# Store the base "properties" path
_basepath = SP_DATA_PATH.joinpath('properties')


# =============================================================================
# >> CLASSES
# =============================================================================
class _Properties(dict):
    '''Dictionary that stores all entities with their properties'''

    def __missing__(self, item):
        '''Called the first time an entity is added to the dictionary'''

        # Get all properties for the given entity
        value = self[item] = _get_all_entity_properties(item)

        # Return the properties
        return value

    def get_entity_properties(self, args):
        '''Returns all properties for the given entities'''

        # Create an empty dictionary
        values = dict()

        # Loop through all given entities
        for arg in args:

            # Add the entities to the dictionary
            values.update(self[arg])

        # Return all properties for the given entities
        return values

# Get the _Properties instance
Properties = _Properties()


class _PropertyInstance(dict):
    '''Stores properties as a dictionary and
        allows access to them via attributes'''

    def __getattr__(self, attr):
        '''Override the __getattr__ method to
            return the item within the dictionary'''

        # Return the item for the given attribute
        return self.__getitem__(attr)


# =============================================================================
# >> FUNCTIONS
# =============================================================================
def _get_all_entity_properties(entity):
    '''Retrieves all properties for the given entity'''

    # Create an empty dictionary to pass
    game_properties = {}

    # Get the path to the entity's property file
    inifile = _basepath.joinpath(entity, GAME_NAME + '.ini')

    # Does the file exist?
    if not inifile.isfile():

        # Return the empty dictionary
        return game_properties

    # Get the file's contents
    ini = ConfigObj(inifile, unrepr=True)

    # Loop through all items in the file
    for key in ini:

        # Add the item to the dictionary
        game_properties[key] = _PropertyInstance(ini[key])

    # Return the dictionary
    return game_properties
