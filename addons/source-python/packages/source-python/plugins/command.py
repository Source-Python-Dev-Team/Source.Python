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
    ''''''

    def __init__(self, command, description='', prefix=''):
        ''''''

        # 
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
        ''''''
        ServerCommandManager.unregister_commands(
            self.command, self.call_command)

    def call_command(self, CICommand):
        ''''''

        # 
        arg_string = CICommand.get_arg_string()
        #
        try:

            # 
            command, args = arg_string.split(maxsplit=1)

        except ValueError:

            # 
            command = arg_string.strip()

            # 
            args = ''

        # 
        args = args.split()

        # 
        command = command.lower()

        # 
        if not command in self:

            # 
            if command:

                # 
                message = self.prefix + _plugin_strings[
                    'Invalid Command'].get_string(
                        command=self.command, subcommand=command)

            # 
            else:

                # 
                message = self.prefix + _plugin_strings[
                    'No Command'].get_string(command=self.command)

            self.print_help(message)

            # 
            return

        # 
        if hasattr(self[command], 'args'):

            # 
            required = len([
                x for x in self[command].args if x.startswith('<')])

            # 
            given = len(args)

            # 
            if given < required:

                # 
                self.logger.log_message(
                    self.prefix + _plugin_strings['Invalid Arguments'
                    ].get_string(command=self.command, subcommand=command) +
                    ' '.join(self[command].args))

                # 
                return

            # 
            if required == len(self[command].args):

                # 
                if given != required:

                    # 
                    self.logger.log_message(
                        self.prefix + _plugin_strings['Invalid Arguments'
                        ].get_string(command=self.command, subcommand=command
                        ) + ' '.join(self[command].args))

                    # 
                    return

            # 
            self[command](*args)

            # 
            return

        # 
        if hasattr(self[command], 'call_command'):

            # 
            self[command].call_command(args)

            # 
            return

        # 
        self[command]()

    def print_help(self, message=''):
        ''''''

        # 
        message += '\n' + self.prefix + _plugin_strings[
            'Help'].get_string(command=self.command) + '\n' + '=' * 78

        # 
        for item in self:

            # 
            text = str(item)

            # 
            if hasattr(self[item], 'print_help'):

                # 
                message += '\n' + self[item]._get_help_text()

                # 
                continue

            # 
            if hasattr(self[item], 'args'):

                # 
                text += ' ' + ' '.join(self[item].args)

            # 
            message += '\n' + text + self[
                item].__doc__.rjust(78 - len(text))

        # 
        self.logger.log_message(message + '\n' + '=' * 78)

    def load_plugin(self, plugin_name):
        ''''''

        # 
        if plugin_name in self.manager:

            # 
            self.logger.log_message(self.prefix + _plugin_strings[
                'Already Loaded'].get_string(plugin=plugin_name))

            # 
            return

        # 
        plugin = self.manager[plugin_name]

        # 
        if plugin is None:

            # 
            self.logger.log_message(self.prefix + _plugin_strings[
                'Unable to Load'].get_string(plugin=plugin_name))

            # 
            return

        # 
        self.logger.log_message(self.prefix + _plugin_strings[
            'Successful Load'].get_string(plugin=plugin_name))

    load_plugin.args = ['<plugin>']

    def unload_plugin(self, plugin_name):
        ''''''

        # 
        if not plugin_name in self.manager:

            # 
            self.logger.log_message(self.prefix + _plugin_strings[
                'Not Loaded'].get_string(plugin=plugin_name))

            # 
            return

        # 
        del self.manager[plugin_name]

        # 
        self.logger.log_message(self.prefix + _plugin_strings[
            'Successful Unload'].get_string(plugin=plugin_name))

    unload_plugin.args = ['<plugin>']

    def reload_plugin(self, plugin_name):
        ''''''

        # 
        self.unload_plugin(plugin_name)

        # 
        self.load_plugin(plugin_name)

    reload_plugin.args = ['<plugin>']

    def print_plugins(self):
        ''''''

        # 
        message = self.prefix + _plugin_strings[
            'Plugins'].get_string() + '\n' + '=' * 61 + '\n\n\t'

        # 
        message += '\n\t'.join(self.manager)

        # 
        message += '\n' + '=' * 61

        # 
        self.logger.log_message(message)
