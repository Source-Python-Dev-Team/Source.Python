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
from _engines._gamerules import GameSystem
from _engines._gamerules import GameSystemPerFrame
from _engines._gamerules import BaseGameSystemPerFrame
from _engines._gamerules import AutoGameSystemPerFrame
from _engines._gamerules import GameRules


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = (
    'AutoGameSystemPerFrame',
    'BaseGameSystemPerFrame',
    'GameSystem',
    'GameSystemPerFrame',
    'find_game_rules',
    'find_game_rules_proxy_name',)


# =============================================================================
# >> FUNCTIONS
# =============================================================================
def find_game_rules_proxy_name():
    """Find the entity name that proxies the game rules (e. g.
    ``cs_gamerules``).

    :raise ValueError:
        Raised if the game rules proxy name wasn't found.
    :rtype: str
    """
    for classname in factory_dictionary:
        if 'gamerules' not in classname:
            continue

        return classname

    raise ValueError('Unable to find game rules proxy name.')

def find_game_rules():
    """Find the game rules instance.

    :raise ValueError:
        Raised if the game rules instance wasn't found.
    :rtype: GameRules
    """
    proxy = BaseEntity.find_or_create(find_game_rules_proxy_name())
    cls = proxy.server_class
    while cls:
        for prop in cls.table:
            if 'gamerules_data' not in prop.name:
                continue

            return make_object(
                GameRules,
                prop.data_table_proxy_function(None, None, None, None, 0))

        cls = cls.next

    raise ValueError('Unable to find game rules.')
