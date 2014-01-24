# ../core.py

# =============================================================================
# >> IMPORTS
# =============================================================================
# Python Imports
#   OS
from os import sep

# Source.Python Imports
from engine_c import GameEngine
from paths import GAME_PATH
from public import public


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
# Add all the global variables to __all__
__all__ = ['GAME_NAME', 'GameEngine']


# =============================================================================
# >> GLOBAL VARIABLES
# =============================================================================
# Get the specific game for the server
GAME_NAME = GAME_PATH.rsplit(sep, 2)[1]


# =============================================================================
# >> CLASSES
# =============================================================================
@public
class AutoUnload(object):
    '''
        Class used to auto unload specific instances.

        Each class which inherits this one
        should have a _unload_instance method.
    '''


# =============================================================================
# >> FUNCTIONS
# =============================================================================
@public
def echo_console(text):
    '''echos a message to the server's console'''

    # Loop through each line in the text
    for line in text.split('\n'):

        # Echo the message
        GameEngine.server_command(
            'echo "{0}"\n'.format(line.replace('"', "'")))
