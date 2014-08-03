# ../_core/command.py

"""Registers the "sp" command and all of its sub-commands."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Site-Package Imports
#   Configobj
from configobj import ConfigObj

# Source.Python Imports
#   Auth
from auth.commands import _AuthCommandsInstance
#   Core
from core import CoreLogger
from core.manager import SPPluginManager
#   Dump
import _dump
#   Engines
from engines.server import EngineServer
#   Paths
from paths import SP_DATA_PATH
#   Plugins
from plugins import _plugin_strings
from plugins.command import SubCommandManager
from plugins.info import PluginInfo
from plugins.instance import LoadedPlugin
#   Tick
from listeners.tick import TickDelays


# =============================================================================
# >> GLOBAL VARIABLES
# =============================================================================
# Get the sp.core.command logger
CoreCommandLogger = CoreLogger.command


# =============================================================================
# >> CLASSES
# =============================================================================
class _SPLoadedPlugin(LoadedPlugin):

    """Plugin instance class used to create "sp" loaded plugins."""

    logger = CoreCommandLogger


class _CoreCommand(SubCommandManager):

    """Class used for executing "sp" sub-command functionality."""

    manager = SPPluginManager
    instance = _SPLoadedPlugin
    logger = CoreCommandLogger

    def print_plugins(self):
        """List all currently loaded plugins."""
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
            if info is not None:

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
        """Execute a command after the given delay."""
        TickDelays.delay(
            float(args[0]),
            EngineServer.server_command, ' '.join(args[1:]) + '\n')

    def dump_data(self, dump_type):
        """Dump data to logs."""
        # Does the given dump type exist as a function?
        if not hasattr(_dump, 'dump_' + dump_type):

            # If not, print message to notify of unknown dump type
            self.logger.log_message(
                'Invalid dump_type "{0}". The valid types are:'.format(
                    dump_type))

            # Loop though all dump functions
            for dump in dir(_dump):

                # Is the current dump not valid?
                if not dump.startswith('dump_'):

                    # No need to go further
                    continue

                # Print the current dump function
                self.logger.log_message('\t{0}'.format(dump.lstrip('dump_')))

            # No need to go further
            return

        # Call the function
        getattr(_dump, 'dump_' + dump_type)()

    # Set the methods arguments
    dump_data.args = ['<dump_type>']

    @staticmethod
    def print_version():
        """Display Source.Python version information."""

    def print_credits(self):
        """List all credits for Source.Python."""
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
        self.logger.log_message(message + '=' * 61 + '\n\n')

# Get the _CoreCommand instance
_CoreCommandInstance = _CoreCommand('sp', 'Source.Python base command.')

# Register the load/unload sub-commands
_CoreCommandInstance['load'] = _CoreCommandInstance.load_plugin
_CoreCommandInstance['unload'] = _CoreCommandInstance.unload_plugin
_CoreCommandInstance['reload'] = _CoreCommandInstance.reload_plugin

# Register the 'auth' sub-command
_CoreCommandInstance['auth'] = _AuthCommandsInstance

# Register the 'delay' sub-command
_CoreCommandInstance['delay'] = _CoreCommandInstance.delay_execution
_CoreCommandInstance['delay'].args = ['<delay>', '<command>', '[arguments]']

# Register the 'dump' sub-command
_CoreCommandInstance['dump'] = _CoreCommandInstance.dump_data

# Register all printing sub-commands
_CoreCommandInstance['list'] = _CoreCommandInstance.print_plugins
_CoreCommandInstance['version'] = _CoreCommandInstance.print_version
_CoreCommandInstance['credits'] = _CoreCommandInstance.print_credits
_CoreCommandInstance['help'] = _CoreCommandInstance.print_help
