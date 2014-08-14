# ../commands/client/__init__.py

"""Provides client command and client command filter functionality."""

# =============================================================================
# >> FORWARD IMPORTS
# =============================================================================
# Source.Python Imports
#   Commands
from _commands import ClientCommandDispatcher
from _commands import get_client_command
from _commands import register_client_command_filter
from _commands import unregister_client_command_filter
from commands.client.command import ClientCommand
from commands.client.filter import ClientCommandFilter
from commands.client.manager import client_command_manager


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('ClientCommand',
           'ClientCommandDispatcher',
           'ClientCommandFilter',
           'client_command_manager',
           'get_client_command',
           'register_client_command_filter',
           'unregister_client_command_filter',
           )
