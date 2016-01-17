# ../players/games/__init__.py

"""Imports game specific functionality for the Player class."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Python Imports
#   Importlib
from importlib import import_module

# Source.Python Imports
#   Core
from core import GAME_NAME


# =============================================================================
# >> GLOBAL VARIALBES
# =============================================================================
# Get the base module
from players.games import base as _base_module

# Use try/except to import the game specific classes
try:

    # Get the game's module
    _game_module = import_module('players.games.{0}'.format(GAME_NAME))

# If the module doesn't exist, set the variable to None
except ImportError:
    _game_module = None

# Loop through each object to import
for object_name in ('_GamePlayer', 'get_client_language'):

    # Use try/except to get the game specific object
    try:
        globals()[object_name] = getattr(_game_module, object_name)

    # If their is no game specific object, use the base object
    except AttributeError:
        globals()[object_name] = getattr(_base_module, object_name)
