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


# =============================================================================
# >> CLASSES
# =============================================================================
class _ProjectileMeta(BaseEntity.__class__):

    """Metaclass used to auto-create methods specific to the projectile."""

    def __new__(cls, name, bases, odict):
        """Create the class and create its methods dynamically."""
        # Store values to use later
        temp = {'_classname': None, '_is_filters': None, '_not_filters': None}

        # Loop through the base class attributes
        for attribute in ('_classname', '_is_filters', '_not_filters'):

            # Try to store the odict's value of
            # the attribute and delete it from odict
            with suppress(KeyError):
                temp[attribute] = odict[attribute]
                del odict[attribute]

        # Create the object
        cls = super(_ProjectileMeta, cls).__new__(cls, name, bases, odict)

        # Is the the baseclass that uses the metaclass?
        if len(bases) != 1 or bases[0].__name__ != '_ProjectileBase':

            # Do not add any methods
            return cls

        # Get the weapon's name based off of the class name
        method_name = name.strip('_').lower()

        # Create the iterator <weapon>_indexes method and set its docstring
        setattr(
            cls, '{0}_indexes'.format(method_name),
            property(lambda self: cls._projectile_indexes(
                self, temp['_classname'],
                temp['_is_filters'], temp['_not_filters']), doc='Returns ' +
                'a generator of {0} indexes the player owns.'.format(
                    method_name)))

        # Create the get_<weapon>_indexes method
        setattr(
            cls, 'get_{0}_indexes'.format(method_name),
            lambda self: cls._get_projectile_index_list(
                self, temp['_classname'],
                temp['_is_filters'], temp['_not_filters']))

        # Set the docstring for the method
        getattr(
            cls, 'get_{0}_indexes'.format(method_name)).__doc__ = (
            'Returns a list of {0} indexes the player owns.'.format(
                method_name))

        # Create the get_<weapon>_count method
        setattr(
            cls, 'get_{0}_count'.format(method_name),
            lambda self: cls._get_projectile_ammo(
                self, temp['_classname'],
                temp['_is_filters'], temp['_not_filters']))

        # Set the docstring for the method
        getattr(
            cls, 'get_{0}_count'.format(method_name)).__doc__ = (
            "Returns the player's {0} ammo amount.".format(method_name))

        # Create the set_<weapon>_count method
        setattr(
            cls, 'set_{0}_count'.format(method_name),
            lambda self, value: cls._set_projectile_ammo(
                self, value, temp['_classname'],
                temp['_is_filters'], temp['_not_filters']))

        # Set the docstring for the method
        getattr(
            cls, 'set_{0}_count'.format(method_name)).__doc__ = (
            "Sets the player's {0} ammo amount to the given value.".format(
                method_name))

        # Return the new class
        return cls


class _ProjectileBase(metaclass=_ProjectileMeta):

    """Base Projectile class used to interact with player projectiles."""

    # Store the base attributes all as None
    _classname = None
    _is_filters = None
    _not_filters = None

    def _projectile_indexes(self, classname, is_filters, not_filters):
        """Iterate over all indexes the player owns for the projectile type."""
        return self.weapon_indexes(classname, is_filters, not_filters)

    def _get_projectile_index_list(self, classname, is_filters, not_filters):
        """Return a list of indexes the player owns for the projectile type."""
        return self.get_weapon_index_list(classname, is_filters, not_filters)

    def _get_projectile_ammo(self, classname, is_filters, not_filters):
        """Return the ammo amount the player has for the projectile type."""
        return self._get_weapon_ammo(classname, is_filters, not_filters)

    def _set_projectile_ammo(self, value, classname, is_filters, not_filters):
        """Set the ammo amount of the player for the projectile type."""
        self._set_weapon_ammo(value, classname, is_filters, not_filters)


class _HEGrenade(_ProjectileBase):

    """Class that interacts with a player based on the hegrenade weapon."""

    _classname = 'weapon_hegrenade'


class _Flashbang(_ProjectileBase):

    """Class that interacts with a player based on the flashbang weapon."""

    _classname = 'weapon_flashbang'


class _SmokeGrenade(_ProjectileBase):

    """Class that interacts with a player based on the smokegrenade weapon."""

    _classname = 'weapon_smokegrenade'


class _Decoy(_ProjectileBase):

    """Class that interacts with a player based on the decoy weapon."""

    _classname = 'weapon_decoy'


class _Incendiary(_ProjectileBase):

    """Class that interacts with a player based on incendiary weapons."""

    _is_filters = 'incendiary'
