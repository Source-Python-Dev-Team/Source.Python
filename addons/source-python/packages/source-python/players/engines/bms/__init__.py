# ../players/engines/bms/__init__.py

"""Provides BM:S specific Player based functionality."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
#   Entities
from entities.helpers import wrap_entity_mem_func
#   Players
from players._base import Player as _Player


# =============================================================================
# >> CLASSES
# =============================================================================
class Player(_Player):
    """Class used to interact directly with players."""

    # Instances of this class will be cached by default
    caching = True

    @wrap_entity_mem_func
    def give_named_item(self, item, sub_type=0, primary_ammo=-1,
            secondary_ammo=-1):
        """Give the player a named item."""
        return [item, sub_type, primary_ammo, secondary_ammo]
