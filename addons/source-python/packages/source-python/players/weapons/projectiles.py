# ../players/weapons/projectiles.py


# =============================================================================
# >> CLASSES
# =============================================================================
class _HEGrenade(object):
    '''Class used to interact with a player based on the hegrenade weapon'''

    def get_hegrenade_indexes(self):
        '''Returns a list of hegrenade indexes for the player'''
        return self.get_weapon_index_list('weapon_hegrenade')

    def get_hegrenade_count(self):
        '''Returns the player's hegrenade ammo value'''
        return self._get_weapon_ammo('weapon_hegrenade')

    def set_hegrenade_count(self, value):
        '''Sets the player's hegrenade amount'''
        self._set_weapon_ammo(value, 'weapon_hegrenade')


class _Flashbang(object):
    '''Class used to interact with a player based on the flashbang weapon'''

    def get_flashbang_indexes(self):
        '''Returns a list of flashbang indexes for the player'''
        return self.get_weapon_index_list('weapon_flashbang')

    def get_flashbang_count(self):
        '''Returns the player's flashbang ammo value'''
        return self._get_weapon_ammo('weapon_flashbang')

    def set_flashbang_count(self, value):
        '''Sets the player's flashbang amount'''
        self._set_weapon_ammo(value, 'weapon_flashbang')


class _SmokeGrenade(object):
    '''Class used to interact with a player based on the smokegrenade weapon'''

    def get_smokegrenade_indexes(self):
        '''Returns a list of smokegrenade indexes for the player'''
        return self.get_weapon_index_list('weapon_smokegrenade')

    def get_smokegrenade_count(self):
        '''Returns the player's smokegrenade ammo value'''
        return self._get_weapon_ammo('weapon_smokegrenade')

    def set_smokegrenade_count(self, value):
        '''Sets the player's smokegrenade amount'''
        self._set_weapon_ammo(value, 'weapon_smokegrenade')


class _Decoy(object):
    '''Class used to interact with a player based on the decoy weapon'''

    def get_decoy_indexes(self):
        '''Returns a list of decoy indexes for the player'''
        return self.get_weapon_index_list('weapon_decoy')

    def get_decoy_count(self):
        '''Returns the player's decoy ammo value'''
        return self._get_weapon_ammo('weapon_decoy')

    def set_decoy_count(self, value):
        '''Sets the player's decoy amount'''
        self._set_weapon_ammo(value, 'weapon_decoy')


class _Incendiary(object):
    '''Class used to interact with a player based on the incendiary weapon'''

    def get_incendiary_indexes(self):
        '''Returns a list of incendiary indexes for the player'''
        return self.get_weapon_index_list(is_filters='incendiary')

    def get_incendiary_count(self):
        '''Returns the player's incendiary ammo value'''
        return self._get_weapon_ammo(is_filters='incendiary')

    def set_incendiary_count(self, value):
        '''Sets the player's incendiary amount'''
        self._set_weapon_ammo(value, is_filters='incendiary')
