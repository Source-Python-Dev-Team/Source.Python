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
        self._maxammo = properties.get('maxammo', 0)

        # Store the weapon's ammo property
        self._ammoprop = properties.get('ammoprop', None)

        # Store the weapon's clip amount
        self._clip = properties.get('clip', 0)

        # Store the weapon's tags
        self._tags = properties.get('tags', 'all').split(',')

    @property
    def name(self):
        """Return the classname of the weapon."""
        return self._name

    @property
    def basename(self):
        """Return the basename of the weapon."""
        return self._basename

    @property
    def slot(self):
        """Return the slot of the weapon."""
        return self._slot

    @property
    def maxammo(self):
        """Return the maxammo amount for the weapon."""
        # Is the stored maxammo an integer?
        if isinstance(self._maxammo, int):

            # Return the value
            return self._maxammo

        # Get the cvar value of the maxammo
        return ConVar(self._maxammo).get_int()

    @property
    def ammoprop(self):
        """Return the ammoprop of the weapon."""
        return self._ammoprop

    @property
    def clip(self):
        """Return the clip value of the weapon."""
        return self._clip

    @property
    def tags(self):
        """Return the tags of the weapon."""
        return self._tags
