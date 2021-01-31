# ../weapons/instance.py

"""Provides a class to store information specific to each weapon."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
#   Cvars
from cvars import ConVar


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

        # Store the weapon's slot number
        self._slot = properties.get('slot', None)

        # Store the weapon's max ammo amount
        self._maxammo = properties.get('maxammo', None)

        # Store the weapon's ammo property
        self._ammoprop = properties.get('ammoprop', None)

        # Store the weapon's clip amount
        self._clip = properties.get('clip', None)

        # Store the weapon's cost
        self._cost = properties.get('cost', None)

        # Store the weapon's item definition index
        self._item_definition_index = properties.get('item_definition_index', None)

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
    def slot(self):
        """Return the slot of the weapon.

        :rtype: int
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
    def item_definition_index(self):
        """Return the item definition index of the weapon.

        :return:
            None if the item defition index data is missing.
        :rtype: int
        """
        return self._item_definition_index

    @property
    def tags(self):
        """Return the tags of the weapon.

        :rtype: list
        """
        return self._tags
