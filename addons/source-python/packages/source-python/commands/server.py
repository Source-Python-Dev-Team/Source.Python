# ../commands/server.py

"""Provides say command functionality."""

# =============================================================================
# >> FORWARD IMPORTS
# =============================================================================
# Source.Python Imports
#   Commands
from _commands._server import ServerCommandDispatcher
from _commands._server import ServerCommandGenerator
from _commands._server import get_server_command
from commands.command import _BaseCommand
from commands.manager import _BaseCommandManager


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('ServerCommand',
           'ServerCommandDispatcher',
           'ServerCommandGenerator',
           '_ServerCommandManager',
           'get_server_command',
           'server_command_manager',
           )


# =============================================================================
# >> CLASSES
# =============================================================================
class _ServerCommandManager(_BaseCommandManager):
    """Manager class used to register server commands."""

    # Store the base functions
    _get_command = staticmethod(get_server_command)
    
    def _register_command(self, name, callback, *args, **kwargs):
        self._get_command(name, *args).add_callback(callback)

# The singleton object of the :class:`_ServerCommandManager` class
server_command_manager = _ServerCommandManager()


class ServerCommand(_BaseCommand):
    """Decorator class used to register a server command."""

    # Store the class used to (un)register server commands
    _manager_class = server_command_manager
