# ../auth/providers/simple.py

# =============================================================================
# >> IMPORTS
# =============================================================================
# Python Imports
#   RE
import re

# Source.Python Imports
#   Auth
from auth.paths import AUTH_CFG_PATH


# =============================================================================
# >> GLOBAL VARIABLES
# =============================================================================
# Store the path to the simple.txt file
SIMPLE_FILE_PATH = AUTH_CFG_PATH.joinpath('simple.txt')


# =============================================================================
# >> CLASSES
# =============================================================================
class _SimpleAuth(set):
    '''Class used to determine if a player is authorized'''

    def _parse_admins(self):
        '''
            Method used to get all uniqueids that are authorized on the server
        '''

        # Open the simple auth config file
        with SIMPLE_FILE_PATH.open() as auth_file:

            # Loop through each line in the file
            for line in auth_file.readlines():

                # Get the match on the current line
                match = re.match('(^STEAM_[0,1]{1}:[0,1]{1}:[0-9]+)', line)

                # Is there a uniqueid on the line?
                if match:

                    # Add the uniqueid to the set
                    self.add(match.group(0))

    def is_player_authorized(self, uniqueid, level, permission, flag):
        '''Method used to check if a player is authorized'''

        # Is the player's uniqueid in the set?
        if uniqueid in self:

            # If so, return True
            return True

        # If not, return False
        return False

# Get the _SimpleAuth instance
SimpleAuth = _SimpleAuth()

is_player_authorized = SimpleAuth.is_player_authorized


# =============================================================================
# >> FUNCTIONS
# =============================================================================
def load():
    '''Loads the provider by getting all uniqueids that are authorized'''

    # Parse the simple auth file
    SimpleAuth._parse_admins()


def unload():
    '''Unloads the provider by clearing the set'''

    # Clear the set
    SimpleAuth.clear()
