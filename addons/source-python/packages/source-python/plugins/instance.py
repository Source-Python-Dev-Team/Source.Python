# ../plugins/instance.py

"""Provides a base class used to store a loaded plugin."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Python Imports
#   Importlib
from importlib import import_module

# Source.Python Imports
#   Plugins
from plugins import plugins_logger
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

    def __init__(self, plugin_name, manager):
        """Called when a plugin's instance is initialized.

        :param str plugin_name:
            Name of the plugin to load.
        :param PluginManager manager:
            A plugin manager instance.
        """
        self.manager = manager
        self.globals = None
        self.name = plugin_name
        self.directory = self.manager.get_plugin_directory(plugin_name)
        self.file_path = self.manager.get_plugin_file_path(plugin_name)
        self.info = self.manager._create_plugin_info(plugin_name)
        self._plugin = None
        self.import_name = (self.manager.base_import + plugin_name +
                            '.' + plugin_name)

    def unload(self):
        """Unload the plugin."""
        # Convenience method
        self.manager.unload(self.name)

    def reload(self):
        """Reload the plugin."""
        # Convenience method
        self.manager.reload(self.name)

    def _load(self):
        """Actually load the plugin."""
        self.info._create_public_convar()
        self._plugin = import_module(self.import_name)
        self.globals = {
            x: getattr(self._plugin, x) for x in dir(self._plugin)}

        if 'load' in self.globals:
            self.globals['load']()

    def _unload(self):
        """Actually unload the plugin."""
        if 'unload' in self.globals:
            self.globals['unload']()
