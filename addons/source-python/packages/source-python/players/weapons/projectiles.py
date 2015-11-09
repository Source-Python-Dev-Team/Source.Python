# ../players/weapons/projectiles.py

"""Provides player projectile weapon functionality."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Python Imports
#   Contextlib
from contextlib import suppress
#   Entities
from entities.entity import BaseEntity
#   Filters
from filters.weapons import WeaponClassIter
#   Weapons
from weapons.manager import weapon_manager


# =============================================================================
# >> CLASSES
# =============================================================================
class _ProjectileMeta(BaseEntity.__class__):
    """Metaclass used to auto-create methods specific to the projectile."""

    def __new__(cls, name, bases, odict):
        """Create the class and create its methods dynamically."""
        # Store values to use later
        temp = {'_classname': None, '_filters': None}

        # Loop through the base class attributes
        for attribute in ('_classname', '_filters'):

            # Try to store the odict's value of
            # the attribute and delete it from odict
            with suppress(KeyError):
                temp[attribute] = odict[attribute]
                del odict[attribute]

        # Create the object
        cls = super().__new__(cls, name, bases, odict)

        # Is the the baseclass that uses the metaclass?
        if len(bases) != 1 or bases[0].__name__ != '_ProjectileBase':

            # Do not add any methods
            return cls

        # Create the iterator <weapon>_indexes method and set its docstring
        setattr(
            cls, '{0}_indexes'.format(cls._name),
            property(lambda self: cls._projectile_indexes(
                self, temp['_classname'], temp['_filters']), doc='Returns a ' +
                'generator of {0} indexes the player owns.'.format(cls._name)))

        # Create the get_<weapon>_ammo method
        setattr(
            cls, 'get_{0}_ammo'.format(cls._name),
            lambda self: cls._get_projectile_ammo(
                self, temp['_classname'], temp['_filters']))

        # Set the docstring for the method
        getattr(
            cls, 'get_{0}_ammo'.format(cls._name)).__doc__ = (
            "Returns the player's {0} ammo amount.".format(cls._name))

        # Create the set_<weapon>_ammo method
        setattr(
            cls, 'set_{0}_ammo'.format(cls._name),
            lambda self, value: cls._set_projectile_ammo(
                self, value, temp['_classname'], temp['_filters']))

        # Set the docstring for the method
        getattr(
            cls, 'set_{0}_ammo'.format(cls._name)).__doc__ = (
            "Sets the player's {0} ammo amount to the given value.".format(
                cls._name))

        # Return the new class
        return cls


class _ProjectileBase(metaclass=_ProjectileMeta):
    """Base Projectile class used to interact with player projectiles."""

    # Store the base attributes all as None
    _classname = None
    _filters = None

    def _projectile_indexes(self, classname, filters):
        """Iterate over all indexes the player owns for the projectile type."""
        return self.weapon_indexes(classname, filters)

    def _get_projectile_ammo(self, classname, filters):
        """Return the ammo amount the player has for the projectile type."""
        if classname is None:
            weapon = [weapon for weapon in WeaponClassIter(filters)][0]
        else:
            weapon = weapon_manager[classname]
        return self.get_property_int(
            weapon_manager.ammoprop + '%03d' % weapon.ammoprop)

    def _set_projectile_ammo(self, value, classname, filters):
        """Set the ammo amount of the player for the projectile type."""
        if classname is None:
            weapon = [weapon for weapon in WeaponClassIter(filters)][0]
        else:
            weapon = weapon_manager[classname]
        self.set_property_int(
            weapon_manager.ammoprop + '%03d' % weapon.ammoprop, value)


class _HEGrenade(_ProjectileBase):
    """Class that interacts with a player based on the hegrenade weapon."""

    _name = 'hegrenade'
    _classname = 'weapon_hegrenade'


class _Flashbang(_ProjectileBase):
    """Class that interacts with a player based on the flashbang weapon."""

    _name = 'flashbang'
    _classname = 'weapon_flashbang'


class _SmokeGrenade(_ProjectileBase):
    """Class that interacts with a player based on the smokegrenade weapon."""

    _name = 'smokegrenade'
    _classname = 'weapon_smokegrenade'


class _Decoy(_ProjectileBase):
    """Class that interacts with a player based on the decoy weapon."""

    _name = 'decoy'
    _classname = 'weapon_decoy'


class _Incendiary(_ProjectileBase):
    """Class that interacts with a player based on incendiary weapons."""

    _name = 'incendiary'
    _filters = 'incendiary'
