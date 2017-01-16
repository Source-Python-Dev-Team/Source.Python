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
# >> FORWARD IMPORTS
# =============================================================================
from _plugins import ServerPlugin


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('Plugin',
           'ServerPlugin',
           )


# =============================================================================
# >> GLOBAL VARIABLES
# =============================================================================
# Get the sp.plugins.instance logger
plugins_instance_logger = plugins_logger.instance


# =============================================================================
# >> CLASSES
# =============================================================================
class Plugin(object):
    """Stores a plugin's instance."""

    def __init__(self, plugin_name, manager):
        """Called when a plugin's instance is initialized.

        :param str plugin_name:
            Name of the plugin to load.
        :param PluginManager manager:
            A plugin manager instance.
        """
        self.module = None
        self.manager = manager
        self.name = plugin_name
        self.sub_plugin = manager.base_import != ''
        self.directory = self.manager.get_plugin_directory(plugin_name)
        self.file_path = self.manager.get_plugin_file_path(plugin_name)
        self.info = self.manager._create_plugin_info(plugin_name)
        self.import_name = (self.manager.base_import + plugin_name +
                            '.' + plugin_name)

    def unload(self):
        """Unload the plugin."""
        self.manager.unload(self.name)

    def reload(self):
        """Reload the plugin."""
        self.manager.reload(self.name)

    def _load(self):
        """Actually load the plugin."""
        self.info._create_public_convar()
        self.module = import_module(self.import_name)
        if hasattr(self.module, 'load'):
            self.module.load()

    def _unload(self):
        """Actually unload the plugin."""
        if hasattr(self.module, 'unload'):
            self.module.unload()

        self.module = None
