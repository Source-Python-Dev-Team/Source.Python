# ../commands/server/command.py

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
from public import public
#   Commands
from commands.command import _BaseCommand
from commands.server.manager import ServerCommandManager


# =============================================================================
# >> CLASSES
# =============================================================================
@public
class ServerCommand(_BaseCommand):
    '''Decorator class used to register a server command'''

    # Store the class used to (un)register server commands
    _manager_class = ServerCommandManager
