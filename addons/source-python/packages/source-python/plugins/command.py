# ../plugins/command.py

"""Provides a way to utilize sub-commands for a server command."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Python Imports
#   Collections
from collections import OrderedDict
#   Re
import re

# Source.Python Imports
#   Commands
from commands.server import server_command_manager
#   Core
from core import AutoUnload
#   Plugins
from plugins import plugins_logger
from plugins import _plugin_strings
from plugins.errors import PluginInstanceError
from plugins.errors import PluginManagerError
from plugins.instance import LoadedPlugin
from plugins.manager import PluginManager


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
class SubCommandManager(AutoUnload, OrderedDict):
    """Class used for executing sub-commands for the given console command."""

    # Set the default class values for base attributes
    logger = plugins_command_logger
    translations = _plugin_strings

    def __init__(self, command, description='', prefix=''):
        """Called on instance initialization."""
        # Re-call OrderedDict's __init__ to properly setup the object
        super().__init__()

        # Does the class have a proper manager object assigned?
        if not (hasattr(self, 'manager') and
                isinstance(self.manager, PluginManager)):

            # If not, raise an error
            raise PluginManagerError(PluginManagerError.__doc__)

        # Does the class have a proper instance class assigned?
        if not (hasattr(self, 'instance') and
                issubclass(self.instance, LoadedPlugin)):

            # If not, raise an error
            raise PluginInstanceError(PluginInstanceError.__doc__)

        # Store the command
        self._command = command

        # Store the prefix
        self._prefix = prefix if prefix else '[{0}] '.format(
            self.command.upper())

        # Set the prefix for the manager and instance classes
        self.manager.prefix = self.instance.prefix = self.prefix

        # Set the instance class for the manager class
        self.manager.instance = self.instance

        # Register the server command
        server_command_manager.register_commands(
            self.command, self.call_command, description, 0)

    @property
    def manager(self):
        """Raise an error if the inheriting class does not have their own."""
        raise NotImplementedError('No manager attribute defined for class.')

    @property
    def instance(self):
        """Raise an error if the inheriting class does not have their own."""
        raise NotImplementedError('No instance attribute defined for class.')

    @property
    def command(self):
        """Return the server command registered to the class."""
        return self._command

    @property
    def prefix(self):
        """Return the prefix to use in log messages."""
        return self._prefix

    def _unload_instance(self):
        """Unregister commands when the instance is unloaded."""
        server_command_manager.unregister_commands(
            self.command, self.call_command)

    def call_command(self, command):
        """Get the provided sub-command and executes accordingly."""
        # Get the argument string
        arg_string = command.arg_string

        # Use try/except to split the argument string,
        # if it contains more than one argument
        try:

            # Split the argument string to get the sub-command
            sub_command, args = arg_string.split(maxsplit=1)

        # Were there not enough arguments to split?
        except ValueError:

            # Set the command to the entire string
            sub_command = arg_string.strip()

            # Set args to nothing
            args = ''

        # Split the remaining args
        args = args.split()

        # Make the sub-command lower-case
        sub_command = sub_command.lower()

        # Is the sub-command in the dictionary?
        if sub_command not in self:

            # Was a sub-command given?
            if sub_command:

                # Print a message about the invalid sub-command
                message = self.prefix + self.translations[
                    'Invalid Command'].get_string(subcommand=sub_command)

            # Was no sub-command given?
            else:

                # Print a message about the missing sub-command
                message = self.prefix + self.translations[
                    'No Command'].get_string(command=self.command)

            # Print the help text for the console command
            self.print_help(message)

            # No need to go further
            return

        # Does the given sub-command's callable allow for arguments?
        if hasattr(self[sub_command], 'args'):

            # Get the number of required arguments for the callable
            required = len([
                x for x in self[sub_command].args if x.startswith('<')])

            # Get the number of arguments provided
            given = len(args)

            # Were enough arguments provided?
            if given < required:

                # Print a message about the invalid number of arguments given
                self.logger.log_message(
                    self.prefix + self.translations[
                        'Invalid Arguments'].get_string(
                        command=self.command, subcommand=sub_command) +
                    ' '.join(self[sub_command].args))

                # No need to go further
                return

            # Are all of the arguments required?
            if required == len(self[sub_command].args):

                # Were the correct number of arguments given?
                if given != required:

                    # Print a message about the invalid
                    # number of arguments given
                    self.logger.log_message(
                        self.prefix + self.translations[
                            'Invalid Arguments'].get_string(
                            command=self.command, subcommand=sub_command) +
                        ' '.join(self[sub_command].args))

                    # No need to go further
                    return

            # Call the sub-command's callable with the given arguments
            self[sub_command](*args)

            # No need to go further
            return

        # Does the current item have its own call_command method?
        if hasattr(self[sub_command], 'call_command'):

            # Call the instance's call_command method with the arguments
            self[sub_command].call_command(args)

            # No need to go further
            return

        # Call the callable without the arguments
        self[sub_command]()

    def print_help(self, message=''):
        """Print all sub-commands for the console command."""
        # Add a header message
        message += '\n' + self.prefix + self.translations[
            'Help'].get_string(command=self.command) + '\n' + '=' * 78

        # Loop through all registered sub-commands
        for item in self:

            # Set the text
            text = str(item)

            # Does the current sub-command have its own print_help method?
            if hasattr(self[item], 'print_help'):

                # Get the instance's help text
                message += '\n' + self[item].help_text

                # Continue to the next item
                continue

            # Does the current command have any arguments?
            if hasattr(self[item], 'args'):

                # Add the arguments to the text
                text += ' ' + ' '.join(self[item].args)

            # Add a message for the current command
            message += '\n' + text + self[
                item].__doc__.rjust(78 - len(text))

        # Send the message
        self.logger.log_message(message + '\n' + '=' * 78)

    def load_plugin(self, plugin_name):
        """Load a plugin by name."""
        # Is the given plugin name a proper name?
        if not self._is_valid_plugin_name(plugin_name):

            # Send a message that the given name is invalid
            self.logger.log_message(self.prefix + self.translations[
                'Invalid Name'].get_string(plugin=plugin_name))

            # No need to go further
            return

        # Is the plugin already loaded?
        if plugin_name in self.manager:

            # Send a message that the plugin is already loaded
            self.logger.log_message(self.prefix + self.translations[
                'Already Loaded'].get_string(plugin=plugin_name))

            # No need to go further
            return

        # Load the plugin and get its instance
        plugin = self.manager[plugin_name]

        # Was the plugin unable to be loaded?
        if plugin is None:

            # Send a message that the plugin was not loaded
            self.logger.log_message(self.prefix + self.translations[
                'Unable to Load'].get_string(plugin=plugin_name))

            # No need to go further
            return

        # Send a message that the plugin was loaded
        self.logger.log_message(self.prefix + self.translations[
            'Successful Load'].get_string(plugin=plugin_name))

    # Set the method's required arguments
    load_plugin.args = ['<plugin>']

    def unload_plugin(self, plugin_name):
        """Unload a plugin by name."""
        # Is the given plugin name a proper name?
        if not self._is_valid_plugin_name(plugin_name):

            # Send a message that the given name is invalid
            self.logger.log_message(self.prefix + self.translations[
                'Invalid Name'].get_string(plugin=plugin_name))

            # No need to go further
            return

        # Is the plugin loaded?
        if plugin_name not in self.manager:

            # Send a message that the plugin is not loaded
            self.logger.log_message(self.prefix + self.translations[
                'Not Loaded'].get_string(plugin=plugin_name))

            # No need to go further
            return

        # Unload the plugin
        del self.manager[plugin_name]

        # Send a message that the plugin was unloaded
        self.logger.log_message(self.prefix + self.translations[
            'Successful Unload'].get_string(plugin=plugin_name))

    # Set the method's required arguments
    unload_plugin.args = ['<plugin>']

    def reload_plugin(self, plugin_name):
        """Reload a plugin by name."""
        # Is the given plugin name a proper name?
        if not self._is_valid_plugin_name(plugin_name):

            # Send a message that the given name is invalid
            self.logger.log_message(self.prefix + self.translations[
                'Invalid Name'].get_string(plugin=plugin_name))

            # No need to go further
            return

        # Unload the plugin
        self.unload_plugin(plugin_name)

        # Load the plugin
        self.load_plugin(plugin_name)

    # Set the method's required arguments
    reload_plugin.args = ['<plugin>']

    def print_plugins(self):
        """Print all currently loaded plugins."""
        # Get the header message
        message = self.prefix + self.translations[
            'Plugins'].get_string() + '\n' + '=' * 61 + '\n\n\t'

        # Add all loaded plugins to the message
        message += '\n\t'.join(self.manager)

        # Add a breaker at the end of the message
        message += '\n\n' + '=' * 61

        # Send the message
        self.logger.log_message(message)

    @staticmethod
    def _is_valid_plugin_name(plugin_name):
        """Return whether or not the given plugin name is valid."""
        # Get the regular expression match for the given plugin name
        match = re.match('([A-Za-z][A-Za-z0-9_]*[A-Za-z0-9])$', plugin_name)

        # Return whether it is valid or not
        return False if match is None else match.group() == plugin_name
