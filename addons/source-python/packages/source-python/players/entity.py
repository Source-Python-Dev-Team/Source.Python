# ../players/entity.py

"""Provides a class used to interact with a specific player."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Python Imports
#   Importlib
from importlib import import_module

# Source.Python Imports
#   Core
from core import GAME_NAME
from core import SOURCE_ENGINE
#   Paths
from paths import SP_PACKAGES_PATH
#   Players
import players._base


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('Player',
           )


# =============================================================================
# >> GLOBAL VARIABLES
# =============================================================================
if SP_PACKAGES_PATH.joinpath(
    'players', 'engines', SOURCE_ENGINE, GAME_NAME + '.py'
).isfile():

    # Import the game-specific 'Player' class
    Player = players._base.Player = import_module(
        'players.engines.{engine}.{game}'.format(
            engine=SOURCE_ENGINE,
            game=GAME_NAME,
        )
    ).Player

elif SP_PACKAGES_PATH.joinpath(
    'players', 'engines', SOURCE_ENGINE, '__init__.py'
).isfile():

    # Import the engine-specific 'Player' class
    Player = players._base.Player = import_module(
        'players.engines.{engine}'.format(
            engine=SOURCE_ENGINE,
        )
    ).Player

else:

    # Import the base 'Player' class
    Player = players._base.Player
