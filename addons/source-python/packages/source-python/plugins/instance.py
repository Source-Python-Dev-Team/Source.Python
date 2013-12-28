# ../plugins/instance.py

# =============================================================================
# >> IMPORTS
# =============================================================================
# Python Imports
#   Importlib
from importlib import import_module

# Source.Python Imports
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
    ''''''

    def __init__(self, plugin_name, base_import):
        ''''''

        # 
        if not hasattr(self, 'logger'):

            # 
            self.logger = PluginsInstanceLogger

        # 
        self.logger.log_message(self.prefix + _plugin_strings[
            'Loading'].get_string(plugin=plugin_name))

        # 
        file_path = PLUGIN_PATH.joinpath(*tuple(
            base_import.split('.')[:~0] + [plugin_name, plugin_name + '.py']))

        # 
        if not file_path.isfile():

            # 
            self.logger.log_message(self.prefix + _plugin_strings[
                'No Module'].get_string(plugin=plugin_name))

            # 
            raise PluginFileNotFoundError

        # 
        self._plugin = import_module(
            base_import + plugin_name + '.' + plugin_name)

        # 
        self._globals = self._plugin.__dict__[plugin_name].__dict__

        # 
        if 'load' in self.globals:

            # 
            self.globals['load']()

    @property
    def globals(self):
        ''''''
        return self._globals
