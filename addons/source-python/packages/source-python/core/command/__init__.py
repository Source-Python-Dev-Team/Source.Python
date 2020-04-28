# ../core/command/__init__.py

"""Registers the "sp" server command and all of its sub-commands."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Python Imports
#   Datetime
from datetime import datetime
#   Platform
import platform

# Site-Package Imports
#   Configobj
from configobj import ConfigObj

# Source.Python Imports
#   Commands
from commands.typed import TypedServerCommand
#   Core
from core import create_checksum
from core import SOURCE_ENGINE_BRANCH
from core.logger import core_logger
from core.update import do_full_update
from core.version import get_last_successful_build_number
from core.version import is_unversioned
from core.version import VERSION
from core.version import GIT_COMMIT
#   Engines
from engines.server import execute_server_command
from engines.server import queue_command_string
#   Paths
from paths import SP_DATA_PATH
#   Plugins
from plugins import _plugin_strings
from plugins.command import SubCommandManager
from plugins.manager import plugin_manager
from plugins.manager import server_plugin_manager
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


@core_command.server_sub_command(['info'])
def print_info(info):
    """Print information about OS, SP and installed plugins."""
    result = '\nDate          : {}'.format(datetime.utcnow())
    result += '\nOS            : {}'.format(platform.platform())
    result += '\nGame          : {}'.format(SOURCE_ENGINE_BRANCH)
    result += '\nSP version    : {}'.format(VERSION)
    result += '\nGithub commit : {}'.format(GIT_COMMIT)
    result += '\nServer plugins:'
    for index, plugin in enumerate(server_plugin_manager.loaded_plugins):
        result += '\n   {:02d}: {}'.format(index, plugin.name)

    result += '\nSP plugins:'
    for index, plugin in enumerate(plugin_manager.loaded_plugins):
        result += '\n   {:02d}: {}'.format(index, plugin.name)
        info = plugin.info
        if info.version != 'unversioned':
            result += ', {}'.format(info.version)

        if info.url is not None:
            result += ', {}'.format(info.url)

    # Create a checksum for all the the essential information (from 'Date' to
    # the last SP plugin)
    checksum = create_checksum(result)
    sep = '-' * 56
    core_command_logger.log_message(
        f'\nIMPORTANT: Please copy the full output.\n{sep}\n'
        f'Checksum      : {checksum}{result}\n{sep}\n')


@core_command.server_sub_command(['update'])
def update_sp(info):
    """Update Source.Python to the latest version. A restart of the server is
    required.
    """
    if not is_unversioned() and VERSION >= get_last_successful_build_number():
        core_command_logger.log_message('No new version available.')
        return
        
    do_full_update()
    

# =============================================================================
# >> DESCRIPTION
# =============================================================================
TypedServerCommand.parser.set_node_description(
    'sp', 'Source.Python main command.')
