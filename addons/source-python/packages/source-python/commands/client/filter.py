# ../commands/client/filter.py

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
from public import public
#   Commands
from commands.filter import _BaseFilter
from commands.client.manager import ClientCommandManager


# =============================================================================
# >> CLASSES
# =============================================================================
@public
class ClientCommandFilter(_BaseFilter):
    '''Class used to register a client command filter'''

    # Store the class used to (un)register client command filters
    _manager_class = ClientCommandManager
