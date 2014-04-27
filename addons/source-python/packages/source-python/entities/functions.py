# ../entities/functions.py

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
from core import PLATFORM
#   Entities
from entities.attributes import EntityAttributes
#   Memory
from memory import Convention
from memory import Function
from memory import Return
from memory.helpers import Key
from memory.helpers import NO_DEFAULT
from memory.helpers import parse_data
from memory.manager import manager


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
# Set all to an empty list
__all__ = []


# =============================================================================
# >> GLOBAL VARIABLES
# =============================================================================
# Store the ini data checkers
_ini_data = (
    (Key.BINARY, str, NO_DEFAULT),
    (Key.IDENTIFIER, Key.as_identifier, NO_DEFAULT),
    (Key.ARGS, Key.as_args_tuple, ()),
    (Key.RETURN_TYPE, Key.as_return_type, Return.VOID),
    (Key.CONVENTION, Key.as_convention, Convention.CDECL),
    (Key.SRV_CHECK, Key.as_bool, True),
    (Key.DOC, str, None)
)


# =============================================================================
# >> CLASSES
# =============================================================================
class _FunctionInstance(Function):
    '''Class used to store a function to be called with
        the entity's pointer as the first argument'''

    def __init__(self, item):
        '''Called on initialization'''

        # Create a list to store the function data
        data = list()

        # Loop through the data checkers
        for key, converter, default in _ini_data:

            # Get the function's value for the current data
            value = item.get(key + '_' + PLATFORM, item.get(key, default))

            # Is the value invalid?
            if value is NO_DEFAULT:
                raise KeyError(
                    'Missing information for key "{0}".'.format(key))

            # Add the current value to the data list
            data.append(value if value is default else converter(value))

        # Get the Function instance
        function = manager.pipe_function(*data)

        # Initialize the Function instance
        super(_FunctionInstance, self).__init__(function)

        # Store the index to add the pointer
        self.pointer_index = int(item.get('pointer_index', 0))

        # Store the pointer as None until it is needed
        self.current_pointer = None

    def __call__(self, *args):
        '''Adds the entity's pointer as the <pointer_index>
            argument when calling the function'''

        # Add the pointer to the arguments
        args = (
            args[:self.pointer_index] +
            (self.current_pointer, ) +
            args[self.pointer_index:])

        # Call the function
        super(_FunctionInstance, self).__call__(*args)

        # Reset the current pointer
        self.current_pointer = None


class _EntityFunctions(EntityAttributes):
    '''Dictionary that stores all entities with their functions'''

    type = 'functions'
    unrepr = False
    instance = _FunctionInstance

# Get the _EntityFunctions instance
EntityFunctions = _EntityFunctions()
