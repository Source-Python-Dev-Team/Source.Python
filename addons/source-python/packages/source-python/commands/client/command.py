# ../commands/client/command.py

"""Provides a decorator class for registering client commands."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
#   Commands
from commands.command import _BaseCommand
from commands.client.manager import client_command_manager


# =============================================================================
# >> CLASSES
# =============================================================================
class ClientCommand(_BaseCommand):

    """Decorator class used to register a client command."""

    # Store the class used to (un)register client commands
    _manager_class = client_command_manager
