# ../engines/gamerules.py

"""Provides access to the gamerules instance."""

# =============================================================================
# >> FORWARD IMPORTS
# =============================================================================
# Source.Python Imports
#   Engines
from _engines._gamerules import GameRules
from _engines._gamerules import find_game_rules_property_offset
from _engines._gamerules import find_game_rules
from _engines._gamerules import find_game_rules_proxy_name


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = (
    'GameRules',
    'find_game_rules_property_offset',
    'find_game_rules',
    'find_game_rules_proxy_name',)
