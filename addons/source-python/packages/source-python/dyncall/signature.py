# ../dyncall/signature.py

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

# Source.Python Imports
from Source import Binutils
from core import GAME_NAME
#   DynCall
from dyncall.base import DynCallArgs
from dyncall.base import DynCallCalls
from dyncall.base import DynCallModes
from dyncall.base import DynCallVM
from dyncall.modules import ModuleData


# =============================================================================
# >> CLASSES
# =============================================================================
class Signature(object):
    '''Class used to call a dynamic function'''

    def __init__(self, ini):
        '''Called when the instance is initializes'''

        # Get the functions return type
        return_type = ini['return_type']

        # Is this a proper return type?
        if not return_type in DynCallCalls:

            # Raise an error about the return type
            raise ValueError('Unknown return type "{0}"'.format(return_type))

        # Get the module for the signature/symbol
        module = ini['module']

        # Fix any paths that use $gamename
        module = module.replace('$gamename', GAME_NAME)

        # Get the module instance
        module = ModuleData[module]

        # Is the server running on Windows?
        if os_name == 'nt':

            # Get the hex version of the signature
            sig = unhexlify(ini['sig'].replace(' ', ''))

            # Get the address of the signature
            self.address = Binutils.FindSignature(module, sig, len(sig))

        # Is the server not running on Windows?
        else:

            # Get the address of the symbol
            self.address = Binutils.FindSymbol(module, ini['symbol'])

        # Store the arguments, return type, and convention
        self.arguments = ini['arguments']
        self.return_type = return_type
        self.convention = DynCallModes.get(
            ini['convention'], DynCallModes[os_name])

    def call_function(self, *args):
        '''Calls the function with the given arguments'''

        # Were the correct number of arguments passed?
        if len(args) != len(self.arguments):

            # Raise an error about the number of arguments passed
            raise ValueError(
                'Given arguments ({0}) do not match required '
                'arguments ({1})'.format(len(args), len(self.arguments)))

        # Reset the vm
        Binutils.dcReset(DynCallVM)

        # Set the mode to the functions convention
        Binutils.dcMode(DynCallVM, self.convention)

        # Loop through all arguments
        for index in range(len(args)):

            # Is the current argument a known argument type?
            if self.arguments[index] in DynCallArgs:

                # Push the argument on the stack
                DynCallArgs[self.arguments[index]](DynCallVM, args[index])

            # Is the current argument of an unknown type?
            else:

                # Raise an error about the argument type
                raise ValueError(
                    'Unknown argument type "{0}"'.format(
                        self.arguments[index]))

        # Call the function
        return DynCallCalls[self.return_type](DynCallVM, self.address)
