# ../players/games/csgo.py

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
from entity_c import EntityGenerator
#   Entities
from entities.entity import BaseEntity
from entities.helpers import index_from_edict


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
# Set all to an empty list
__all__ = []


# =============================================================================
# >> CLASSES
# =============================================================================
class _GameWeapons(object):
    '''Game-specific class inherited by _PlayerWeapons to
        handle CS:GO specific weapon functionality for players'''

    # =========================================================================
    # >> WEAPON INDEXES
    # =========================================================================
    def get_hegrenade_indexes(self):
        '''Returns a list of hegrenade indexes for the player'''
        return self.get_weapon_index_list('weapon_hegrenade')

    def get_flashbang_indexes(self):
        '''Returns a list of flashbang indexes for the player'''
        return self.get_weapon_index_list('weapon_flashbang')

    def get_smoke_grenade_indexes(self):
        '''Returns a list of smokegrenade indexes for the player'''
        return self.get_weapon_index_list('weapon_smokegrenade')

    def get_incendiary_grenade_indexes(self):
        '''Returns a list of incendiary/molotov indexes for the player'''
        return self.get_weapon_index_list(is_filters='incendiary')

    def get_decoy_grenade_indexes(self):
        '''Returns a list of decoy grenade indexes for the player'''
        return self.get_weapon_index_list('weapon_decoy')

    # =========================================================================
    # >> GET AMMO
    # =========================================================================
    def get_hegrenade_count(self):
        '''Returns the player's hegrenade ammo value'''
        return self._get_weapon_ammo('weapon_hegrenade')

    def get_flashbang_count(self):
        '''Returns the player's flashbang ammo value'''
        return self._get_weapon_ammo('weapon_flashbang')

    def get_smoke_grenade_count(self):
        '''Returns the player's smokegrenade ammo value'''
        return self._get_weapon_ammo('weapon_smokegrenade')

    def get_incendiary_grenade_count(self):
        '''Returns the player's incendiary/molotov ammo value'''
        return self._get_weapon_ammo(is_filters='incendiary')

    def get_decoy_grenade_count(self):
        '''Returns the player's decoy grenade ammo value'''
        return self._get_weapon_ammo('weapon_decoy')

    # =========================================================================
    # >> SET AMMO
    # =========================================================================
    def set_hegrenade_count(self, value):
        '''Sets the player's hegrenade amount'''
        self._set_weapon_ammo(value, 'weapon_hegrenade')

    def set_flashbang_count(self, value):
        '''Sets the player's flashbang amount'''
        self._set_weapon_ammo(value, 'weapon_flashbang')

    def set_smoke_grenade_count(self, value):
        '''Sets the player's smoke grenade amount'''
        self._set_weapon_ammo(value, 'weapon_smokegrenade')

    def set_incendiary_grenade_count(self, value):
        '''Sets the player's incendiary/molotov grenade amount'''
        self._set_weapon_ammo(value, is_filters='incendiary')

    def set_decoy_grenade_count(self, value):
        '''Sets the player's decoy grenade amount'''
        self._set_weapon_ammo(value, 'weapon_decoy')

    # =========================================================================
    # >> OTHER METHODS
    # =========================================================================
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
