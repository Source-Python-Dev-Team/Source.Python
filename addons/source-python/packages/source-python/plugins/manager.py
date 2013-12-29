# ../plugins/manager.py

# =============================================================================
# >> IMPORTS
# =============================================================================
# Python Imports
#   Collections
from collections import OrderedDict
#   Sys
import sys

# Source.Python Imports
from excepthooks import ExceptHooks
from public import public
#   Plugins
from plugins import PluginsLogger
from plugins import _plugin_strings
from plugins.errors import PluginFileNotFoundError


# =============================================================================
# >> GLOBAL VARIABLES
# =============================================================================
# Get the sp.plugins.manager logger
PluginsManagerLogger = PluginsLogger.manager


# =============================================================================
# >> CLASSES
# =============================================================================
@public
class PluginManager(OrderedDict):
    '''Stores plugins and their instances'''

    def __init__(self, base_import=''):
        '''Called when the class instance is initialized'''

        # Re-call OrderedDict's __init__ to properly setup the object
        super(PluginManager, self).__init__()

        # Store the base import path
        self.base_import = base_import

        # Does the object have a logger set?
        if not hasattr(self, 'logger'):

            # If not, set the default logger
            self.logger = PluginsManagerLogger

    def __missing__(self, plugin_name):
        '''Tries to load a plugin that is not loaded'''

        # Try to get the plugin's instance
        try:

            # Get the plugin's instance
            instance = self.instance(plugin_name, self.base_import)

        # Was the file not found?
        # We use this check because we already printed the error to console
        except PluginFileNotFoundError:

            # Return None as the value to show the plugin was not loaded
            return None

        # Was a different error encountered?
        except:

            # Get the error
            error = sys.exc_info()

            # Is the error due to "No module named '<plugin>.<plugin>'"?
            if (len(error[1].args) and error[1].args[0] ==
                    "No module named '{0}.{0}'".format(plugin_name)):

                # Print a message about not using built-in module names
                # We already know the path exists, so the only way this error
                # could occur is if it shares its name with a built-in module
                self.logger.log_message(self.prefix + _plugin_strings[
                    'Built-in'].get_string(plugin=plugin_name))

            # Otherwise
            else:

                # Print the exception to the console
                ExceptHooks.print_exception(*error)

                # Remove all modules from sys.modules
                self._remove_modules(plugin_name)

            # Return None as the value to show the addon was not loaded
            return None

        # Add the plugin to the dictionary with its instance
        self[plugin_name] = instance

        # Return the give instance
        return instance
