# ../engines/gamerules.py

"""Provides access to the gamerules instance."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
#   Memory
from memory import make_object
#   Entities
from entities.entity import BaseEntity
from entities.factories import factory_dictionary


# =============================================================================
# >> FORWARD IMPORTS
# =============================================================================
# Source.Python Imports
#   Engines
#from _engines._gamerules import GameSystem
#from _engines._gamerules import GameSystemPerFrame
#from _engines._gamerules import BaseGameSystemPerFrame
#from _engines._gamerules import AutoGameSystemPerFrame
from _engines._gamerules import GameRules
from _engines._gamerules import find_game_rules_property_offset
from _engines._gamerules import find_game_rules
from _engines._gamerules import find_game_rules_proxy_name


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = (
#    'AutoGameSystemPerFrame',
#    'BaseGameSystemPerFrame',
#    'GameSystem',
#    'GameSystemPerFrame',
    'GameRules',
    'find_game_rules_property_offset',
    'find_game_rules',
    'find_game_rules_proxy_name',)
