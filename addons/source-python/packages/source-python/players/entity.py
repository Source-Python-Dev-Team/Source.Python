# 

""""""

# =============================================================================
# >> IMPORTS
# =============================================================================
from importlib import import_module
from core import GAME_NAME
from core import SOURCE_ENGINE
from paths import SP_PACKAGES_PATH


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('Player',
           )


# =============================================================================
# >> GLOBAL VARIABLES
# =============================================================================
if SP_PACKAGES_PATH.joinpath(
        'players', '_entity', SOURCE_ENGINE, GAME_NAME + '.py').isfile():
    Player = import_module('players._entity.{0}.{1}'.format(
        SOURCE_ENGINE, GAME_NAME)).Player
elif SP_PACKAGES_PATH.joinpath(
        'players', '_entity', SOURCE_ENGINE, '__init__.py').isfile():
    Player = import_module('players._entity.{0}'.format(SOURCE_ENGINE)).Player
else:
    from players._entity import Player
