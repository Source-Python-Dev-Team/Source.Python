# ../players/weapons/types.py

"""Provides other weapon type functionality."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
#   Filters
from filters.entities import EntityIter


# =============================================================================
# >> CLASSES
# =============================================================================
class _C4(object):
    """Class used to interact with a player based on the c4 weapon."""

    def has_c4(self):
        """Return whether or not the player is carrying C4."""
        # Loop through all c4 entities on the server
        for weapon in EntityIter('weapon_c4'):

            # Is the entity's "owner" the player?
            if weapon.owner == self.handle.to_int():

                # Return True
                return True

        # If no c4 is owned by the player, return False
        return False
