# ../commands/server/command.py

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
#   Commands
from commands.command import _BaseCommand
from commands.server.manager import ServerCommandManager


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
# Add all the global variables to __all__
__all__ = [
    'ServerCommand',
]


# =============================================================================
# >> CLASSES
# =============================================================================
class ServerCommand(_BaseCommand):
    '''Decorator class used to register a server command'''

    # Store the class used to (un)register server commands
    _manager_class = ServerCommandManager
