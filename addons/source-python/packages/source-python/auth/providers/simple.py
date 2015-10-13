# ../auth/providers/simple.py

"""A basic Authorization Service."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Python Imports
#   RE
import re

# Source.Python Imports
#   Auth
from auth.providers import AuthBase
from auth.paths import AUTH_CFG_PATH


# =============================================================================
# >> GLOBAL VARIABLES
# =============================================================================
# Store the path to the simple.txt file
_SIMPLE_FILE_PATH = AUTH_CFG_PATH.joinpath('simple.txt')


# =============================================================================
# >> CLASSES
# =============================================================================
class _SimpleAuth(AuthBase, set):
    """Class used to determine if a player is authorized."""

    def load(self):
        """Called when the service is loaded."""
        self._parse_admins()

    def unload(self):
        """Called when the service is unloaded."""
        self.clear()

    def _parse_admins(self):
        """Retrieve all uniqueids that are authorized on the server."""
        # Open the simple auth config file
        with _SIMPLE_FILE_PATH.open() as auth_file:

            # Loop through each line in the file
            for line in auth_file.readlines():

                # Get the match on the current line
                match = re.match('(^STEAM_[0,1]{1}:[0,1]{1}:[0-9]+)', line)

                # Is there a uniqueid on the line?
                if match:

                    # Add the uniqueid to the set
                    self.add(match.group(0))

    def is_player_authorized(self, uniqueid, level, permission, flag):
        """Method used to check if a player is authorized."""
        # Is the player's uniqueid in the set?
        if uniqueid in self:

            # If so, return True
            return True

        # If not, return False
        return False

# Get the _SimpleAuth instance
simple_auth = _SimpleAuth()
