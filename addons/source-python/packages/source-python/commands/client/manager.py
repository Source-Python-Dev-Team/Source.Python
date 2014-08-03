# ../commands/client/manager.py

"""Provides a manager class used to register client commands and filters."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
#   Commands
from commands.client import get_client_command
from commands.client import register_client_command_filter
from commands.client import unregister_client_command_filter
from commands.player import _PlayerCommandManager


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('ClientCommandManager',
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

# Get the _ClientCommandManager instance
ClientCommandManager = _ClientCommandManager()
