# ../players/__init__.py

"""Provides player based functionality."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
#   Loggers
from loggers import _sp_logger


# =============================================================================
# >> FORWARD IMPORTS
# =============================================================================
# Source.Python Imports
#   Players
from _players import Client
from _players import PlayerGenerator
from _players import PlayerInfo
from _players import UserCmd


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('BaseClient',
           'Client',
           'PlayerGenerator',
           'PlayerInfo',
           'UserCmd',
           )


# =============================================================================
# >> GLOBAL VARIABLES
# =============================================================================
# Get the sp.players logger
players_logger = _sp_logger.players

# Will be loaded by a setup routine
BaseClient = None
