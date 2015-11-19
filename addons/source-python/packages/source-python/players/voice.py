# ../players/voice.py

"""Provides functions specific to the player's voice."""


# =============================================================================
# >> IMPORTS
# =============================================================================
# Python
import collections

# Source.Python
#    Engines
from engines.server import global_vars
#    Listeners
from listeners import OnClientDisconnect
#    Memory
import memory

from memory.hooks import PreHook


# =============================================================================
# >> FORWARD IMPORTS
# =============================================================================
# Source.Python Imports
#  Voice
from _players._voice import voice_server


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('_MuteManager',
           'mute_manager',
           'voice_server',
           )


# =============================================================================
# >> CLASSES
# =============================================================================
class _MuteManager(collections.defaultdict):
    """A singleton that manages muting players."""

    @staticmethod
    def _get_receivers(receivers):
        """Return a tuple containing player indexes.

        If <receivers> is None, a tuple is returned that contains all valid
        player indexes. Otherwise the given argument is returned after it has
        been validated.
        """
        if receivers is None:
            return range(1, global_vars.max_clients + 1)

        # Check if "receivers" contains valid player indexes
        if not all(map(lambda index: (isinstance(
                index, int) and 0 < index <= global_vars.max_clients),
                receivers)):
            raise ValueError(
                '"receivers" doesn\'t contain valid player indexes.')

        return receivers

    def mute_player(self, sender, receivers=None):
        """Mute a player, so other players can't hear him talking.

        The muted player will still hear the other players.

        If <receivers> is None, the sender will be muted for all players
        currently on the server and all future players.

        If you want to mute the sender only for specific players, pass a tuple
        that contains the player indexes that shouldn't hear the sender
        anymore.
        """
        for receiver in self._get_receivers(receivers):
            self[receiver].add(sender)

    def unmute_player(self, sender, receivers=None):
        """Unmute a player, so other players can hear him again.

        If <receivers> is None, the sender will be unmuted for all players
        currently on the server and all future players.

        If you want to unmute the sender only for specific players, pass a
        tuple that contains the player indexes that should hear the sender
        again.
        """
        for receiver in self._get_receivers(receivers):
            self[receiver].discard(sender)

    def is_muted(self, sender, receivers=None):
        """Return True if a player is muted.

        If <receivers> is None, it will be check if the players is muted for
        everyone.

        If you want to check if the player is muted only for specific players,
        pass a tuple that contains the player indexes that should be checked.
        """
        return all(map(
            lambda receiver: sender in self[receiver],
            self._get_receivers(receivers)))

# The singleton object of the :class:`_MuteManager` class
mute_manager = _MuteManager(set)


# =============================================================================
# >> CALLBACKS
# =============================================================================
@PreHook(memory.get_virtual_function(voice_server, 'SetClientListening'))
def _pre_set_client_listening(args):
    """Called before IVoiceServer::SetClientListening is called."""
    receiver = args[1]
    sender = args[2]

    # Check if the sender is muted for this receiver
    if mute_manager.is_muted(sender, [receiver]):
        args[3] = False


@OnClientDisconnect
def _on_client_disconnect(index):
    """Called when a player left the server."""
    # Unmute the player, so the next player who gets this index won't be muted
    mute_manager.unmute_player(index)
