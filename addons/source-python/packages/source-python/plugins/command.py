# ../plugins/command.py

"""Provides a way to utilize sub-commands for a server command."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
#   Commands
from commands.typed import (
    TypedClientCommand, TypedSayCommand, TypedServerCommand,
)
#   Core
from core import AutoUnload
#   Hooks
from hooks.exceptions import except_hooks
#   Paths
from paths import GAME_PATH
#   Plugins
from plugins import plugins_logger
from plugins import _plugin_strings
from plugins.manager import PluginFileNotFoundError
from plugins.manager import InvalidPluginName
from plugins.manager import PluginAlreadyLoaded
from plugins.manager import PluginHasBuiltInName
from plugins.manager import PluginNotLoaded


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('SubCommandManager',
           )


# =============================================================================
# >> GLOBAL VARIABLES
# =============================================================================
# Get the sp.plugins.command logger
plugins_command_logger = plugins_logger.command


# =============================================================================
# >> CLASSES
# =============================================================================
class SubCommandManager(AutoUnload, list):
    """Class used for executing sub-commands for the given console command."""

    # Set the default class values for base attributes
    logger = plugins_command_logger
    translations = _plugin_strings

    def __init__(self, command, prefix=''):
        """Initializes the sub-command manager.

        :param str command:
            Command to register.
        :param str prefix:
            Prefix used for printing to the console.
        """
        # Re-call OrderedDict's __init__ to properly setup the object
        super().__init__()
        self._command = command
        self._prefix = prefix if prefix else '[{0}] '.format(
            self.command.upper())

    @property
    def manager(self):
        """Return a plugin manager.

        :rtype: PluginManager
        """
        raise NotImplementedError('No manager attribute defined for class.')

    def _unload_instance(self):
        """Unload all sub-commands."""
        for item in self:
            item._unload_instance()

        # Probably not necessary, but just in case...
        self.clear()

    def server_sub_command(self, commands):
        """Add a server sub-command.

        .. seealso:: :class:`commands.typed.TypedServerCommand`
        """
        if isinstance(commands, str):
            commands = [commands]
        command = TypedServerCommand([self._command] + list(commands))
        self.append(command)
        return command

    def client_sub_command(self, commands, permission=None):
        """Add a client sub-command.

        .. seealso:: :class:`commands.typed.TypedClientCommand`
        """
        if isinstance(commands, str):
            commands = [commands]
        command = TypedClientCommand(
            [self._command] + list(commands),
            permission=permission,
        )
        self.append(command)
        return command

    def say_sub_command(self, commands, permission=None):
        """Add a say sub-command.

        .. seealso:: :class:`commands.typed.TypedSayCommand`
        """
        if isinstance(commands, str):
            commands = [commands]
        command = TypedSayCommand(
            [self._command] + list(commands),
            permission=permission,
        )
        self.append(command)
        return command

    @property
    def command(self):
        """Return the server command registered to the class.

        :rtype: str
        """
        return self._command

    @property
    def prefix(self):
        """Return the prefix to use in log messages.

        :rtype: str
        """
        return self._prefix

    def load_plugin(self, plugin_name):
        """Load a plugin by name.

        :param str plugin_name:
            Name of the plugin to load.
        :return:
            Return the loaded plugin. Return ``None`` on failure.
        :rtype: LoadedPlugin
        """
        plugin = None
        self.log_message(self.translations[
            'Loading'].get_string(plugin=plugin_name))
        try:
            plugin = self.manager.load(plugin_name)
        except InvalidPluginName:
            self.log_message(self.translations[
                'Invalid Name'].get_string(plugin=plugin_name))
        except PluginAlreadyLoaded:
            self.log_message(self.translations[
                'Already Loaded'].get_string(plugin=plugin_name))
        except PluginFileNotFoundError:
            self.log_message(self.translations[
                'No Module'].get_string(
                    plugin=plugin_name, file=GAME_PATH.relpathto(
                        self.manager.get_plugin_file_path(
                            plugin_name)).replace('\\', '/')))
        except PluginHasBuiltInName:
            self.log_message(self.translations[
                'Built-in'].get_string(plugin=plugin_name))
        except:
            except_hooks.print_exception()
            self.log_message(self.translations[
                'Unable to Load'].get_string(plugin=plugin_name))
        else:
            self.log_message(self.translations[
                'Successful Load'].get_string(plugin=plugin_name))

        return plugin

    def unload_plugin(self, plugin_name):
        """Unload a plugin by name.

        :param str plugin_name:
            Name of the plugin to unload.
        """
        self.log_message(self.translations[
            'Unloading'].get_string(plugin=plugin_name))
        try:
            self.manager.unload(plugin_name)
        except InvalidPluginName:
            self.log_message(self.translations[
                'Invalid Name'].get_string(plugin=plugin_name))
        except PluginNotLoaded:
            self.log_message(self.translations[
                'Not Loaded'].get_string(plugin=plugin_name))
        else:
            self.log_message(self.translations[
                'Successful Unload'].get_string(plugin=plugin_name))

    def reload_plugin(self, plugin_name):
        """Reload a plugin by name.

        :param str plugin_name:
            Name of the plugin to reload.
        :return:
            Return the loaded plugin. Return ``None`` on failure.
        :rtype: LoadedPlugin
        """
        self.unload_plugin(plugin_name)
        return self.load_plugin(plugin_name)

    def print_plugins(self):
        """Print all currently loaded plugins."""
        # Get the header message
        message = self.translations[
            'Plugins'].get_string() + '\n' + '=' * 61 + '\n\n\t'

        # Add all loaded plugins to the message
        message += '\n\t'.join(self.manager)

        # Add a breaker at the end of the message
        message += '\n\n' + '=' * 61

        # Send the message
        self.log_message(message)

    def log_message(self, message):
        """Log a message. The prefix will be added automatically.

        :param str message:
            Message to log.
        """
        self.logger.log_message(self.prefix + message)
