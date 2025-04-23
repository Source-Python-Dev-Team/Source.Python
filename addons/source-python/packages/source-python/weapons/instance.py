# ../weapons/instance.py

"""Provides a class to store information specific to each weapon."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
#   Cvars
from cvars import ConVar
#   Weapons
from weapons.constants import WeaponID
from weapons.constants import WeaponSlot
from weapons.constants import WeaponType


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('WeaponClass',
           )


# =============================================================================
# >> CLASSES
# =============================================================================
class WeaponClass(object):
    """Class used to store information specific to the given weapon."""

    def __init__(self, name, basename, properties):
        """Store the base attributes for the weapon."""
        # Store the weapon's full name
        self._name = name

        # Store the weapon's base name
        self._basename = basename

        # Store the weapon's id number
        self._id = self.parse_weapon_constants(
            properties.get('id', None), WeaponID)

        # Store the weapon's type number
        self._type = self.parse_weapon_constants(
            properties.get('type', None), WeaponType)

        # Store the weapon's slot number
        self._slot = self.parse_weapon_constants(
            properties.get('slot', None), WeaponSlot)

        # Store the weapon's max ammo amount
        self._maxammo = properties.get('maxammo', None)

        # Store the weapon's ammo property
        self._ammoprop = properties.get('ammoprop', None)

        # Store the weapon's clip amount
        self._clip = properties.get('clip', None)

        # Store the weapon's cost
        self._cost = properties.get('cost', None)

        # Store the weapon's parent_class
        self._parent_class = properties.get('parent_class', None)

        # Store the weapon's tags
        self._tags = properties.get('tags', 'all').split(',')

    @property
    def name(self):
        """Return the entity classname of the weapon (e.g. 'weapon_knife').

        :rtype: str
        """
        return self._name

    @property
    def basename(self):
        """Return the basename of the weapon (e.g. 'knife').

        :rtype: str
        """
        return self._basename

    @property
    def id(self):
        """Return the id of the weapon.

        :rtype: int/WeaponID
        """
        return self._id

    @property
    def type(self):
        """Return the type of the weapon.

        :rtype: int/WeaponType
        """
        return self._type

    @property
    def slot(self):
        """Return the slot of the weapon.

        :rtype: int/WeaponSlot
        """
        return self._slot

    @property
    def maxammo(self):
        """Return the maxammo amount for the weapon.

        :return:
            None if the weapon doesn't have ammo.
        :rtype: int
        """
        # Is the stored maxammo an integer?
        if self._maxammo is None or isinstance(self._maxammo, int):

            # Return the value
            return self._maxammo

        # Get the cvar value of the maxammo
        return ConVar(self._maxammo).get_int()

    @property
    def ammoprop(self):
        """Return the ammoprop of the weapon.

        :return:
            None if the weapon doesn't have ammo.
        :rtype: int
        """
        return self._ammoprop

    @property
    def clip(self):
        """Return the clip value of the weapon.

        :return:
            None if the weapon doesn't have ammo.
        :rtype: int
        """
        return self._clip

    @property
    def cost(self):
        """Return the cost of the weapon.

        :return:
            None if the cost data is missing.
        :rtype: int
        """
        return self._cost

    @property
    def parent_class(self):
        """Return the parent class of the weapon.

        :return:
            None if the parent class data is missing.
        :rtype: str
        """
        return self._parent_class

    @property
    def item_definition_index(self):
        """Return the item definition index of the weapon.

        :return:
            None if the item defition index data is missing.
        :rtype: int
        """
        return int(self._id) if self._id is not None else None

    @property
    def tags(self):
        """Return the tags of the weapon.

        :rtype: list
        """
        return self._tags

    @staticmethod
    def parse_weapon_constants(value, weapon_constants):
        """Parses weapon constants.

        :return:
            Returns the value as it is if weapon constants cannot be found.
        """
        if isinstance(value, int):
            try:
                return weapon_constants(value)
            except (ValueError, TypeError):
                pass
        elif isinstance(value, str):
            try:
                return getattr(weapon_constants, value)
            except AttributeError:
                pass

        return value
