# ../_core/commands.py

# =============================================================================
# >> IMPORTS
# =============================================================================
# Site Package Imports
#   Configobj
from configobj import ConfigObj

# Source.Python Imports
from _core import _CoreLogger
from core import GameEngine
from paths import SP_DATA_PATH
#   Plugins
from plugins import _plugin_strings
from plugins.command import SubCommandManager
from plugins.info import PluginInfo
from plugins.instance import LoadedPlugin
from plugins.manager import PluginManager
#   Auth
from auth.commands import _AuthCommandsInstance
#   Tick
from tick.delays import TickDelays
#   Translations
from translations.strings import LangStrings


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
# Set all to an empty list
__all__ = []


# =============================================================================
# >> GLOBAL VARIABLES
# =============================================================================
# Get the sp._core.command logger
_CoreCommandLogger = _CoreLogger.command


# =============================================================================
# >> CLASSES
# =============================================================================
class SPLoadedPlugin(LoadedPlugin):
    '''Plugin instance class used to create "sp" loaded plugins'''
    logger = _CoreCommandLogger


class _SPPluginManager(PluginManager):
    '''Plugin Manager class used to load "sp" plugins'''
    logger = _CoreCommandLogger

# Get the _SPPluginManager instance
SPPluginManager = _SPPluginManager()


class _SPSubCommandManager(SubCommandManager):
    '''Class used for executing "sp" sub-command functionality'''

    manager = SPPluginManager
    instance = SPLoadedPlugin
    logger = _CoreCommandLogger

    def print_plugins(self):
        '''Lists all currently loaded plugins.'''

        # Get header messages
        message = self.prefix + _plugin_strings[
            'Plugins'].get_string() + '\n' + '=' * 61 + '\n\n'

        # Loop through all loaded plugins
        for plugin in self.manager:

            # Set info to None before retrieving it
            info = None

            # Loop through all global objects in the plugin
            for object_name in self.manager[plugin].globals:

                # Get the current object's instance
                instance = self.manager[plugin].globals[object_name]

                # Is the current object an PluginInfo instance?
                if isinstance(instance, PluginInfo):

                    # Set info to the current instance
                    info = instance

                    # No need to continue the loop
                    break

            # Was an PluginInfo instance found?
            if not info is None:

                # Add message with the current plugin's name
                message += plugin + ':\n'

                # Loop through all items in the PluginInfo instance
                for item in info:

                    # Add message for the current item and its value
                    message += '\t{0}:\n\t\t{1}\n'.format(item, info[item])

            # Was no PluginInfo instance found?
            else:

                # Add message with the current plugin's name
                message += plugin + '\n'

            # Add 1 blank line between each plugin
            message += '\n' + '=' * 61

        # Print the message
        self.logger.log_message(message)

    @staticmethod
    def delay_execution(*args):
        '''Executes a command after the given delay.'''

        # Add the delay
        TickDelays.delay(
            float(args[0]),
            GameEngine.server_command, ' '.join(args[1:]) + '\n')

    @staticmethod
    def print_version():
        '''Displays Source.Python version information.'''

    @staticmethod
    def print_credits():
        '''Lists all credits for Source.Python.'''

        # Get header messages
        message = self.prefix + _plugin_strings[
            'Credits'].get_string() + '\n' + '=' * 61 + '\n\n'

        # Get the credits information
        groups = ConfigObj(
            SP_DATA_PATH.joinpath('credits.ini'), encoding='unicode_escape')

        # Loop through all groups in the credits
        for group in groups:

            # Add the current group's name
            message += '\t' + group + ':\n'

            # Loop through all names in the current group
            for name in groups[group]:

                # Add the current name
                message += '\t\t' + name + ' ' * (
                    20 - len(name)) + groups[group][name] + '\n'

            # Add 1 blank line between groups
            message += '\n'

        # Print the message
        _CoreCommandsLogger.log_message(message + '=' * 61 + '\n\n')

SPSubCommandManager = _SPSubCommandManager('sp', 'Source.Python base command.')

# Register the load/unload sub-commands
SPSubCommandManager['load'] = SPSubCommandManager.load_plugin
SPSubCommandManager['unload'] = SPSubCommandManager.unload_plugin
SPSubCommandManager['reload'] = SPSubCommandManager.reload_plugin

# Register the 'auth' sub-command
SPSubCommandManager['auth'] = _AuthCommandsInstance

# Register the 'delay' sub-command
SPSubCommandManager['delay'] = SPSubCommandManager.delay_execution
SPSubCommandManager['delay'].args = ['<delay>', '<command>', '[arguments]']

# Register all printing sub-commands
SPSubCommandManager['list'] = SPSubCommandManager.print_plugins
SPSubCommandManager['version'] = SPSubCommandManager.print_version
SPSubCommandManager['credits'] = SPSubCommandManager.print_credits
SPSubCommandManager['help'] = SPSubCommandManager.print_help
