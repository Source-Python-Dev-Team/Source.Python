# ../engines/server.py

"""Provides access to the base Server interfaces."""

# =============================================================================
# >> FORWARD IMPORTS
# =============================================================================
# Source.Python Imports
#   Engines
from _engines import engine_server
from _engines import server_game_dll
from _engines import QueryCvarStatus
from _engines import Server
#   Globals
from _globals import global_vars
from _globals import MapLoadType
#   Memory
from memory.manager import manager


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('MapLoadType',
           'QueryCvarStatus',
           'Server',
           'engine_server',
           'global_vars',
           'server',
           'server_game_dll',
           )


# =============================================================================
# >> GLOBAL VARIABLES
# =============================================================================
server = manager.get_global_pointer('Server')
