# ../players/engines/l4d2/__init__.py

"""Provides L4D2 specific Player based functionality."""

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
    def give_named_item(self, item, sub_type=0, unk1=False, unk2=None):
        """Give the player a named item."""
        # TODO: What are the last two arguments for?
        return [item, sub_type, unk1, unk2]
