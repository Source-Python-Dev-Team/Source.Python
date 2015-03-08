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
#   Globals
from _globals import global_vars
from _globals import MapLoadType


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('QueryCvarStatus',
           'engine_server',
           'global_vars',
           'MapLoadType',
           'server_game_dll',
           )
