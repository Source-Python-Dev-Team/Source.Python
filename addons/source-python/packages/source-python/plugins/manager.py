# ../plugins/manager.py

"""Provides plugin loading/unloading functionality."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Python Imports
#   Collections
from collections import OrderedDict
#   Sys
import sys

# Source.Python Imports
#   Core
from core import AutoUnload
#   Hooks
from hooks.exceptions import except_hooks
#   Plugins
from plugins import plugins_logger
from plugins import _plugin_strings
from plugins.errors import PluginFileNotFoundError


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('PluginManager',
           )


# =============================================================================
# >> GLOBAL VARIABLES
# =============================================================================
# Get the sp.plugins.manager logger
plugins_manager_logger = plugins_logger.manager


# =============================================================================
# >> CLASSES
# =============================================================================
class PluginManager(OrderedDict):
    """Stores plugins and their instances."""

    def __init__(self, base_import=''):
        """Called when the class instance is initialized."""
        # Re-call OrderedDict's __init__ to properly setup the object
        super(PluginManager, self).__init__()

        # Store the base import path
        self._base_import = base_import

        # Does the object have a logger set?
        if not hasattr(self, 'logger'):

            # If not, set the default logger
            self.logger = plugins_manager_logger

        # Does the object have a translations value set?
        if not hasattr(self, 'translations'):

            # If not, set the default translations
            self.translations = _plugin_strings

    def __missing__(self, plugin_name):
        """Try to load a plugin that is not loaded."""
        # Try to get the plugin's instance
        try:

            # Get the plugin's instance
            instance = self.instance(plugin_name, self.base_import)

            # Does the plugin have a load function?
            if 'load' in instance.globals:

                # Call the plugin's load function
                instance.globals['load']()

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
                self.logger.log_message(self.prefix + self.translations[
                    'Built-in'].get_string(plugin=plugin_name))

            # Otherwise
            else:

                # Print the exception to the console
                except_hooks.print_exception(*error)

                # Remove all modules from sys.modules
                self._remove_modules(plugin_name)

            # Return None as the value to show the addon was not loaded
            return None

        # Add the plugin to the dictionary with its instance
        self[plugin_name] = instance

        # Return the give instance
        return instance

    def __delitem__(self, plugin_name):
        """Remove a plugin from the manager."""
        # Is the plugin in the dictionary?
        if plugin_name not in self:

            # Do nothing
            return

        # Print a message about the plugin being unloaded
        self.logger.log_message(self.prefix + self.translations[
            'Unloading'].get_string(plugin=plugin_name))

        # Does the plugin have an unload function?
        if 'unload' in self[plugin_name].globals:

            # Use a try/except here to still allow the plugin to be unloaded
            try:

                # Call the plugin's unload function
                self[plugin_name].globals['unload']()

            # Was an exception raised?
            except:

                # Print the error to console, but
                # allow the plugin to still be unloaded
                except_hooks.print_exception()

        # Remove all modules from sys.modules
        self._remove_modules(plugin_name)

        # Remove the plugin from the dictionary
        super(PluginManager, self).__delitem__(plugin_name)

    @property
    def base_import(self):
        """Return the base import path for the manager."""
        return self._base_import

    def is_loaded(self, plugin_name):
        """Return whether or not a plugin is loaded."""
        return plugin_name in self

    def get_plugin_instance(self, plugin_name):
        """Return a plugin's instance, if it is loaded."""
        # Is the plugin loaded?
        if plugin_name in self:

            # Return the plugin's instance
            return self[plugin_name]

        # Return None if the plugin is not loaded
        return None

    def _remove_modules(self, plugin_name):
        """Remove all modules from the plugin."""
        # Get the plugins import path
        base_name = self.base_import + plugin_name

        # Loop through all loaded modules
        for module in list(sys.modules):

            # Is the current module within the plugin?
            if module.startswith(base_name):

                # Remove the module
                self._remove_module(module)

    @staticmethod
    def _remove_module(module):
        """Remove a module and unloads any AutoUnload instances."""
        # Loop through all items in the module
        for name in dir(sys.modules[module]):

            # Get the item's object
            instance = getattr(sys.modules[module], name)

            # Is the object an AutoUnload instance
            if not isinstance(instance, AutoUnload):

                # No need to do anything with this object
                continue

            # Is the instance native to the given module?
            if instance._calling_module == module:

                # Use try/except in-case the instance
                # does not have an _unload_instance method
                try:

                    # Unload the object
                    instance._unload_instance()

                # Was a NotImplementedError encountered?
                except NotImplementedError:

                    # Print the error to console, but allow all
                    # other AutoUnload instances to be unloaded
                    # and the plugin to be fully unloaded itself
                    except_hooks.print_exception()

        # Delete the module
        del sys.modules[module]
