# ../players/games/__init__.py

"""Provides base classes to be used when the game is not natively supported."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Python Imports
#   Importlib
from importlib import import_module

# Source.Python Imports
#   Core
from core import GAME_NAME
#   Players
from players.classes.language import _LanguagePropertyBase


# =============================================================================
# >> CLASSES
# =============================================================================
class _BaseWeapons(object):

    """Base class for game specific weapon functionality."""


class _BaseClasses(_LanguagePropertyBase):

    """Base class for game specific functionality."""


# =============================================================================
# >> GLOBAL VARIALBES
# =============================================================================
# Use try/except to import the game specific classes
try:

    # Get the game's module
    _game_module = import_module('players.games.{0}'.format(GAME_NAME))

except ImportError:

    # Set the variable to None
    _game_module = None

# Use try/except to store the game specific weapons
try:
    _GameWeapons = _game_module._GameWeapons

# If the module doesn't contain the class, use the base class
except AttributeError:
    _GameWeapons = _BaseWeapons

# Use try/except to store the game specific classes
try:
    _GameClasses = _game_module._GameClasses

# If the module doesn't contain the class, use the base class
except AttributeError:
    _GameClasses = _BaseClasses
