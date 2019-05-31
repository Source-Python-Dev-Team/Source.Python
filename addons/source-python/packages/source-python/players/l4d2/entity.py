# ../players/l4d2/entity.py

"""Provides L4D2 specific Player based functionality."""

# =============================================================================
# >> CLASSES
# =============================================================================
class Player(Player):
    """Class used to interact directly with players."""

    @wrap_entity_mem_func
    def give_named_item(self, item, sub_type=0, unk1=False, unk2=None):
        """Give the player a named item."""
        # TODO: What are the last two arguments for?
        return [item, sub_type, unk1, unk2]
