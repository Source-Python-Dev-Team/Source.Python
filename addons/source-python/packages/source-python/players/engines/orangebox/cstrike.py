# ../players/engines/orangebox/cstrike.py

"""Provides CS:S specific Player based functionality."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
#   Engines
from engines.server import engine_server
#   Filters
from filters.entities import EntityIter
#   Memory
from memory import get_virtual_function
from memory.hooks import PreHook
#   Players
from . import Player as _Player


# =============================================================================
# >> CLASSES
# =============================================================================
class Player(_Player):
    """Class used to interact directly with players."""

    def has_c4(self):
        """Return whether or not the player is carrying C4."""
        # Loop through all c4 entities on the server
        for weapon in EntityIter('weapon_c4'):

            # Is the entity's "owner" the player?
            if weapon.owner_handle == self.inthandle:

                # Return True
                return True

        # If no c4 is owned by the player, return False
        return False


# =============================================================================
# >> CALLBACKS
# =============================================================================
@PreHook(get_virtual_function(engine_server, 'ClientCommand'))
def _pre_client_command(args):
    """Block name changes started by the server.

    Pre-hook on IVEngineServer::ClientCommand to block the name changes.
    """
    if args[2] == 'name "%s"':
        return 0
