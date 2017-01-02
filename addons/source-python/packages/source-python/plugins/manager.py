# ../plugins/manager.py

"""Provides plugin loading/unloading functionality."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Python Imports
#   Collections
from collections import OrderedDict
#   Configobj
from configobj import ConfigObj
from configobj import Section
#   Sys
import sys

# Source.Python Imports
#   Core
from core import AutoUnload
from core import WeakAutoUnload
#   Hooks
from hooks.exceptions import except_hooks
#   Listeners
from listeners import on_plugin_loaded_manager
from listeners import on_plugin_unloaded_manager
#   Paths
from paths import PLUGIN_PATH
#   Plugins
from plugins import plugins_logger
from plugins import _plugin_strings
from plugins.errors import PluginFileNotFoundError
from plugins.info import PluginInfo


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

    instance = None
    prefix = None

    def __init__(self, base_import=''):
        """Called when the class instance is initialized."""
        # Re-call OrderedDict's __init__ to properly setup the object
        super().__init__()
        self.logger = None
        self.translations = None
        self._base_import = base_import

        # Does the object have a logger set?
        if self.logger is None:
            self.logger = plugins_manager_logger

        # Does the object have a translations value set?
        if self.translations is None:
            self.translations = _plugin_strings

    def __missing__(self, plugin_name):
        """Try to load a plugin that is not loaded."""
        # Try to get the plugin's instance
        try:

            # Get the plugin's instance
            instance = self.instance(plugin_name, self)

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
        on_plugin_loaded_manager.notify(plugin_name)
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
        super().__delitem__(plugin_name)
        on_plugin_unloaded_manager.notify(plugin_name)

    @property
    def base_import(self):
        """Return the base import path for the manager.

        :rtype: str
        """
        return self._base_import

    @property
    def plugins_directory(self):
        """Return the directory where the plugins are stored.

        :rtype: path.Path
        """
        return PLUGIN_PATH.joinpath(*tuple(self.base_import.split('.')[:~0]))

    def is_loaded(self, plugin_name):
        """Return whether or not a plugin is loaded.

        :rtype: bool
        """
        return plugin_name in self

    def plugin_exists(self, plugin_name):
        """Return whether of not a plugin exists.

        :rtype: bool
        """
        return self.get_plugin_directory(plugin_name).isdir()

    def get_plugin_instance(self, plugin_name):
        """Return a plugin's instance, if it is loaded.

        :rtype: LoadedPlugin
        """
        if plugin_name in self:
            return self[plugin_name]

        return None

    def get_plugin_directory(self, plugin_name):
        """Return the directory of the given plugin.

        :rtype: path.Path
        """
        return self.plugins_directory / plugin_name

    def get_plugin_info(self, plugin_name):
        """Return information about the given plugin.

        :rtype: PluginInfo
        """
        plugin = self.get_plugin_instance(plugin_name)
        if plugin is not None:
            return plugin.info

        return self._create_plugin_info(plugin_name)

    def _create_plugin_info(self, plugin_name):
        """Create a new :class:`plugins.info.PluginInfo` instance.

        :param str plugin_name:
            Name of the plugin whose plugin info should be created.
        :rtype: PluginInfo
        """
        if not self.plugin_exists(plugin_name):
            raise ValueError(
                'Plugin "{}" does not exist.'.format(plugin_name))

        info_file = self.get_plugin_directory(plugin_name) / 'info.ini'
        if not info_file.isfile():
            # Just return an "empty" PluginInfo instance. We don't have more
            # information.
            return PluginInfo(plugin_name)

        info = ConfigObj(info_file)
        return PluginInfo(
            plugin_name,
            info.pop('verbose_name', None),
            info.pop('author', None),
            info.pop('description', None),
            info.pop('version', None),
            info.pop('url', None),
            tuple(info.pop('permissions', dict()).items()),
            self._get_public_convar_from_info_file(info),
            self._get_display_in_listing_from_info_file(info),
            **info.dict()
        )

    @staticmethod
    def _get_public_convar_from_info_file(info):
        data = info.pop('public_convar', True)
        if isinstance(data, Section):
            return data.dict()

        return data

    @staticmethod
    def _get_display_in_listing_from_info_file(info):
        data = info.pop('display_in_listing', [])
        if isinstance(data, (tuple, list)):
            return list(data)

        return [data]

    def _remove_modules(self, plugin_name):
        """Remove all modules from the plugin."""
        # Get the plugins import path
        base_name = self.base_import + plugin_name

        # Remove modules from sys.modules
        for module in list(sys.modules):
            if self._is_related_module(base_name, module):
                del sys.modules[module]

        # Unload AutoUnload instances
        for module, instances in list(AutoUnload._module_instances.items()):
            if not self._is_related_module(base_name, module):
                continue

            self._unload_auto_unload_instances(instances)
            del AutoUnload._module_instances[module]

        # Unload WeakAutoUnload instances
        for module, instance_dict in list(
                WeakAutoUnload._module_instances.items()):
            if not self._is_related_module(base_name, module):
                continue

            self._unload_auto_unload_instances(instance_dict.values())
            del WeakAutoUnload._module_instances[module]

    @staticmethod
    def _is_related_module(base_name, module):
        """Check if a plugin's base name is related to a module name.

        :rtype: bool
        """
        return (module.startswith('{}.'.format(base_name))
            or module == base_name)

    @staticmethod
    def _unload_auto_unload_instances(instances):
        """Unload all given :class:`AutoUnload` instances."""
        for instance in instances:
            try:
                instance._unload_instance()
            except:
                # Print the error to console, but allow all
                # other AutoUnload instances to be unloaded
                # and the plugin to be fully unloaded itself
                except_hooks.print_exception()
