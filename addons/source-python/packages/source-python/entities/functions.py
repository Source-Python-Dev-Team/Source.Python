# ../entities/functions.py

# =============================================================================
# >> IMPORTS
# =============================================================================
# Site Package Imports
#   ConfigObj
from configobj import ConfigObj

# Source.Python Imports
from core import GAME_NAME
from paths import SP_DATA_PATH
#   DynCall
from dyncall.dictionary import SignatureDictionary


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
# Set all to an empty list
__all__ = []


# =============================================================================
# >> GLOBAL VARIABLES
# =============================================================================
# Store the base "functions" path
_basepath = SP_DATA_PATH.joinpath('functions')


# =============================================================================
# >> CLASSES
# =============================================================================
class _Functions(dict):
    '''Dictionary that stores all entities with their functions'''

    def __missing__(self, item):
        '''Called the first time an entity is added to the dictionary'''

        # Get all functions for the given entity
        value = self[item] = _get_all_entity_functions(item)

        # Return the functions
        return value

    def get_entity_functions(self, args):
        '''Returns all functions for the given entities'''

        # Create an empty dictionary
        values = dict()

        # Loop through all given entities
        for arg in args:

            # Add the entities to the dictionary
            values.update(self[arg])

        # Return all functions for the given entities
        return values

# Get the _Functions instance
Functions = _Functions()


class _FunctionInstance(object):
    '''Class used to store a function to be called with
        the entity's pointer as the first argument'''

    def __init__(self, item):
        '''Called on initialization'''

        # Get the name of the function
        name = item['name']

        # Is the given name registered in the dictionary?
        if not name in SignatureDictionary:

            # Raise an error
            raise KeyError(
                'No function "{0}" stored in SignatureDictionary'.format(name))

        # Store the function to be called
        self.function = SignatureDictionary[name]

        # Store the index to add the pointer
        self.pointer_index = item['pointer']

        # Store the pointer as None until it is needed
        self.current_pointer = None

    def _pre_call_function(self, *args):
        '''Adds the entity's pointer as the first
            argument when calling the function'''

        # Does the pointer need added?
        if self.current_pointer:

            # Add the pointer to the arguments
            args = (
                args[:self.pointer_index] +
                (self.current_pointer, ) +
                args[self.pointer_index:])

        # Call the function with the entity's pointer as the first argument
        self.function.call_function(*args)

        # Reset the current pointer
        self.current_pointer = None


# =============================================================================
# >> FUNCTIONS
# =============================================================================
def _get_all_entity_functions(entity):
    '''Retrieves all functions for the given entity'''

    # Create an empty dictionary to pass
    game_functions = {}

    # Get the path to the entity's function file
    inifile = _basepath.join(entity, GAME_NAME + '.ini')

    # Does the file exist?
    if not inifile.isfile():

        # Return the empty dictionary
        return game_functions

    # Get the file's contents
    ini = ConfigObj(inifile, unrepr=True)

    # Loop through all items in the file
    for key in ini:

        # Add the item to the dictionary
        game_functions[key] = _FunctionInstance(ini[key])

    # Return the dictionary
    return game_functions
