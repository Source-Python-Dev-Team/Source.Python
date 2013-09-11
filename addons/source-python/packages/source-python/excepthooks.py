# ../excepthooks.py

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
from loggers import _SPLogger
from paths import GAME_PATH
from public import public
#   Translations
from translations.strings import LangStrings


# =============================================================================
# >> GLOBAL VARIABLES
# =============================================================================
_excepthook_strings = LangStrings('_core/excepthooks_strings')

# Get the sp.excepthooks logger
ExceptHooksLogger = _SPLogger.excepthooks


# =============================================================================
# >> CLASSES
# =============================================================================
@public
class _ExceptHooks(list):
    '''List class that stores callbacks to be called on an exception'''

    def append(self, callback):
        '''Hook the append method to verify the given callback is callable'''

        # Is the given callback callable?
        if not callable(callback):

            # Raise an exception
            raise ValueError('ExceptHook callbacks must be callable')

        # Is the given callback already registered?
        if callback in self:

            # Raise an exception
            raise ValueError('ExceptHook callback already registered')

        # Add the callback to the list
        super(_ExceptHooks, self).append(callback)

    def print_exception(
            self, exctype=None, value=None,
            trace_back=None, callbacks=True):
        '''Called when an exception is raised'''

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
        message = '\n[SP] ' + _excepthook_strings['Exception'].get_string()

        # Loop through each line in the exception
        for line in format_error:

            # Remove any lines pertaining to importlib in the exception
            if 'importlib' in line:
                continue

            # Strip the ending \n from the exception
            line = line.rstrip()

            # Strip the GAME_PATH to make the exception shorter
            line = line.replace(
                GAME_PATH, '..{0}'.format(sep)).replace(sep + '.' + sep, sep)

            # Add the current line to the message
            message += '\n' + line

        # Print a blank line to separate the console
        ExceptHooksLogger.log_exception(message + '\n\n')

# Get the _ExceptHooks instance
ExceptHooks = _ExceptHooks()

# Set sys.excepthook to the print_exception method
sys.excepthook = ExceptHooks.print_exception
