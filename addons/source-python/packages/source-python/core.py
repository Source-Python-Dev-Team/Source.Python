# ../core.py

# =============================================================================
# >> IMPORTS
# =============================================================================
# Python Imports
#   OS
from os import sep

# Source.Python Imports
from constants_c import SOURCE_ENGINE
from engine_c import EngineServer
from paths import GAME_PATH


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
# Add all the global variables to __all__
__all__ = [
    'AutoUnload',
    'GAME_NAME',
    'GameEngine',
    'SOURCE_ENGINE',
    'echo_console',
]


# =============================================================================
# >> GLOBAL VARIABLES
# =============================================================================
# Get the specific game for the server
GAME_NAME = GAME_PATH.rsplit(sep, 2)[1]

# Set GameEngine variable
GameEngine = EngineServer


# =============================================================================
# >> CLASSES
# =============================================================================
class AutoUnload(object):
    '''
        Class used to auto unload specific instances.

        Each class which inherits this one
        should have a _unload_instance method.
    '''


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
