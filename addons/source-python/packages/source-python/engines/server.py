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
from _engines._server import execute_server_command
from _engines._server import queue_command_string
from _engines._server import queue_server_command
from _engines._server import insert_command_string
from _engines._server import insert_server_command
#   Globals
from _globals import MapLoadType
from _globals import global_vars


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('ConnectionlessPacketHandler',
           'MapLoadType',
           'QueryCvarStatus',
           'Server',
           'server',
           'engine_server',
           'execute_server_command',
           'global_vars',
           'queue_command_string',
           'queue_server_command',
           'insert_command_string',
           'insert_server_command',
           'server_game_dll',
           )


# =============================================================================
# >> GLOBAL VARIABLES
# =============================================================================
# The singleton object of the :class:`Server` class.
server = None
