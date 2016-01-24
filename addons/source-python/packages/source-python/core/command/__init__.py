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
from commands.server import server_command_manager
from commands.typed import TypedServerCommand
#   Core
from core import core_logger
from core.manager import core_plugin_manager
from core.version import VERSION
#   Cvars
from cvars import ConVar
#   Engines
from engines.server import engine_server
#   Messages
from messages import HudDestination
from messages import TextMsg
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


class _CorePermissions(object):
    """Class used to store a sub-command callback and its permission."""

    def __init__(self, sub_command, callback):
        """Store the base attributes."""
        self._sub_command = sub_command
        self._callback = callback
        self._container_class = None
        self.permission = None
        self.__doc__ = self.callback.__doc__

        # Does the callback have args?
        if hasattr(self.callback, 'args'):

            # Set the args
            self.args = self.callback.args

    @property
    def sub_command(self):
        """Return the sub_command."""
        return self._sub_command

    @property
    def callback(self):
        """Return the callback."""
        return self._callback

    @property
    def container_class(self):
        """Return the container_class."""
        return self._container_class

    def __call__(self, *args):
        """Call the callback if authorization check out."""
        # Is the client command being used?
        if self.container_class.index is not None:

            # Does the sub-command have a permission?
            if self.permission is not None:

                # Is the client not authorized to use the sub-command?
                if self.permission not in Player(
                        self.container_class.index).permissions:

                    # Notify the client that they are not authorized
                    TextMsg(
                        'You are not authorized to use the "{0} {1}" '
                        'sub-command.'.format(
                            self.container_class.command, self.sub_command),
                        HudDestination.CONSOLE).send(
                            self.container_class.index)

                    # No need to go further
                    return

        # Call the callback
        self.callback(*args)


class _CoreCommandManager(SubCommandManager):
    """Class used for executing "sp" sub-command functionality."""

    manager = core_plugin_manager
    instance = _CoreLoadedPlugin
    logger = core_command_logger

    def __setitem__(self, item, value):
        """Overwrite __setitem__ to set using _CorePermissions class."""
        # Is the item already registered?
        if item in self:

            # Raise an error
            raise KeyError('Key "{0}" already exists.'.format(item))

        # Get the _CorePermissions instance for the item
        instance = _CorePermissions(item, value)

        # Set the container class for the instance
        instance._container_class = self

        # Add the instance to the dictionary
        super().__setitem__(item, instance)

    def print_plugins(self):
        """List all currently loaded plugins."""
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
                            value.get_name(),
                            value.get_help_text(),
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

# A little hack, so I don't have to rewrite everything...
server_command_manager.unregister_commands('sp', _core_command.call_command)


# =============================================================================
# >> sp
# =============================================================================
@TypedServerCommand(['sp', 'load'])
def _sp_load(command_info, plugin):
    """Load a plugin."""
    _core_command.load_plugin(plugin)

@TypedServerCommand(['sp', 'unload'])
def _sp_unload(command_info, plugin):
    """Unload a plugin."""
    _core_command.unload_plugin(plugin)

@TypedServerCommand(['sp', 'reload'])
def _sp_reload(command_info, plugin):
    """Reload a plugin."""
    _core_command.reload_plugin(plugin)

@TypedServerCommand(['sp', 'delay'])
def _sp_delay(command_info, delay:float, command, *args):
    """Execute a command after a given delay."""
    Delay(delay, engine_server.server_command, command + ' ' + ' '.join(args))

@TypedServerCommand(['sp', 'list'])
def _sp_list(command_info):
    """List all currently loaded plugins."""
    _core_command.print_plugins()

@TypedServerCommand(['sp', 'version'])
def _sp_version(command_info):
    """Display Source.Python version information."""
    _core_command.print_version()

@TypedServerCommand(['sp', 'credits'])
def _sp_credits(command_info):
    """List all credits for Source.Python."""
    core_command_logger.log_message(
        'Current Source.Python version: {0}'.format(VERSION))

@TypedServerCommand(['sp', 'help'])
def _sp_help(command_info, command=None, *sub_commands):
    """Print all sp sub-commands or help for a specific command."""
    if command is None:
        engine_server.server_command('sp')
        return

    commands = (command,) + sub_commands
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
_sp = TypedServerCommand.parser.get_node(['sp'])
_sp.description = 'Source.Python main command.'