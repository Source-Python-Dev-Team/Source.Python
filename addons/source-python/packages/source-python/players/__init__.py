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
#   Core
from core import GameConfigObj
#   Memory
from memory.manager import manager
#   Paths
from paths import SP_DATA_PATH
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

BaseClient = manager.create_type_from_dict(
    'BaseClient', GameConfigObj(SP_DATA_PATH / 'client' / 'CBaseClient.ini'))
