# ../players/orangebox/entity.py

"""Provides Orangebox specific Player based functionality."""

# =============================================================================
# >> CLASSES
# =============================================================================
class Player(Player):
    """Class used to interact directly with players."""

    @wrap_entity_mem_func
    def give_named_item(self, item, sub_type=0):
        """Give the player a named item."""
        return [item, sub_type]
