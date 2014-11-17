# ../commands/client/filter.py

"""Provides a decorator class used for registering client command filters."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
#   Commands
from commands.filter import _BaseFilter
from commands.client.manager import client_command_manager


# =============================================================================
# >> CLASSES
# =============================================================================
class ClientCommandFilter(_BaseFilter):

    """Class used to register a client command filter."""

    # Store the class used to (un)register client command filters
    _manager_class = client_command_manager
