# ../players/__init__.py

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
#   Loggers
from loggers import _SPLogger


# =============================================================================
# >> FORWARD IMPORTS
# =============================================================================
# Source.Python Imports
#   Players
from _players import NetChannelInfo
from _players import PlayerGenerator
from _players import PlayerInfo


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
