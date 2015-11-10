# ../commands/server/command.py

"""Provides a decorator class for registering server commands."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
#   Commands
from commands.command import _BaseCommand
from commands.server.manager import server_command_manager


# =============================================================================
# >> CLASSES
# =============================================================================
class ServerCommand(_BaseCommand):
    """Decorator class used to register a server command."""

    # Store the class used to (un)register server commands
    _manager_class = server_command_manager
