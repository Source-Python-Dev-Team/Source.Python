# ../core/command/__init__.py

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
from core.version import VERSION
#   Engines
from engines.server import execute_server_command
from engines.server import queue_command_string
#   Paths
from paths import SP_DATA_PATH
#   Plugins
from plugins import _plugin_strings
from plugins.command import SubCommandManager
from plugins.manager import plugin_manager
#   Tick
from listeners.tick import Delay


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('CoreCommandManager',
           'core_command',
           )


# =============================================================================
# >> GLOBAL VARIABLES
# =============================================================================
# Get the sp.core.command logger
core_command_logger = core_logger.command


# =============================================================================
# >> CLASSES
# =============================================================================
class CoreCommandManager(SubCommandManager):
    """Class used for executing "sp" sub-command functionality."""

    def print_credits(self):
        """List all credits for Source.Python."""
        # Get header messages
        message = _plugin_strings[
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
        self.log_message(message + '=' * 61 + '\n\n')

core_command = CoreCommandManager(
    plugin_manager, 'sp', logger=core_command_logger)


# =============================================================================
# >> sp
# =============================================================================
@core_command.server_sub_command(['delay'])
def _sp_delay(command_info, delay:float, command, *args):
    """Execute a command after a given delay."""
    Delay(delay, queue_command_string, (command + ' ' + ' '.join(args), ))


@core_command.server_sub_command(['version'])
def _sp_version(command_info):
    """Display Source.Python version information."""
    core_command_logger.log_message(
        'Current Source.Python version: {0}'.format(VERSION))


@core_command.server_sub_command(['credits'])
def _sp_credits(command_info):
    """List all credits for Source.Python."""
    core_command.print_credits()


@core_command.server_sub_command(['help'])
def _sp_help(command_info, command=None, *server_sub_commands):
    """Print all sp sub-commands or help for a specific command."""
    if command is None:
        execute_server_command('sp')
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
TypedServerCommand.parser.set_node_description(
    'sp', 'Source.Python main command.')
