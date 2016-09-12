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
    'weapons', 'engines', SOURCE_ENGINE, GAME_NAME + '.py'
).isfile():

    # Import the game-specific 'Weapon' class
    Weapon = import_module(
        'weapons.engines.{engine}.{game}'.format(
            engine=SOURCE_ENGINE,
            game=GAME_NAME,
        )
    ).Weapon

elif SP_PACKAGES_PATH.joinpath(
    'weapons', 'engines', SOURCE_ENGINE, '__init__.py'
).isfile():

    # Import the engine-specific 'Weapon' class
    Weapon = import_module(
        'weapons.engines.{engine}'.format(
            engine=SOURCE_ENGINE,
        )
    ).Weapon

else:

    # Import the base 'Weapon' class
    from weapons._base import Weapon
