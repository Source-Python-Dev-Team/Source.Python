# ../hooks/exceptions.py

"""Provides exception hooking functionality."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Python Imports
#   OS
from os import sep
#   Sys
import sys
#   Traceback
from traceback import format_exception

# Source.Python Imports
#   Hooks
from hooks import _hooks_strings
from hooks import hooks_logger
from hooks.base import _HookBase
from hooks.base import _HookDecorator
#   Paths
from paths import GAME_PATH


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('ExceptHook',
           '_ExceptHooks',
           'except_hooks',
           )


# =============================================================================
# >> GLOBAL VARIABLES
# =============================================================================
# Get the sp.hooks.exceptions logger
hooks_exceptions_logger = hooks_logger.exceptions


# =============================================================================
# >> CLASSES
# =============================================================================
class _ExceptHooks(_HookBase):
    """List class that stores callbacks to be called on an exception."""

    def print_exception(
            self, exctype=None, value=None,
            trace_back=None, callbacks=True):
        """Called when an exception is raised.

        This method hooks :obj:`sys.excepthook` to log all exceptions
        and call registered callbacks with the provided arguments.
        """
        # Was an exception passed?
        if exctype is None:

            # Get the exception
            exctype, value, trace_back = sys.exc_info()

        # Do all of the callbacks need looped through?
        if callbacks:

            # Loop through all callbacks in the list
            for callback in self:

                # Try to call the callback
                # Without this try/except, if a callback encounters
                # an error, it could cause an infinite loop.
                try:

                    # Call the callback with the exception arguments
                    callback(exctype, value, trace_back)

                # Was an exception raised?
                except:

                    # Re-call print_exception with the new error.
                    # Pass False for callbacks, so that
                    # it does not cause an infinite loop.
                    self.print_exception(callbacks=False)

        # Format the exception
        format_error = format_exception(exctype, value, trace_back)

        # Add a blank line before the actual exception
        format_error.insert(-1, '')

        # Get the header
        message = '\n[SP] ' + _hooks_strings['Exception'].get_string()

        # Loop through each line in the exception
        for line in format_error:

            # Remove any lines pertaining to importlib in the exception
            if 'importlib' in line:
                continue

            # Strip the ending \n from the exception
            line = line.rstrip()

            # Strip the GAME_PATH to make the exception shorter
            line = line.replace(GAME_PATH, '..').replace(sep + '.' + sep, sep)

            # Add the current line to the message
            message += '\n' + line

        # Print a blank line to separate the console
        hooks_exceptions_logger.log_exception(message + '\n\n')

# The singleton object of the :class:`_ExceptHooks` class
except_hooks = _ExceptHooks()

# Set sys.excepthook to the print_exception method
sys.excepthook = except_hooks.print_exception


class ExceptHook(_HookDecorator):
    """Register/unregister a function/method for hooking exceptions."""

    _class_instance = except_hooks
