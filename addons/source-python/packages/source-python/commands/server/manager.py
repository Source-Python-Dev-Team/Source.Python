# ../commands/server/manager.py

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
#   Commands
from commands.manager import _BaseCommandManager
from commands.server import get_server_command


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('ServerCommandManager',
           )


# =============================================================================
# >> CLASSES
# =============================================================================
class _ServerCommandManager(_BaseCommandManager):
    '''Manager class used to register server commands'''

    # Store the base functions
    _get_command = staticmethod(get_server_command)

# Get the _ServerCommandManager instance
ServerCommandManager = _ServerCommandManager()
