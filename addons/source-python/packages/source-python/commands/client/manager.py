# ../commands/client/manager.py

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
from command_c import get_client_command
from command_c import register_client_command_filter
from command_c import unregister_client_command_filter
#   Commands
from commands.player import _PlayerCommandManager


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
# Add all the global variables to __all__
__all__ = [
    'ClientCommandManager',
]


# =============================================================================
# >> CLASSES
# =============================================================================
class _ClientCommandManager(_PlayerCommandManager):
    '''Manager class used to register client
        commands and client command filters'''

    # Store the base functions
    _get_command = staticmethod(get_client_command)
    register_filter = staticmethod(register_client_command_filter)
    unregister_filter = staticmethod(unregister_client_command_filter)

# Get the _ClientCommandManager instance
ClientCommandManager = _ClientCommandManager()
