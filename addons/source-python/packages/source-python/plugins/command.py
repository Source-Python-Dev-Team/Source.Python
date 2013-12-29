# ../plugins/command.py

# =============================================================================
# >> IMPORTS
# =============================================================================
# Python Imports
#   Collections
from collections import OrderedDict

# Source.Python Imports
from core import AutoUnload
from public import public
#   Commands
from commands.server import ServerCommandManager
#   Plugins
from plugins import PluginsLogger
from plugins import _plugin_strings
from plugins.errors import PluginInstanceError
from plugins.errors import PluginManagerError
from plugins.instance import LoadedPlugin
from plugins.manager import PluginManager


# =============================================================================
# >> GLOBAL VARIABLES
# =============================================================================
# Get the sp.plugins.command logger
PluginCommandLogger = PluginsLogger.command


# =============================================================================
# >> CLASSES
# =============================================================================
@public
class SubCommandManager(OrderedDict, AutoUnload):
    '''Class used for executing sub-commands for the given console command'''

    def __init__(self, command, description='', prefix=''):
        '''Called on instance initialization'''

        # Re-call OrderedDict's __init__ to properly setup the object
        super(SubCommandManager, self).__init__()

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
        self.command = command

        # Store the prefix
        self.prefix = prefix if prefix else '[{0}] '.format(
            self.command.upper())

        # Set the prefix for the manager and instance classes
        self.manager.prefix = self.instance.prefix = self.prefix

        # Set the instance class for the manager class
        self.manager.instance = self.instance

        # Does the class have a logger assigned?
        if not hasattr(self, 'logger'):

            # If not, assign the base logger
            self.logger = PluginCommandLogger

        # Register the server command
        ServerCommandManager.register_commands(
            self.command, self.call_command, description, 0)

    def _unload_instance(self):
        '''Unregisters commands when the instance is unloaded'''
        ServerCommandManager.unregister_commands(
            self.command, self.call_command)

    def call_command(self, CICommand):
        '''Gets the provided sub-command and executes accordingly'''

        # Get the argument string
        arg_string = CICommand.get_arg_string()

        # Use try/except to split the argument string,
        # if it contains more than one argument
        try:

            # Split the argument string to get the sub-command
            command, args = arg_string.split(maxsplit=1)

        # Were there not enough arguments to split?
        except ValueError:

            # Set the command to the entire string
            command = arg_string.strip()

            # Set args to nothing
            args = ''

        # Split the remaining args
        args = args.split()

        # Make the sub-command lower-case
        command = command.lower()

        # Is the sub-command in the dictionary?
        if not command in self:

            # Was a sub-command given?
            if command:

                # Print a message about the invalid sub-command
                message = self.prefix + _plugin_strings[
                    'Invalid Command'].get_string(
                        command=self.command, subcommand=command)

            # Was no sub-command given?
            else:

                # Print a message about the missing sub-command
                message = self.prefix + _plugin_strings[
                    'No Command'].get_string(command=self.command)

            # Print the help text for the console command
            self.print_help(message)

            # No need to go further
            return

        # Does the given sub-command's callable allow for arguments?
        if hasattr(self[command], 'args'):

            # Get the number of required arguments for the callable
            required = len([
                x for x in self[command].args if x.startswith('<')])

            # Get the number of arguments provided
            given = len(args)

            # Were enough arguments provided?
            if given < required:

                # Print a message about the invalid number of arguments given
                self.logger.log_message(
                    self.prefix + _plugin_strings['Invalid Arguments'
                    ].get_string(command=self.command, subcommand=command) +
                    ' '.join(self[command].args))

                # No need to go further
                return

            # Are all of the arguments required?
            if required == len(self[command].args):

                # Were the correct number of arguments given?
                if given != required:

                    # Print a message about the invalid
                    # number of arguments given
                    self.logger.log_message(
                        self.prefix + _plugin_strings['Invalid Arguments'
                        ].get_string(command=self.command, subcommand=command
                        ) + ' '.join(self[command].args))

                    # No need to go further
                    return

            # Call the sub-command's callable with the given arguments
            self[command](*args)

            # No need to go further
            return

        # Does the current item have its own call_command method?
        if hasattr(self[command], 'call_command'):

            # Call the instance's call_command method with the arguments
            self[command].call_command(args)

            # No need to go further
            return

        # Call the callable without the arguments
        self[command]()

    def print_help(self, message=''):
        '''Prints all sub-commands for the console command'''

        # Add a header message
        message += '\n' + self.prefix + _plugin_strings[
            'Help'].get_string(command=self.command) + '\n' + '=' * 78

        # Loop through all registered sub-commands
        for item in self:

            # Set the text
            text = str(item)

            # Does the current sub-command have its own print_help method?
            if hasattr(self[item], 'print_help'):

                # Get the instance's help text
                message += '\n' + self[item].get_help_text()

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
        '''Loads a plugin by name'''

        # Is the plugin already loaded?
        if plugin_name in self.manager:

            # Send a message that the plugin is already loaded
            self.logger.log_message(self.prefix + _plugin_strings[
                'Already Loaded'].get_string(plugin=plugin_name))

            # No need to go further
            return

        # Load the plugin and get its instance
        plugin = self.manager[plugin_name]

        # Was the plugin unable to be loaded?
        if plugin is None:

            # Send a message that the plugin was not loaded
            self.logger.log_message(self.prefix + _plugin_strings[
                'Unable to Load'].get_string(plugin=plugin_name))

            # No need to go further
            return

        # Send a message that the plugin was loaded
        self.logger.log_message(self.prefix + _plugin_strings[
            'Successful Load'].get_string(plugin=plugin_name))

    # Set the method's required arguments
    load_plugin.args = ['<plugin>']

    def unload_plugin(self, plugin_name):
        '''Unloads a plugin by name'''

        # Is the plugin loaded?
        if not plugin_name in self.manager:

            # Send a message that the plugin is not loaded
            self.logger.log_message(self.prefix + _plugin_strings[
                'Not Loaded'].get_string(plugin=plugin_name))

            # No need to go further
            return

        # Unload the plugin
        del self.manager[plugin_name]

        # Send a message that the plugin was unloaded
        self.logger.log_message(self.prefix + _plugin_strings[
            'Successful Unload'].get_string(plugin=plugin_name))

    # Set the method's required arguments
    unload_plugin.args = ['<plugin>']

    def reload_plugin(self, plugin_name):
        '''Reloads a plugin by name'''

        # Unload the plugin
        self.unload_plugin(plugin_name)

        # Load the plugin
        self.load_plugin(plugin_name)

    # Set the method's required arguments
    reload_plugin.args = ['<plugin>']

    def print_plugins(self):
        '''Prints all currently loaded plugins'''

        # Get the header message
        message = self.prefix + _plugin_strings[
            'Plugins'].get_string() + '\n' + '=' * 61 + '\n\n\t'

        # Add all loaded plugins to the message
        message += '\n\t'.join(self.manager)

        # Add a breaker at the end of the message
        message += '\n' + '=' * 61

        # Send the message
        self.logger.log_message(message)
