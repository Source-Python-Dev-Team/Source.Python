# ../players/games/base.py

"""Provides base classes to be used when the game is not natively supported."""

# =============================================================================
# >> IMPORTS
# =============================================================================
from players._language import get_client_language


# =============================================================================
# >> CLASSES
# =============================================================================
class _GamePlayer(object):
    """Base class for game specific functionality."""
