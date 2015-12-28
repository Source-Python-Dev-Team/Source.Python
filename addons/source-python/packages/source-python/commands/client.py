# ../commands/client.py

"""Provides client command and client command filter functionality."""

# =============================================================================
# >> FORWARD IMPORTS
# =============================================================================
# Source.Python Imports
#   Commands
from _commands._client import ClientCommandDispatcher
from _commands._client import ClientCommandGenerator
from _commands._client import get_client_command
from _commands._client import register_client_command_filter
from _commands._client import unregister_client_command_filter
from commands.command import _BaseCommand
from commands.filter import _BaseFilter
from commands.player import _PlayerCommandManager


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('ClientCommand',
           'ClientCommandDispatcher',
           'ClientCommandFilter',
           'ClientCommandGenerator',
           '_ClientCommandManager',
           'client_command_manager',
           'get_client_command',
           'register_client_command_filter',
           'unregister_client_command_filter',
           )


# =============================================================================
# >> CLASSES
# =============================================================================
class _ClientCommandManager(_PlayerCommandManager):
    """Registers client commands and client command filters."""

    # Store the base functions
    _get_command = staticmethod(get_client_command)
    register_filter = staticmethod(register_client_command_filter)
    unregister_filter = staticmethod(unregister_client_command_filter)

# The singleton object of the :class:`_ClientCommandManager` class
client_command_manager = _ClientCommandManager()


class ClientCommand(_BaseCommand):
    """Decorator class used to register a client command."""

    # Store the class used to (un)register client commands
    _manager_class = client_command_manager


class ClientCommandFilter(_BaseFilter):
    """Class used to register a client command filter."""

    # Store the class used to (un)register client command filters
    _manager_class = client_command_manager
