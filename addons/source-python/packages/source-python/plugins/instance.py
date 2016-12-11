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

    def __init__(self, plugin_name, base_import):
        """Called when a plugin's instance is initialized."""
        self.file_path = None
        self.import_name = None
        self.globals = None
        self.base_import = base_import
        self.plugin_name = plugin_name

        # Fall back to the default logger if none was set
        if self.logger is None:
            self.logger = plugins_instance_logger

        # Fall back to the default translations if none was set
        if self.translations is None:
            self.translations = _plugin_strings

        # Print message that the plugin is going to be loaded
        self.logger.log_message(self.prefix + self.translations[
            'Loading'].get_string(plugin=plugin_name))

        # Get the plugin's main file
        self.file_path = PLUGIN_PATH.joinpath(*tuple(
            base_import.split('.')[:~0] + [plugin_name, plugin_name + '.py']))

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

        # Get the base import
        self.import_name = base_import + plugin_name + '.' + plugin_name

        # Import the plugin
        self._plugin = import_module(self.import_name)

        # Set the globals value
        self.globals = {
            x: getattr(self._plugin, x) for x in dir(self._plugin)}

        # Add this instance to the plugin info for easier access
        info = self.info
        if info is not None:
            info.plugin = self
            info._create_public_convar()

    @property
    def info(self):
        """Return the plugin's PluginInfo instance.

        If no PluginInfo was found, None will be returned.

        :rtype: PluginInfo
        """
        for obj in self.globals.values():
            if isinstance(obj, PluginInfo):
                return obj

        return None
