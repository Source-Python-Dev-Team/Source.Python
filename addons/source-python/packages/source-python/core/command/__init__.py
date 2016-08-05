# ../core/commands/command.py

"""Registers the "sp" server command and all of its sub-commands."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Site-Package Imports
#   Configobj
from configobj import ConfigObj

# Source.Python Imports
#   Commands
from commands.typed import TypedServerCommand
#   Core
from core import core_logger
from core.manager import core_plugin_manager
from core.version import VERSION
#   Cvars
from cvars import ConVar
#   Engines
from engines.server import engine_server
#   Paths
from paths import SP_DATA_PATH
#   Players
from players.entity import Player
#   Plugins
from plugins import _plugin_strings
from plugins.command import SubCommandManager
from plugins.instance import LoadedPlugin
#   Tick
from listeners.tick import Delay


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
        """List all currently loaded plugins.
        
        .. todo:: Move this to :class:`plugins.command.SubCommandManager`?
        """
        # Get header messages
        message = self.prefix + _plugin_strings[
            'Plugins'].get_string() + '\n' + '=' * 61 + '\n\n'

        # Loop through all loaded plugins
        for plugin_name in sorted(self.manager):
            info = self.manager[plugin_name].info

            # Was an PluginInfo instance found?
            if info is not None:

                # Add message with the current plugin's name
                message += plugin_name + ':\n'

                # Loop through all items in the PluginInfo instance
                for item, value in info.items():

                    # Is the value a ConVar?
                    if isinstance(value, ConVar):

                        # Get the ConVar's text
                        value = '{0}:\n\t\t\t{1}: {2}'.format(
                            value.name,
                            value.help_text,
                            value.get_string())

                    # Add message for the current item and its value
                    message += '\t{0}:\n\t\t{1}\n'.format(item, value)

            # Was no PluginInfo instance found?
            else:

                # Add message with the current plugin's name
                message += plugin_name + '\n'

            # Add 1 blank line between each plugin
            message += '\n'

        # Add the ending separator
        message += '=' * 61

        # Print the message
        self._log_message(message)

    def print_credits(self):
        """List all credits for Source.Python."""
        # Get header messages
        message = self.prefix + _plugin_strings[
            'Credits'].get_string() + '\n' + '=' * 61 + '\n\n'

        # Get the credits information
        groups = ConfigObj(
            SP_DATA_PATH / 'credits.ini', encoding='unicode_escape')

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
        self._log_message(message + '=' * 61 + '\n\n')

# Get the _CoreCommandManager instance
_core_command = _CoreCommandManager('sp', 'Source.Python base command.')


# =============================================================================
# >> sp
# =============================================================================
@_core_command.server_sub_command(['delay'])
def _sp_delay(command_info, delay:float, command, *args):
    """Execute a command after a given delay."""
    Delay(delay, engine_server.server_command, command + ' ' + ' '.join(args))

@_core_command.server_sub_command(['version'])
def _sp_version(command_info):
    """Display Source.Python version information."""
    core_command_logger.log_message(
        'Current Source.Python version: {0}'.format(VERSION))

@_core_command.server_sub_command(['credits'])
def _sp_credits(command_info):
    """List all credits for Source.Python."""
    _core_command.print_credits()

@_core_command.server_sub_command(['help'])
def _sp_help(command_info, command=None, *server_sub_commands):
    """Print all sp sub-commands or help for a specific command."""
    if command is None:
        engine_server.server_command('sp')
        return

    commands = (command,) + server_sub_commands
    try:
        node = TypedServerCommand.parser.get_node(commands)
    except ValueError:
        core_command_logger.log_message(
            'Command "{}" does not exist.'.format(' '.join(commands)))
        return

    core_command_logger.log_message(node.signature)
    if node.description is not None:
        core_command_logger.log_message('  ' + node.description)


# =============================================================================
# >> DESCRIPTION
# =============================================================================
_sp = TypedServerCommand.parser.get_node('sp')
_sp.description = 'Source.Python main command.'
