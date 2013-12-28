# ../plugins/manager.py

# =============================================================================
# >> IMPORTS
# =============================================================================
# Python Imports
#   Collections
from collections import OrderedDict

# Source.Python Imports
from public import public
#   Plugins
from plugins import PluginsLogger
from plugins import _plugin_strings
from plugins.errors import PluginFileNotFoundError


# =============================================================================
# >> GLOBAL VARIABLES
# =============================================================================
# Get the sp.plugins.manager logger
PluginsManagerLogger = PluginsLogger.manager


# =============================================================================
# >> CLASSES
# =============================================================================
@public
class PluginManager(OrderedDict):
    ''''''

    def __init__(self, base_import=''):
        ''''''

        # 
        self.base_import = base_import

        # 
        if not hasattr(self, 'logger'):

            # 
            self.logger = PluginsManagerLogger

    def __missing__(self, plugin_name):
        ''''''

        # 
        try:

            # 
            instance = self.instance(plugin_name, self.base_import)

        # 
        except PluginFileNotFoundError:

            # 
            return None

        # 
        except:

            # 
            error = sys.exc_info()

            # 
            if (len(error[1].args) and error[1].args[0] ==
                    "No module named '{0}.{0}'".format(plugin_name)):

                # 
                self.logger.log_message(self.prefix + _plugin_strings[
                    'Built-in'].get_string(plugin=plugin_name))

            # 
            else:

                # 
                ExceptHooks.print_exception(*error)

                # 
                self._remove_modules(plugin_name)

            # 
            return None

        # 
        self[plugin_name] = instance

        # 
        return instance
