# ../core.py

# =============================================================================
# >> IMPORTS
# =============================================================================
# Python Imports
#   Inspect
from inspect import getmodule
from inspect import stack
#   OS
from os import sep
#   Platform
from platform import system
#   Sys
import sys

# Source.Python Imports
from constants import SOURCE_ENGINE
from engines.server import EngineServer
from paths import GAME_PATH


# =============================================================================
# >> FORWARD IMPORTS
# =============================================================================
# Source.Python Imports
#   Globals
from _globals import GlobalVars


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
# Add all the global variables to __all__
__all__ = [
    'AutoUnload',
    'GAME_NAME',
    'GlobalVars',
    'PLATFORM',
    'SOURCE_ENGINE',
    'echo_console',
]


# =============================================================================
# >> GLOBAL VARIABLES
# =============================================================================
# Get the specific game for the server
GAME_NAME = GAME_PATH.rsplit(sep, 2)[1]

# Get the platform the server is on
PLATFORM = system().lower()


# =============================================================================
# >> CLASSES
# =============================================================================
class AutoUnload(object):
    '''
        Class used to auto unload specific instances.

        Each class which inherits this one
            should have an _unload_instance method.
    '''

    def __new__(cls, *args, **kwargs):
        '''Overwrite __new__ to store the calling module'''

        # Get the class instance
        self = super(AutoUnload, cls).__new__(cls)

        # Get the calling module
        caller = getmodule(stack()[1][0])

        # Set the _calling_module attribute for the instance
        self._calling_module = caller.__name__

        # Return the instance
        return self

    def _unload_instance(self):
        '''Base _unload_instance method that, if
            called, raises a NotImplementedError'''
        raise NotImplementedError(
            'Class "{0}" from file "{1}" does not '.format(
                self.__class__.__name__, sys.modules[
                    self.__class__.__module__].__file__.split(
                    'plugins', 1)[1][1:]) +
            'have its own implementation of an _unload_instance method.')


# =============================================================================
# >> FUNCTIONS
# =============================================================================
def echo_console(text):
    '''echos a message to the server's console'''

    # Loop through each line in the text
    for line in text.split('\n'):

        # Echo the message
        EngineServer.server_command(
            'echo "{0}"\n'.format(line.replace('"', "'")))
