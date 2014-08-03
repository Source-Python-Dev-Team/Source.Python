# ../players/__init__.py

"""Provides player based functionality."""

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
__all__ = ('NetChannelInfo',
           'PlayerGenerator',
           'PlayerInfo',
           )


# =============================================================================
# >> GLOBAL VARIABLES
# =============================================================================
# Get the sp.players logger
PlayersLogger = _SPLogger.players
