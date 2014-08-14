# ../commands/auth.py

"""Provides Authorization checks for client and say commands."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
#   Auth
from auth.manager import auth_manager
#   Hooks
from hooks.exceptions import except_hooks


# =============================================================================
# >> CLASSES
# =============================================================================
class _AuthCallback(object):

    """Command Authorization callback hook class."""

    def __init__(
            self, callback, level=None,
            permission=None, flag=None, fail_callback=None):
        """Store all the given arguments."""
        self.callback = callback
        self.level = level
        self.permission = permission
        self.flag = flag
        self.check_auth = not all(
            [item is None for item in [level, permission, flag]])
        self.fail_callback = fail_callback

    def __call__(self, *args):
        """Verify the player's authorization."""
        # Does the player's authorization need to be checked?
        if self.check_auth:

            # Is the player authorized?
            if not auth_manager.is_player_authorized(
                    args[0], self.level, self.permission, self.flag):

                # Is there fail callback?
                if self.fail_callback is not None:

                    # Use try/except in case the fail
                    # callback encounters an error
                    try:

                        # Call the fail callback
                        self.fail_callback(*args)

                    # Was an error encountered?
                    except:

                        # Print the exception to the console
                        except_hooks.print_exception()

                # Return a False value, since the player is not authorized
                return False

        # Call the callback and return its value
        return self.callback(*args)
