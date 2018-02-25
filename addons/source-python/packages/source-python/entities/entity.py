# ../entities/entity.py

"""Provides a class used to interact with a specific entity."""

# =============================================================================
# >> IMPORTS
# =============================================================================
from importlib import import_module
from core import GAME_NAME
from core import SOURCE_ENGINE
from paths import SP_PACKAGES_PATH


# =============================================================================
# >> FORWARD IMPORTS
# =============================================================================
# Source.Python Imports
#   Entities
from _entities._entity import BaseEntity


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('BaseEntity',
           'Entity',
           )


# =============================================================================
# >> GLOBAL VARIABLES
# =============================================================================
if SP_PACKAGES_PATH.joinpath(
    'entities', 'engines', SOURCE_ENGINE, GAME_NAME + '.py'
).isfile():

    # Import the game-specific 'Entity' class
    Entity = import_module(
        'entities.engines.{engine}.{game}'.format(
            engine=SOURCE_ENGINE,
            game=GAME_NAME,
        )
    ).Entity

elif SP_PACKAGES_PATH.joinpath(
    'entities', 'engines', SOURCE_ENGINE, '__init__.py'
).isfile():

    # Import the engine-specific 'Entity' class
    Entity = import_module(
        'entities.engines.{engine}'.format(
            engine=SOURCE_ENGINE,
        )
    ).Entity

else:

    # Import the base 'Entity' class
    from entities._base import Entity
