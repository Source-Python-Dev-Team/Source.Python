# ../engines/server.py

"""Provides access to the base Server interfaces."""

# =============================================================================
# >> FORWARD IMPORTS
# =============================================================================
# Source.Python Imports
#   Engines
from _engines import ConnectionlessPacketHandler
from _engines import QueryCvarStatus
from _engines import Server
from _engines import engine_server
from _engines import server_game_dll
#   Globals
from _globals import MapLoadType
from _globals import global_vars
#   Memory
from memory.manager import manager


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('ConnectionlessPacketHandler',
           'MapLoadType',
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
try:
    # Try to get the server using the exposed method. If that fails, because
    # it isn't implemented for this game ...
    server = engine_server.get_server()
except NotImplementedError:
    # ... fall back to the sig-scanned server
    server = manager.get_global_pointer('Server')
