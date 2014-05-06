# ../players/weapons/projectiles.py


# =============================================================================
# >> CLASSES
# =============================================================================
class _ProjectileMeta(type):
    ''''''

    def __new__(mcl, name, bases, odict):
        ''''''

        cls = super().__new__(mcl, name, bases, odict)

        if not bases:

            return cls

        method_name = name.strip('_').lower()

        setattr(
            cls, 'get_{0}_indexes'.format(method_name),
            lambda self: self._get_projectile_index_list())

        setattr(
            cls, 'get_{0}_count'.format(method_name),
            lambda self: self._get_projectile_ammo())

        setattr(
            cls, 'set_{0}_count'.format(method_name),
            lambda self, value: self._set_projectile_ammo(value))

        return cls


class _ProjectileBase(metaclass=_ProjectileMeta):
    ''''''

    # 
    _classname = None
    _is_filters = None
    _not_filters = None

    def _get_projectile_index_list(self):
        ''''''
        return self.get_weapon_index_list(
            self._classname, self._is_filters, self._not_filters)

    def _get_projectile_ammo(self):
        ''''''
        return self._get_weapon_ammo(
            self._classname, self._is_filters, self._not_filters)

    def _set_projectile_ammo(self, value):
        ''''''
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
    '''Class used to interact with a player based on the incendiary weapon'''
    _is_filters = 'incendiary'
