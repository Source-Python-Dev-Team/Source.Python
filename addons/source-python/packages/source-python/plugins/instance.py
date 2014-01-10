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

        # Does the object have a translations value set?
        if not hasattr(self, 'translations'):

            # If not, set the default translations
            self.translations = _plugin_strings

        # Print message that the plugin is going to be loaded
        self.logger.log_message(self.prefix + self.translations[
            'Loading'].get_string(plugin=plugin_name))

        # Get the plugin's main file
        file_path = PLUGIN_PATH.joinpath(*tuple(
            base_import.split('.')[:~0] + [plugin_name, plugin_name + '.py']))

        # Does the plugin's main file exist?
        if not file_path.isfile():

            # Print a message that the plugin's main file was not found
            self.logger.log_message(self.prefix + self.translations[
                'No Module'].get_string(plugin=plugin_name,
                file=file_path.replace(GAME_PATH, '').replace('\\', '/')))

            # Raise an error so that the plugin
            # is not added to the PluginManager
            raise PluginFileNotFoundError

        # Get the base import
        import_name = base_import + plugin_name + '.' + plugin_name

        # Import the plugin
        self._plugin = __import__(import_name)

        # Set the globals value
        self._globals = self._plugin.__dict__

        # Get the import name minus the first directory
        import_name = import_name.split('.', 1)[1]

        # Use "while" statement to find the proper globals for the plugin
        while '__path__' in self._globals:

            # Try to split the import path
            try:

                # Split the path by one level
                start, import_name = import_name.split('.', 1)

            # Are we at the end of the import?
            except ValueError:

                # Set the next value to the import name
                start = import_name

            # Set the globals value to the current module
            self._globals = self._globals[start].__dict__

    @property
    def globals(self):
        '''Returns the plugin's globals'''
        return self._globals
