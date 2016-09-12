# ../players/_language/__init__.py

"""Provides a function to get a player's language."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
#   Core
from core import GAME_NAME
#   Players
if GAME_NAME in ('csgo', ):
    from .cache import get_client_language
else:
    from .base import get_client_language
