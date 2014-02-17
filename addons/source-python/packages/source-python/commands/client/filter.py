# ../commands/client/filter.py

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
#   Commands
from commands.filter import _BaseFilter
from commands.client.manager import ClientCommandManager


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
# Add all the global variables to __all__
__all__ = [
    'ClientCommandFilter',
]


# =============================================================================
# >> CLASSES
# =============================================================================
class ClientCommandFilter(_BaseFilter):
    '''Class used to register a client command filter'''

    # Store the class used to (un)register client command filters
    _manager_class = ClientCommandManager
