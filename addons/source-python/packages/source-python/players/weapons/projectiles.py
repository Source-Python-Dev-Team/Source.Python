# ../players/weapons/projectiles.py


# =============================================================================
# >> CLASSES
# =============================================================================
class _ProjectileMeta(type):
    '''Metaclass used to auto-create methods specific to the projectile'''

    def __new__(mcl, name, bases, odict):
        '''Create the class and create its methods dynamically'''

        # Store values to use later
        temp = {'_classname': None, '_is_filters': None, '_not_filters': None}

        # Loop through the base class attributes
        for attribute in ('_classname', '_is_filters', '_not_filters'):

            # Try to store the odict's value of
            # the attribute and delete it from odict
            try:
                temp[attribute] = odict[attribute]
                del odict[attribute]

            # If the attribute is not found in odict, just leave it as None
            except KeyError:
                continue

        # Create the object
        cls = super().__new__(mcl, name, bases, odict)

        # Is the the baseclass that uses the metaclass?
        if len(bases) != 1 or bases[0].__name__ != '_ProjectileBase':

            # Do not add any methods
            return cls

        # Get the weapon's name based off of the class name
        method_name = name.strip('_').lower()

        # Create the iterator <weapon>_indexes method
        setattr(
            cls, '{0}_indexes'.format(method_name),
            property(lambda self: cls._projectile_indexes(
            self, temp['_classname'],
            temp['_is_filters'], temp['_not_filters'])))

        # Create the get_<weapon>_indexes method
        setattr(
            cls, 'get_{0}_indexes'.format(method_name),
            lambda self: cls._get_projectile_index_list(
            self, temp['_classname'],
            temp['_is_filters'], temp['_not_filters']))

        # Create the get_<weapon>_count method
        setattr(
            cls, 'get_{0}_count'.format(method_name),
            lambda self: cls._get_projectile_ammo(
            self, temp['_classname'],
            temp['_is_filters'], temp['_not_filters']))

        # Create the set_<weapon>_count method
        setattr(
            cls, 'set_{0}_count'.format(method_name),
            lambda self, value: cls._set_projectile_ammo(
            self, value, temp['_classname'],
            temp['_is_filters'], temp['_not_filters']))

        # Return the new class
        return cls


class _ProjectileBase(metaclass=_ProjectileMeta):
    '''Base Projectile class used to interact with player projectiles'''

    # Store the base attributes all as None
    _classname = None
    _is_filters = None
    _not_filters = None

    def _projectile_indexes(self, classname, is_filters, not_filters):
        '''Iterates over all indexes the player owns for the projectile type'''
        return self.weapon_indexes(classname, is_filters, not_filters)

    def _get_projectile_index_list(self, classname, is_filters, not_filters):
        '''Returns a list of indexes the player owns for the projectile type'''
        return self.get_weapon_index_list(classname, is_filters, not_filters)

    def _get_projectile_ammo(self, classname, is_filters, not_filters):
        '''Returns the ammo amount the player has for the projectile type'''
        return self._get_weapon_ammo(classname, is_filters, not_filters)

    def _set_projectile_ammo(self, value, classname, is_filters, not_filters):
        '''Sets the ammo amount of the player for the projectile type'''
        self._set_weapon_ammo(value, classname, is_filters, not_filters)


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
