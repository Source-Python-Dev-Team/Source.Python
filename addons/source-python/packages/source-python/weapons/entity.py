# ../weapons/entity.py

"""Provides simplified weapon functionality."""

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
__all__ = ('Weapon',
           )


# =============================================================================
# >> GLOBAL VARIABLES
# =============================================================================
if SP_PACKAGES_PATH.joinpath(
        'weapons', '_entity', SOURCE_ENGINE, GAME_NAME + '.py').isfile():

    # Import the game-specific 'Weapon' class
    Weapon = import_module('weapons._entity.{0}.{1}'.format(
        SOURCE_ENGINE, GAME_NAME)).Weapon

elif SP_PACKAGES_PATH.joinpath(
        'weapons', '_entity', SOURCE_ENGINE, '__init__.py').isfile():

    # Import the engine-specific 'Weapon' class
    Weapon = import_module('weapons._entity.{0}'.format(SOURCE_ENGINE)).Weapon

else:

    # Import the base 'Weapon' class
    from weapons._entity import Weapon
