# ../entities/functions.py

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
#   DynCall
from dyncall.dictionary import SignatureDictionary
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


class _EntityFunctions(EntityAttributes):
    '''Dictionary that stores all entities with their functions'''

    type = 'functions'
    unrepr = True
    instance = _FunctionInstance

# Get the _EntityFunctions instance
EntityFunctions = _EntityFunctions()
