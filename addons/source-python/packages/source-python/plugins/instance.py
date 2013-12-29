# ../plugins/instance.py

# =============================================================================
# >> IMPORTS
# =============================================================================
# Python Imports
#   Importlib
from importlib import import_module

# Source.Python Imports
from paths import GAME_PATH
from paths import PLUGIN_PATH
from public import public
#   Plugins
from plugins import PluginsLogger
from plugins import _plugin_strings
from plugins.errors import PluginFileNotFoundError


# =============================================================================
# >> GLOBAL VARIABLES
# =============================================================================
# Get the sp.plugins.instance logger
PluginsInstanceLogger = PluginsLogger.instance


# =============================================================================
# >> CLASSES
# =============================================================================
@public
class LoadedPlugin(object):
    '''Stores a plugin's instance'''

    def __init__(self, plugin_name, base_import):
        '''Called when a plugin's instance is initialized'''

        # Does the object have a logger set?
        if not hasattr(self, 'logger'):

            # If not, set the default logger
            self.logger = PluginsInstanceLogger

        # Print message that the plugin is going to be loaded
        self.logger.log_message(self.prefix + _plugin_strings[
            'Loading'].get_string(plugin=plugin_name))

        # Get the plugin's main file
        file_path = PLUGIN_PATH.joinpath(*tuple(
            base_import.split('.')[:~0] + [plugin_name, plugin_name + '.py']))

        # Does the plugin's main file exist?
        if not file_path.isfile():

            # Print a message that the plugin's main file was not found
            self.logger.log_message(self.prefix + _plugin_strings[
                'No Module'].get_string(plugin=plugin_name,
                file=file_path.replace(GAME_PATH, '').replace('\\', '/')))

            # Raise an error so that the plugin
            # is not added to the PluginManager
            raise PluginFileNotFoundError

        # Import the plugin
        self._plugin = __import__(
            base_import + plugin_name + '.' + plugin_name)

        # Store the globals for the plugin
        self._globals = self._plugin.__dict__[plugin_name].__dict__

        # Does the plugin have a load function?
        if 'load' in self.globals:

            # Call the plugin's load function
            self.globals['load']()

    @property
    def globals(self):
        '''Returns the plugin's globals'''
        return self._globals
