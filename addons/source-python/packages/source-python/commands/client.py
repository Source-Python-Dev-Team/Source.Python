# ../commands/client/__init__.py

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
from commands.client.command import ClientCommand
from commands.client.filter import ClientCommandFilter
from commands.client.manager import _ClientCommandManager
from commands.client.manager import client_command_manager


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
