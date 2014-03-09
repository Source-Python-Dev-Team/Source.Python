# ../memory/signature.py

# =============================================================================
# >> IMPORTS
# =============================================================================
# Python Imports
#   Binascii
from binascii import unhexlify
#   OS
from os import name as os_name
if os_name != 'nt':
    os_name = 'linux'
#   Warnings
from warnings import warn

# Source.Python Imports
from memory_c import Argument
from memory_c import Convention
from memory_c import Return
from core import GAME_NAME
#   Memory
from memory.modules import ModuleData


# =============================================================================
# >> CLASSES
# =============================================================================
class Signature(object):
    '''Class used to call a dynamic function'''

    def __init__(self, ini):
        '''Called when the instance is initializes'''

        # Get the functions return type
        return_type = getattr(Return, ini['return_type'])

        # Get the module for the signature/symbol
        module = ini['module']

        # Fix any paths that use $gamename
        module = module.replace('$gamename', GAME_NAME)

        # Get the module instance
        module = ModuleData[module]

        # Is the server running on Windows?
        if os_name == 'nt':

            # Get the hex version of the signature
            func = unhexlify(ini['sig'].replace(' ', ''))

        # Is the server not running on Windows?
        else:

            # Use the symbol for Linux
            func = ini['symbol']

        # Get the address of the function
        address = module.find_address(func)

        # Was the function found?
        if not address:

            # Raise an error
            raise ValueError('Unable to find address')

        # Get the function's arguments
        arguments = ini['arguments']

        # Is there only one argument for this function?
        if type(arguments) == str:

            # Make a tuple with the argument type
            arguments = tuple([getattr(Argument, arguments)])

        # Are there multiple arguments for this function?
        else:

            # Get a tuple of the arguments
            arguments = tuple([getattr(Argument, x) for x in arguments])

        # Get the convention
        convention = getattr(Convention, ini['convention'])

        # Store the function for later use
        self.function = address.make_function(
            convention, arguments, return_type)

    def __call__(self, *args):
        '''Call the function with the given arguments'''
        return self.function(*args)
