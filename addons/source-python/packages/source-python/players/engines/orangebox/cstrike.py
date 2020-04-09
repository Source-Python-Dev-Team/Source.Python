# ../players/engines/orangebox/cstrike.py

"""Provides CS:S specific Player based functionality."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
#   Engines
from engines.server import engine_server
#   Entities
from entities.entity import BaseEntity
#   Filters
from filters.entities import EntityIter
#   Memory
from memory import get_virtual_function
from memory.hooks import PreHook
#   Players
from . import Player as _Player
from players.constants import LifeState


# =============================================================================
# >> CLASSES
# =============================================================================
class Player(_Player):
    """Class used to interact directly with players."""

    # Instances of this class will be cached by default
    caching = True

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

    def spawn(self, force=False):
        """Spawn the player.

        :param bool force: Whether or not the spawn should be forced.
        """
        # Is the player spawnable?
        if not force and (self.team <= 1 or not self.dead):
            return

        # Spawn the player...
        self.player_state = 0
        self.life_state = LifeState.ALIVE
        BaseEntity.spawn(self)


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
