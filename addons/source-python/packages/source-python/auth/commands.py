# ../auth/commands.py

"""Registers the "sp auth" sub-commands."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Python Imports
#   Collections
from collections import OrderedDict

# Source.Python imports
#   Auth
from auth import auth_logger
from auth import _auth_strings
from auth.manager import auth_manager


# =============================================================================
# >> GLOBAL VARIABLES
# =============================================================================
# Get the sp.auth.commands logger
auth_commands_logger = auth_logger.commands


# =============================================================================
# >> CLASSES
# =============================================================================
class _AuthCommands(OrderedDict):
    """Class used for executing "sp auth" sub-command functionality."""

    manager = auth_manager

    def call_command(self, args):
        """Execute the given "sp auth" sub-command."""
        # Was a command given?
        if not args:

            # Get a message that a sub-command is needed
            message = '[SP Auth] ' + _auth_strings[
                'Missing Command'].get_string() + '\n'

            # Print the auth help text
            self.print_auth_help(message)

            # No need to go further
            return

        # Get the command
        command = args[0]

        # Is the command registered?
        if command not in self:

            # Get a message about the invalid command
            message = '[SP Auth] ' + _auth_strings[
                'Invalid Sub-Command'].get_string(command=command) + '\n'

            # Print the auth help text
            self.print_auth_help(message)

            # No need to go further
            return

        # Does the given command use arguments?
        if hasattr(self[command], 'args'):

            # Execute the command with the given arguments
            self[command](args[1:])

            # Go no further
            return

        # Execute the command
        self[command]()

    def _load_auth_providers(self, providers):
        """Load the given auth providers."""
        # Were any providers given?
        if not providers:

            # Send a message about the required argument
            auth_commands_logger.log_message(
                '[SP Auth] ' + _auth_strings['Missing Load'].get_string())

            # No need to go further
            return

        # Loop through all of the given providers
        for provider in providers:

            # Load the current provider
            self.manager.load_auth(provider)

    _load_auth_providers.args = ['<provider>', '[provider]', '...']

    def _unload_auth_providers(self, providers):
        """Unload the given auth providers."""
        # Were any providers given?
        if not providers:

            # Send a message about the required argument
            auth_commands_logger.log_message(
                '[SP Auth] ' + _auth_strings['Missing Unload'].get_string())

            # No need to go further
            return

        # Loop through all of the given providers
        for provider in providers:

            # Unload the current provider
            self.manager.unload_auth(provider)

    _unload_auth_providers.args = ['<provider>', '[provider]', '...']

    def _reload_auth_providers(self, providers=None):
        """Reload the given auth providers."""
        # Were any providers given?
        if not providers:

            # Set providers to all currently loaded providers
            providers = list(self.manager)

        # Loop through the providers
        for provider in providers:

            # Reload the given provider
            self.manager.reload_auth(provider)

    _reload_auth_providers.args = ['[provider]', '[provider]', '...']

    def _print_auth_providers(self):
        """List all currently loaded auth providers."""
        # Get header messages
        message = '[SP Auth] ' + _auth_strings[
            'Providers'].get_string() + '\n' + '=' * 61 + '\n'

        # Loop through all loaded auth providers
        for provider in self.manager:

            # Add the current provider to the message
            message += provider + '\n'

        # Print ending messages
        auth_commands_logger.log_message(message + '=' * 61)

    def print_auth_help(self, message=''):
        """Print all "sp auth" sub-commands."""
        # Get header messages
        header = message + '[SP Auth] ' + _auth_strings[
            'Help'].get_string() + 'sp auth <command> [arguments]\n' + '=' * 78

        # Print all "sp auth" sub-commands
        self.print_help(header, '=' * 78)

    def print_help(self, pretext='', posttext=''):
        """Print all "sp auth" sub-commands."""
        auth_commands_logger.log_message(
            pretext + '\n' + self.help_text + '\n' + posttext)

    @property
    def help_text(self):
        """Return the help text for auth commands."""
        # Store the base message
        message = ''

        # Loop through all registered sub-commands
        for item in self:

            # Add the base text
            text = 'auth {0}'.format(item)

            # Does the current item use arguments?
            if hasattr(self[item], 'args'):

                # Add the arguments to the text
                text += ' ' + ' '.join(self[item].args)

            # Add the doc strings
            message += text + self[item].__doc__.rjust(78 - len(text)) + '\n'

        # Return the message
        return message.rstrip('\n')


# =============================================================================
# >> FUNCTIONS
# =============================================================================
# Get the _AuthCommands instance
_auth_commands = _AuthCommands()

# Add all auth loading/unloading commands to the dictionary
_auth_commands['load'] = _auth_commands._load_auth_providers
_auth_commands['unload'] = _auth_commands._unload_auth_providers
_auth_commands['reload'] = _auth_commands._reload_auth_providers

# Add all printing commands to the dictionary
_auth_commands['list'] = _auth_commands._print_auth_providers
_auth_commands['help'] = _auth_commands.print_auth_help
