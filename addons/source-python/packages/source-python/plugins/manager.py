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
#   Importlib
from importlib.util import find_spec
from importlib.util import spec_from_file_location
#   Sys
import sys
#   Re
import re

# Source.Python Imports
#   Core
from core import AutoUnload
from core import WeakAutoUnload
#   Hooks
from hooks.exceptions import except_hooks
#   Listeners
from listeners import on_plugin_loaded_manager
from listeners import on_plugin_unloaded_manager
from listeners import on_plugin_loading_manager
from listeners import on_plugin_unloading_manager
#   Paths
from paths import PLUGIN_PATH
#   Plugins
from plugins import plugins_logger
from plugins.info import PluginInfo
from plugins.instance import Plugin


# =============================================================================
# >> FORWARD IMPORTS
# =============================================================================
from _plugins import ServerPluginManager
from _plugins import server_plugin_manager


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('PluginManager',
           'ServerPluginManager',
           'plugin_manager',
           'server_plugin_manager'
           )


# =============================================================================
# >> GLOBAL VARIABLES
# =============================================================================
# Get the sp.plugins.manager logger
plugins_manager_logger = plugins_logger.manager


# =============================================================================
# >> CLASSES
# =============================================================================
class PluginError(ValueError):
    pass

class PluginFileNotFoundError(PluginError):
    pass

class InvalidPluginName(PluginError):
    pass

class PluginAlreadyLoaded(PluginError):
    pass

class PluginHasBuiltInName(PluginError):
    pass

class PluginNotLoaded(PluginError):
    pass


class PluginManager(OrderedDict):
    """Stores plugins and their instances."""

    RE_VALID_PLUGIN = re.compile('^([A-Za-z][A-Za-z0-9_]*[A-Za-z0-9])$')

    def __init__(self, base_import=''):
        """Called when the class instance is initialized."""
        # Re-call OrderedDict's __init__ to properly setup the object
        super().__init__()
        self._base_import = base_import

    def _create_plugin_instance(self, plugin_name):
        """Create a new plugin instance.

        Overwrite this method if you wish to use your own Plugin subclass.

        :rtype: Plugin
        """
        return Plugin(plugin_name, self)

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

    @property
    def plugins(self):
        """Return a generator to iterate over all existing plugins.

        :return:
            The generator yields the plugin names.
        :rtype: generator
        """
        for path in self.plugins_directory.dirs():
            plugin_name = path.namebase
            if not self.is_valid_plugin_name(plugin_name):
                continue

            if not self.plugin_exists(plugin_name):
                continue

            yield plugin_name
            
    @property
    def loaded_plugins(self):
        """Return a tuple containing all loaded plugins.
        
        :return:
            The tuple contains :class:`plugins.instance.Plugin` instances.
        :rtype: tuple
        """
        return tuple(self.values())

    def load(self, plugin_name):
        """Load a plugin by name.

        :param str plugin_name:
            Name of the plugin to load.
        :raise InvalidPluginName:
            Raised if the given plugin name is invalid.
        :raise PluginAlreadyLoaded:
            Raised if the plugin was already loaded.
        :raise PluginFileNotFoundError:
            Raised if the plugin's main file wasn't found.
        :raise PluginHasBuiltInName:
            Raised if the plugin has the name of a built-in module.
        :raise Exception:
            Any other exceptions raised by the plugin during the load process.
        :rtype: Plugin
        """
        if self.is_loaded(plugin_name):
            raise PluginAlreadyLoaded(
                'Plugin "{}" is already loaded.'.format(plugin_name))

        if not self.is_valid_plugin_name(plugin_name):
            raise InvalidPluginName(
                '"{}" is an invalid plugin name.'.format(plugin_name))

        plugin = self._create_plugin_instance(plugin_name)
        if not plugin.file_path.isfile():
            raise PluginFileNotFoundError(
                'File {} does not exist.'.format(plugin.file_path))

        spec = None
        if plugin_name not in sys.builtin_module_names:
            try:
                spec = find_spec(plugin.import_name)
            # AttributeError:
            #   An existing .py module that do not have a __path__ attribute.
            # ValueError:
            #   An existing .pyc/.pyd package that have its __spec__ attribute
            #   set to None.
            except (AttributeError, ValueError):
                pass

        if spec is None or spec != spec_from_file_location(
                plugin.import_name, plugin.file_path):
            raise PluginHasBuiltInName(
                'Plugin "{}" has the name of a built-in module.'.format(
                    plugin_name))

        # Add the instance here, so we can use get_plugin_instance() etc.
        # within the plugin itself before the plugin has been fully loaded.
        # This is also required e.g. for retrieving the PluginInfo instance.
        self[plugin_name] = plugin
        on_plugin_loading_manager.notify(plugin)

        try:
            # Actually load the plugin
            plugin._load()
        except:
            self.pop(plugin_name, 0)
            self._remove_modules(plugin_name)
            raise

        on_plugin_loaded_manager.notify(plugin)
        return plugin

    def unload(self, plugin_name):
        """Unload a plugin by name.

        :param str plugin_name:
            Name of the plugin to unload.
        :raise PluginNotLoaded:
            Raised if the plugin is not loaded.
        """
        if not self.is_loaded(plugin_name):
            raise PluginNotLoaded(
                'Plugin "{}" is not loaded.'.format(plugin_name))

        plugin = self[plugin_name]
        on_plugin_unloading_manager.notify(plugin)
        try:
            plugin._unload()
        except:
            except_hooks.print_exception()

        self._remove_modules(plugin_name)
        del self[plugin_name]
        on_plugin_unloaded_manager.notify(plugin)

    def reload(self, plugin_name):
        """Reload a plugin by name.

        :param str plugin_name:
            Name of the plugin to reload.
        :raise PluginNotLoaded:
            Raised if the plugin is not loaded.
        :raise InvalidPluginName:
            Raised if the given plugin name is invalid.
        :raise PluginFileNotFoundError:
            Raised if the plugin's main file wasn't found.
        :raise PluginHasBuiltInName:
            Raised if the plugin has the name of a built-in module.
        :raise Exception:
            Any other exceptions raised by the plugin during the load process.
        :rtype: Plugin
        """
        self.unload(plugin_name)
        return self.load(plugin_name)

    def is_valid_plugin_name(self, plugin_name):
        """Return whether or not the given plugin name is valid.

        :param str plugin_name:
            Name to check.
        :rtype: bool
        """
        return self.RE_VALID_PLUGIN.match(plugin_name) is not None

    def is_loaded(self, plugin_name):
        """Return whether or not a plugin is loaded.

        :param str plugin_name:
            The plugin to check.
        :rtype: bool
        """
        return plugin_name in self

    def plugin_exists(self, plugin_name):
        """Return whether of not a plugin exists.

        :param str plugin_name:
            The plugin to check.
        :rtype: bool
        """
        return self.get_plugin_file_path(plugin_name).isfile()

    def get_plugin_instance(self, plugin_name):
        """Return a plugin's instance, if it is loaded.

        :param str plugin_name:
            The plugin to check. You can pass ``__name__`` from one of your
            plugin files to retrieve its own plugin instance.
        :rtype: Plugin
        """
        plugin_name = self.get_plugin_name(plugin_name)
        if self.is_loaded(plugin_name):
            return self[plugin_name]

        return None

    def get_plugin_name(self, plugin_name):
        """Return the plugin's name.

        :param str plugin_name:
            The plugin's real name (will be passed through) or the
            ``__name__`` variable of one of the plugin's files.
        :rtype: str
        """
        if not plugin_name.startswith(self.base_import):
            return plugin_name

        return plugin_name.replace(self.base_import, '', 1).split('.', 1)[0]

    def get_plugin_directory(self, plugin_name):
        """Return the directory of the given plugin.

        :rtype: path.Path
        """
        return self.plugins_directory / plugin_name

    def get_plugin_file_path(self, plugin_name):
        """Return the path to the plugin's main file.

        :param str plugin_name:
            Name of the plugin.
        :rtype: path.Path
        """
        return self.get_plugin_directory(plugin_name) / plugin_name + '.py'

    def get_plugin_info(self, plugin_name):
        """Return information about the given plugin.

        :param str plugin_name:
            The plugin to check. You can pass ``__name__`` from one of your
            plugin files to retrieve its own plugin instance.
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
        :raise PluginFileNotFoundError:
            Raised if the plugin's main file wasn't found.
        :rtype: PluginInfo
        """
        if not self.plugin_exists(plugin_name):
            raise PluginFileNotFoundError(
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
        data = info.pop('public_convar', 'True')
        if isinstance(data, Section):
            return data.dict()

        # False and 0 result in False, everything else is True
        return data.lower() not in ('false', '0')

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

# The singleton instance of the :class:`PluginManager` class
plugin_manager = PluginManager()
