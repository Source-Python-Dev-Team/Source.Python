# ../core/command.py

"""Registers the "sp" server command and all of its sub-commands."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Site-Package Imports
#   Configobj
from configobj import ConfigObj

# Source.Python Imports
#   Auth
from auth.commands import _auth_commands
#   Core
from core import core_logger
from core import dumps
from core.manager import core_plugin_manager
#   Cvars
from cvars import ConVar
#   Engines
from engines.server import engine_server
#   Paths
from paths import SP_DATA_PATH
#   Plugins
from plugins import _plugin_strings
from plugins.command import SubCommandManager
from plugins.info import PluginInfo
from plugins.instance import LoadedPlugin
#   Tick
from listeners.tick import tick_delays


# =============================================================================
# >> GLOBAL VARIABLES
# =============================================================================
# Get the sp.core.command logger
core_command_logger = core_logger.command


# =============================================================================
# >> CLASSES
# =============================================================================
class _CoreLoadedPlugin(LoadedPlugin):

    """Plugin instance class used to create "sp" loaded plugins."""

    logger = core_command_logger


class _CoreCommandManager(SubCommandManager):

    """Class used for executing "sp" sub-command functionality."""

    manager = core_plugin_manager
    instance = _CoreLoadedPlugin
    logger = core_command_logger

    def print_plugins(self):
        """List all currently loaded plugins."""
        # Get header messages
        message = self.prefix + _plugin_strings[
            'Plugins'].get_string() + '\n' + '=' * 61 + '\n\n'

        # Loop through all loaded plugins
        for plugin in sorted(self.manager):

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
                for item, value in info.items():

                    # Is the value a ConVar?
                    if isinstance(value, ConVar):

                        # Get the ConVar's text
                        value = '{0}:\n\t\t\t{1}: {2}'.format(
                            value.get_name(),
                            value.get_help_text(),
                            value.get_string())

                    # Add message for the current item and its value
                    message += '\t{0}:\n\t\t{1}\n'.format(item, value)

            # Was no PluginInfo instance found?
            else:

                # Add message with the current plugin's name
                message += plugin + '\n'

            # Add 1 blank line between each plugin
            message += '\n'

        # Add the ending separator
        message += '=' * 61

        # Print the message
        self.logger.log_message(message)

    @staticmethod
    def delay_execution(*args):
        """Execute a command after the given delay."""
        tick_delays.delay(
            float(args[0]),
            engine_server.server_command, ' '.join(args[1:]) + '\n')

    def dump_data(self, dump_type, filename):
        """Dump data to logs."""
        # Does the given dump type exist as a function?
        if not 'dump_{0}'.format(dump_type) in dumps.__all__:

            # If not, print message to notify of unknown dump type
            self.logger.log_message(
                'Invalid dump_type "{0}". The valid types are:'.format(
                    dump_type))

            # Loop though all dump functions
            for dump in dumps.__all__:

                # Print the current dump function
                self.logger.log_message(
                    '\t{0}'.format(dump.replace('dump_', '')))

            # No need to go further
            return

        # Call the function
        getattr(dumps, 'dump_{0}'.format(dump_type))(filename)

    # Set the methods arguments
    dump_data.args = ['<dump_type>', '<filename>']

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

# Get the _CoreCommandManager instance
_core_command = _CoreCommandManager('sp', 'Source.Python base command.')

# Register the load/unload sub-commands
_core_command['load'] = _core_command.load_plugin
_core_command['unload'] = _core_command.unload_plugin
_core_command['reload'] = _core_command.reload_plugin

# Register the 'auth' sub-command
_core_command['auth'] = _auth_commands

# Register the 'delay' sub-command
_core_command['delay'] = _core_command.delay_execution
_core_command['delay'].args = ['<delay>', '<command>', '[arguments]']

# Register the 'dump' sub-command
_core_command['dump'] = _core_command.dump_data

# Register all printing sub-commands
_core_command['list'] = _core_command.print_plugins
_core_command['version'] = _core_command.print_version
_core_command['credits'] = _core_command.print_credits
_core_command['help'] = _core_command.print_help
