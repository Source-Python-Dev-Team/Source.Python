# ../commands/player.py

"""Extends the base command manager for player type commands/filters."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
#   Commands
from commands.auth import _AuthCallback
from commands.manager import _BaseCommandManager


# =============================================================================
# >> CLASSES
# =============================================================================
class _PlayerCommandManager(_BaseCommandManager):

    """Base class used to (un)register client/say commands."""

    # Store the base attributes
    _use_args = False
    _callback_manager = _AuthCallback
