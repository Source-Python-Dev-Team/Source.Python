# ../players/weapons/types.py

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
from conversions_c import index_from_edict
from entity_c import EntityGenerator
#   Entities
from entities.entity import BaseEntity


# =============================================================================
# >> CLASSES
# =============================================================================
class _C4(object):
    '''Class used to interact with a player based on the c4 weapon'''

    def has_c4(self):
        '''Returns whether or not the player is carrying C4'''

        # Loop through all c4 entities on the server
        for edict in EntityGenerator('weapon_c4'):

            # Get the entity's index
            index = index_from_edict(edict)

            # Get the entity's BaseEntity instance
            entity = BaseEntity(index)

            # Is the entity's "owner" the player?
            if entity.owner == self.handle.to_int():

                # Return True
                return True

        # If no c4 is owned by the player, return False
        return False
