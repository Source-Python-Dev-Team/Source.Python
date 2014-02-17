# ../commands/client/command.py

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
#   Commands
from commands.command import _BaseCommand
from commands.client.manager import ClientCommandManager


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
# Add all the global variables to __all__
__all__ = [
    'ClientCommand',
]


# =============================================================================
# >> CLASSES
# =============================================================================
class ClientCommand(_BaseCommand):
    '''Decorator class used to register a client command'''

    # Store the class used to (un)register client commands
    _manager_class = ClientCommandManager
