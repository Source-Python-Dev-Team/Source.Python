# ../players/games/base.py

"""Provides base classes to be used when the game is not natively supported."""

# =============================================================================
# >> IMPORTS
# =============================================================================
from players.classes.language import get_client_language


# =============================================================================
# >> CLASSES
# =============================================================================
class _GameWeapons(object):
    """Base class for game specific weapon functionality."""
