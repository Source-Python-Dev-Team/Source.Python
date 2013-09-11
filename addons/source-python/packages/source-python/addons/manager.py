# ../addons/manager.py

# =============================================================================
# >> IMPORTS
# =============================================================================
# Python Imports
#   Collections
from collections import OrderedDict
#   Sys
import sys

# Source.Python Imports
from core import AutoUnload
from excepthooks import ExceptHooks
from paths import PLUGIN_PATH
from public import public
#   Addons
from addons import AddonLogger
from addons.errors import AddonFileNotFoundError
#   Translations
from translations.strings import LangStrings


# =============================================================================
# >> GLOBAL VARIABLES
# =============================================================================
# Get the addons language strings
_addon_strings = LangStrings('_core/addons_strings')

# Get the sp.addons.manager logger
AddonManagerLogger = AddonLogger.manager


# =============================================================================
# >> MAIN CLASSES
# =============================================================================
@public
class _AddonManager(OrderedDict):
    '''Stores addon's and their instances'''

    def __missing__(self, addon_name):
        '''Tries to load an addon that is not loaded'''

        # Try to get the addon's instance
        try:

            # Get the addon's instance
            instance = _LoadedAddon(addon_name)

        # Was the file not found?
        # We use this check because we already printed the error to console
        except AddonFileNotFoundError:

            # Return None as the value to show the addon was not loaded
            return None

        # Was a different error was encountered?
        except:

            # Get the error
            error = sys.exc_info()

            # Is the error due to "No module named '<addon>.<addon>'?
            if (len(error[1].args) and error[1].args[0] ==
                    "No module named '{0}.{0}'".format(addon_name)):

                # Print a message about not using built-in module names
                # We already know the path exists, so the only way this error
                # could occur is if it shares its name with a built-in module
                AddonManagerLogger.log_message('[SP] ' + _addon_strings[
                    'Built-in'].get_string(addon=addon_name))

            # Otherwise
            else:

                # Print the exception to the console
                ExceptHooks.print_exception(*error)

                # Remove all modules from sys.modules
                self._remove_modules(addon_name)

            # Return None as the value to show the addon was not loaded
            return None

        # Add the addon to the dictionary with its instance
        self[addon_name] = instance

        # Return the given value
        return instance

    def __delitem__(self, addon_name):
        '''Removes an addon from the manager'''

        # Is the addon in the dictionary?
        if not addon_name in self:

            # Is the addon loaded?
            return

        # Print message about unloading the addon
        AddonManagerLogger.log_message('[SP] ' + _addon_strings[
            'Unloading'].get_string(addon=addon_name))

        # Does the addon have an unload function?
        if 'unload' in self[addon_name]._globals:

            # Use a try/except here to still allow the addon to be unloaded
            try:

                # Call the addon's unload function
                self[addon_name]._globals['unload']()

            # Was an exception raised?
            except:

                # Print the error to console, but
                # allow the addon to still be unloaded
                ExceptHooks.print_exception()

        # Remove all modules from sys.modules
        self._remove_modules(addon_name)

        # Remove the addon from the dictionary
        super(_AddonManager, self).__delitem__(addon_name)

    def get_addon_instance(self, addon_name):
        '''Returns an addon's instance if it is loaded'''

        # Is the addon loaded?
        if addon_name in self:

            # Return the addon's instance
            return self[addon_name]

        # Return None if the addon is not loaded
        return None

    def is_loaded(self, addon_name):
        '''Returns whether an addon is loaded or not'''
        return addon_name in self

    def _remove_modules(self, addon_name):
        '''Recursively removes modules from within the unloading addon'''

        # Get the addon's module
        addon_import = addon_name + '.' + addon_name

        # Is the addon's module loaded?
        if not addon_import in sys.modules:

            # If not, simply return
            return

        # Is the addon in the dictionary?
        if addon_name in self:

            # Get the addon's instance
            instance = self[addon_name]._addon

        # Is the addon not in the dictionary?
        else:

            # Get the addon's instance
            instance = __import__(addon_name + '.' + addon_name)

        # Remove all instances of AutoUnload classes from the addon
        self._unload_instances(instance, addon_name)

        # Loop through all loaded modules
        for module in list(sys.modules):

            # Is the current module part of the given addon?
            if module.split('.')[0] == addon_name:

                # Remove the module from memory
                del sys.modules[module]

    def _unload_instances(self, instance, module):
        '''Removes all AutoUnload instances from the registry for the addon'''

        # Does the current object have a __dict__?
        if not hasattr(instance, '__dict__'):

            # If not, simply return
            return

        # Loop through all items in the instance's dictionary
        for item in dict(instance.__dict__):

            # Get the new object's instance
            new_instance = instance.__dict__[item]

            # Get the object's module
            new_module = module + '.' + item

            # Is the item an "AutoUnload" instance?
            if isinstance(new_instance, AutoUnload):

                # Unregister the instance
                new_instance._unload_instance()

            # Does the module exist in sys.modules?
            elif new_module in sys.modules:

                # Loop through all items in the module
                self._unload_instances(new_instance, new_module)

# Get the _AddonManager instance
AddonManager = _AddonManager()


class _LoadedAddon(object):
    '''Stores an addon's instance'''

    def __init__(self, addon_name):
        '''Called when an addon's instance is initialized'''

        # Print message that the addon is going to be loaded
        AddonManagerLogger.log_message('[SP] ' + _addon_strings[
            'Loading'].get_string(addon=addon_name))

        # Get the addon's main file
        file_path = PLUGIN_PATH.joinpath(addon_name, addon_name + '.py')

        # Does the addon's main file exist?
        if not file_path.isfile():

            # Print a message that the addon's main file was not found
            AddonManagerLogger.log_message('[SP] ' + _addon_strings[
                'No Module'].get_string(addon=addon_name))

            # Raise an error, so that the addon
            # is not added to the AddonManager
            raise AddonFileNotFoundError

        # Import the addon
        self._addon = __import__(addon_name + '.' + addon_name)

        # Store the globals for the addon
        self._globals = self._addon.__dict__[addon_name].__dict__

        # Does the addon have a load function?
        if 'load' in self._globals:

            # Call the addon's load function
            self._globals['load']()
