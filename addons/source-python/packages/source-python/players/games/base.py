# ../players/games/base.py

"""Provides base classes to be used when the game is not natively supported."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
#   Core
from core import GAME_NAME
#   Players
from players._language import get_client_language


# =============================================================================
# >> CLASSES
# =============================================================================
class _GamePlayer(object):
    """Base class for game specific weapon functionality."""

    # =========================================================================
    # >> BASE GAME-SPECIFIC FUNCTIONALITY
    # =========================================================================
    @property
    def hegrenade_indexes(self):
        """Yield all hegrenade indexes for the player."""
        raise NotImplementedError(
            'Not supported for game {0}.'.format(GAME_NAME))

    @property
    def get_hegrenade_ammo(self):
        """Return the amount of hegrenade ammo for the player."""
        raise NotImplementedError(
            'Not supported for game {0}.'.format(GAME_NAME))

    @property
    def set_hegrenade_ammo(self):
        """Set the amount of hegrenade ammo for the player."""
        raise NotImplementedError(
            'Not supported for game {0}.'.format(GAME_NAME))

    @property
    def flashbang_indexes(self):
        """Yield all flashbang indexes for the player."""
        raise NotImplementedError(
            'Not supported for game {0}.'.format(GAME_NAME))

    @property
    def get_flashbang_ammo(self):
        """Return the amount of flashbang ammo for the player."""
        raise NotImplementedError(
            'Not supported for game {0}.'.format(GAME_NAME))

    @property
    def set_flashbang_ammo(self):
        """Set the amount of flashbang ammo for the player."""
        raise NotImplementedError(
            'Not supported for game {0}.'.format(GAME_NAME))

    @property
    def smokegrenade_indexes(self):
        """Yield all smokegrenade indexes for the player."""
        raise NotImplementedError(
            'Not supported for game {0}.'.format(GAME_NAME))

    @property
    def get_smokegrenade_ammo(self):
        """Return the amount of smokegrenade ammo for the player."""
        raise NotImplementedError(
            'Not supported for game {0}.'.format(GAME_NAME))

    @property
    def set_smokegrenade_ammo(self):
        """Set the amount of smokegrenade ammo for the player."""
        raise NotImplementedError(
            'Not supported for game {0}.'.format(GAME_NAME))

    @property
    def decoy_indexes(self):
        """Yield all decoy indexes for the player."""
        raise NotImplementedError(
            'Not supported for game {0}.'.format(GAME_NAME))

    @property
    def get_decoy_ammo(self):
        """Return the amount of decoy ammo for the player."""
        raise NotImplementedError(
            'Not supported for game {0}.'.format(GAME_NAME))

    @property
    def set_decoy_ammo(self):
        """Set the amount of decoy ammo for the player."""
        raise NotImplementedError(
            'Not supported for game {0}.'.format(GAME_NAME))

    @property
    def incendiary_indexes(self):
        """Yield all incendiary indexes for the player."""
        raise NotImplementedError(
            'Not supported for game {0}.'.format(GAME_NAME))

    @property
    def get_incendiary_ammo(self):
        """Return the amount of incendiary ammo for the player."""
        raise NotImplementedError(
            'Not supported for game {0}.'.format(GAME_NAME))

    @property
    def set_incendiary_ammo(self):
        """Set the amount of incendiary ammo for the player."""
        raise NotImplementedError(
            'Not supported for game {0}.'.format(GAME_NAME))

    @property
    def has_c4(self):
        """Return whether or not the player is carrying C4."""
        raise NotImplementedError(
            'Not supported for game {0}.'.format(GAME_NAME))
