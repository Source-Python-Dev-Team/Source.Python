# ../commands/server/manager.py

"""Provides a manager class used to register server commands."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
#   Commands
from commands.manager import _BaseCommandManager
from commands.server import get_server_command


# =============================================================================
# >> CLASSES
# =============================================================================
class _ServerCommandManager(_BaseCommandManager):
    """Manager class used to register server commands."""

    # Store the base functions
    _get_command = staticmethod(get_server_command)

#: The singleton object of the :class:`_ServerCommandManager` class
server_command_manager = _ServerCommandManager()
