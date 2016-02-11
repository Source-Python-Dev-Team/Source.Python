# ../engines/server.py

"""Provides access to the base Server interfaces."""

# =============================================================================
# >> FORWARD IMPORTS
# =============================================================================
# Source.Python Imports
#   Engines
from _engines._server import ConnectionlessPacketHandler
from _engines._server import QueryCvarStatus
from _engines._server import Server
from _engines._server import engine_server
from _engines._server import server_game_dll
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
    server = engine_server.server
except NotImplementedError:
    # ... fall back to the sig-scanned server
    server = manager.get_global_pointer('Server')
