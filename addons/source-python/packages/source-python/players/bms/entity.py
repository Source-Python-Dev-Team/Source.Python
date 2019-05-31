# ../players/bms/entity.py

"""Provides BM:S specific Player based functionality."""

# =============================================================================
# >> CLASSES
# =============================================================================
class Player(Player):
    """Class used to interact directly with players."""

    @wrap_entity_mem_func
    def give_named_item(self, item, sub_type=0, primary_ammo=-1,
            secondary_ammo=-1):
        """Give the player a named item."""
        return [item, sub_type, primary_ammo, secondary_ammo]
