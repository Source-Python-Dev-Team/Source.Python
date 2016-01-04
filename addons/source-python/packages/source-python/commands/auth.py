# ../commands/auth.py

"""Provides a proxy for authorization checks for client and say commands."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
#   Commands
from commands import CommandReturn
#   Auth
from auth.manager import auth_manager


# =============================================================================
# >> CLASSES
# =============================================================================
class _AuthCallback(object):
    """Command Authorization callback hook class."""

    def __init__(self, callback, permission=None, fail_callback=None):
        """Store all the given arguments."""
        self.callback = callback
        self.permission = permission
        self.fail_callback = fail_callback

    def __call__(self, *args):
        """Verify the player's authorization."""
        if self.permission is None:
            return self.callback(*args)

        if self.permission in auth_manager.get_player_permissions(args[1]):
            return self.callback(*args)

        if self.fail_callback is not None:
            return self.fail_callback(*args)

        return CommandReturn.CONTINUE
