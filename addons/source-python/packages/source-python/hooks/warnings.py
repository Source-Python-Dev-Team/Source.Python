# ../hooks/warnings.py

"""Provides warning hooking functionality."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Python Imports
#   Linecache
import linecache
#   Warnings
import warnings

# Source.Python Imports
#   Hooks
from hooks import _hooks_strings
from hooks import hooks_logger
from hooks.base import _HookBase
from hooks.base import _HookDecorator
from hooks.exceptions import except_hooks
#   Paths
from paths import GAME_PATH


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('WarningHook',
           'warning_hooks',
           )


# =============================================================================
# >> GLOBAL VARIABLES
# =============================================================================
# Get the sp.hooks.warnings logger
hooks_warnings_logger = hooks_logger.warnings


# =============================================================================
# >> CLASSES
# =============================================================================
class _WarningHooks(_HookBase):

    """Class used to log warnings."""

    def print_warning(
            self, message, category, filename, lineno, file=None, line=None):
        """Log a warning."""
        # Loop through each callback in the warning list
        for callback in self:

            # Try to call the callback
            try:

                # Call the callback
                callback(message, category, filename, lineno, file, line)

            # Was an error encountered?
            except:

                # Log the exception
                except_hooks.print_exception()

        # Get the message to log
        print_message = '\n[SP] ' + _hooks_strings['Warning'].get_string()

        # Add the warning to the message
        print_message += "\n  File '..{0}', line {1}: {2}\n    {3}".format(
            filename.replace(GAME_PATH, ''), lineno,
            category.__name__, linecache.getline(filename, lineno))

        # Log the warning
        hooks_warnings_logger.log_warning(print_message + '\n')

# Get the _WarningHooks instance
warning_hooks = _WarningHooks()

# Set warnings.showwarning to the print_warning method
warnings.showwarning = warning_hooks.print_warning

# Set warnings to only appear once (the first occurrence)
warnings.filterwarnings('once')


class WarningHook(_HookDecorator):

    """Register/unregister a function/method for hooking warnings."""

    _class_instance = warning_hooks
