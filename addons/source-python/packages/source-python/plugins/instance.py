# ../plugins/instance.py

"""Provides a base class used to store a loaded plugin."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Python Imports
#   Importlib
from importlib import import_module

# Source.Python Imports
#   Paths
from paths import GAME_PATH
from paths import PLUGIN_PATH
#   Plugins
from plugins import plugins_logger
from plugins import _plugin_strings
from plugins.errors import PluginFileNotFoundError
from plugins.info import PluginInfo


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('LoadedPlugin',
           )


# =============================================================================
# >> GLOBAL VARIABLES
# =============================================================================
# Get the sp.plugins.instance logger
plugins_instance_logger = plugins_logger.instance


# =============================================================================
# >> CLASSES
# =============================================================================
class LoadedPlugin(object):
    """Stores a plugin's instance."""

    logger = None
    translations = None
    prefix = None

    def __init__(self, plugin_name, manager):
        """Called when a plugin's instance is initialized.

        :param str plugin_name:
            Name of the plugin to load.
        :param PluginManager manager:
            A plugin manager instance.
        """
        self.manager = manager
        self.file_path = None
        self.import_name = None
        self.globals = None
        self.plugin_name = plugin_name
        self.directory = self.manager.get_plugin_directory(plugin_name)
        self.file_path = self.directory / plugin_name + '.py'
        self.info = self.manager._create_plugin_info(self.plugin_name)
        self.info._create_public_convar()
        self._plugin = None

        # Fall back to the default logger if none was set
        if self.logger is None:
            self.logger = plugins_instance_logger

        # Fall back to the default translations if none was set
        if self.translations is None:
            self.translations = _plugin_strings

        # Print message that the plugin is going to be loaded
        self.logger.log_message(self.prefix + self.translations[
            'Loading'].get_string(plugin=plugin_name))

        # Does the plugin's main file exist?
        if not self.file_path.isfile():

            # Print a message that the plugin's main file was not found
            self.logger.log_message(self.prefix + self.translations[
                'No Module'].get_string(
                plugin=plugin_name, file=self.file_path.replace(
                    GAME_PATH, '').replace('\\', '/')))

            # Raise an error so that the plugin
            # is not added to the PluginManager
            raise PluginFileNotFoundError

        # Get the import name
        self.import_name = (self.manager.base_import + plugin_name +
                            '.' + plugin_name)

    def _load(self):
        """Actually load the plugin."""
        self._plugin = import_module(self.import_name)
        self.globals = {
            x: getattr(self._plugin, x) for x in dir(self._plugin)}

        if 'load' in self.globals:
            self.globals['load']()

    def _unload(self):
        """Actually unload the plugin."""
        if 'unload' in self.globals:
            # Use a try/except here to still allow the plugin to be unloaded
            try:
                self.globals['unload']()
            except:
                except_hooks.print_exception()
