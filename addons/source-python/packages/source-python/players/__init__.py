# ../players/__init__.py

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
from _players import NetChannelInfo
from _players import PlayerGenerator
from _players import PlayerInfo
from loggers import _SPLogger


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
# Set all to an empty list
__all__ = [
    'NetChannelInfo',
    'PlayerGenerator',
    'PlayerInfo',
]


# =============================================================================
# >> GLOBAL VARIABLES
# =============================================================================
# Get the sp.players logger
PlayersLogger = _SPLogger.players
