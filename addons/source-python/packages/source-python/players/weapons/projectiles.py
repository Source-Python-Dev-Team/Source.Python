# ../players/weapons/projectiles.py


# =============================================================================
# >> CLASSES
# =============================================================================
class _ProjectileMeta(type):
    '''Metaclass used to auto-create methods specific to the projectile'''

    def __new__(mcl, name, bases, odict):
        '''Create the class and create its methods dynamically'''

        # Create the object
        cls = super().__new__(mcl, name, bases, odict)

        # Is the the baseclass that uses the metaclass?
        if not bases:

            # Do not add any methods
            return cls

        # Get the weapon's name based off of the class name
        method_name = name.strip('_').lower()

        # Create the iterator <weapon>_indexes method
        setattr(
            cls, '{0}_indexes'.format(method_name),
            lambda self: self._projectile_indexes)

        # Create the get_<weapon>_indexes method
        setattr(
            cls, 'get_{0}_indexes'.format(method_name),
            lambda self: self._get_projectile_index_list())

        # Create the get_<weapon>_count method
        setattr(
            cls, 'get_{0}_count'.format(method_name),
            lambda self: self._get_projectile_ammo())

        # Create the set_<weapon>_count method
        setattr(
            cls, 'set_{0}_count'.format(method_name),
            lambda self, value: self._set_projectile_ammo(value))

        # Return the new class
        return cls


class _ProjectileBase(metaclass=_ProjectileMeta):
    '''Base Projectile class used to interact with player projectiles'''

    # Store the base attributes all as None
    _classname = None
    _is_filters = None
    _not_filters = None

    def _projectile_indexes(self):
        '''Iterates over all indexes the player owns for the projectile type'''
        return self.weapon_indexes(
            self._classname, self._is_filters, self._not_filters)

    def _get_projectile_index_list(self):
        '''Returns a list of indexes the player owns for the projectile type'''
        return self.get_weapon_index_list(
            self._classname, self._is_filters, self._not_filters)

    def _get_projectile_ammo(self):
        '''Returns the ammo amount the player has for the projectile type'''
        return self._get_weapon_ammo(
            self._classname, self._is_filters, self._not_filters)

    def _set_projectile_ammo(self, value):
        '''Sets the ammo amount of the player for the projectile type'''
        self._set_weapon_ammo(
            value, self._classname, self._is_filters, self._not_filters)


class _HEGrenade(_ProjectileBase):
    '''Class used to interact with a player based on the hegrenade weapon'''
    _classname = 'weapon_hegrenade'


class _Flashbang(_ProjectileBase):
    '''Class used to interact with a player based on the flashbang weapon'''
    _classname = 'weapon_flashbang'


class _SmokeGrenade(_ProjectileBase):
    '''Class used to interact with a player based on the smokegrenade weapon'''
    _classname = 'weapon_smokegrenade'


class _Decoy(_ProjectileBase):
    '''Class used to interact with a player based on the decoy weapon'''
    _classname = 'weapon_decoy'


class _Incendiary(_ProjectileBase):
    '''Class used to interact with a player based on incendiary weapons'''
    _is_filters = 'incendiary'
