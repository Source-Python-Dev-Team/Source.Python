# ../weapons/entity.py

"""Provides simplified weapon functionality."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
#   Entities
from entities.entity import BaseEntity
from entities.helpers import index_from_inthandle
#   Weapons
from weapons.manager import weapon_manager


# =============================================================================
# >> GLOBAL VARIABLES
# =============================================================================
# Add all the global variables to __all__
__all__ = ('WeaponEntity',
           )


# =============================================================================
# >> CLASSES
# =============================================================================
class WeaponEntity(BaseEntity):

    """Allows easy usage of the weapon's attributes."""

    # =========================================================================
    # >> AMMO
    # =========================================================================
    def has_ammo(self):
        """Return whether or not the weapon has ammo."""
        return weapon_manager[self.classname].ammoprop is not None

    def get_ammo(self):
        """Return the amount of ammo the player has for the given weapon."""
        # Do we have a valid ammoprop?
        if not self.has_ammo():
            raise ValueError(
                'Unable to get ammoprop for {0}'.format(self.classname))

        # Get the index of the owner
        index = index_from_inthandle(self.owner)

        # Get the owner's entity instance
        player = BaseEntity(index)

        # Get the weapon
        weapon = weapon_manager[self.classname]

        # Return the ammo
        return player.get_property_int(
            'localdata.' + weapon_manager.ammoprop + '%03d' % weapon.ammoprop)

    def set_ammo(self, value):
        """Set the player's ammo property for the given weapon."""
        # Do we have a valid ammoprop?
        if not self.has_ammo():
            raise ValueError(
                'Unable to set ammoprop for {0}'.format(self.classname))

        # Get the index of the owner
        index = index_from_inthandle(self.owner)

        # Get the owner's entity instance
        player = BaseEntity(index)

        # Get the weapon
        weapon = weapon_manager[self.classname]

        # Set the ammo
        player.set_property_int(
            'localdata.' + weapon_manager.ammoprop + '%03d' % weapon.ammoprop,
            value)

    # Set the "ammo" property methods
    ammo = property(
        get_ammo, set_ammo,
        doc="""Property to get/set the weapon's ammo.""")

    # =========================================================================
    # >> CLIP
    # =========================================================================
    def get_clip(self):
        """
        Return the amount of ammo in the primary clip for the given weapon.
        """
        return self.get_property_int('m_iClip1')

    def set_clip(self, value):
        """Set the player's primary clip value for the given weapon."""
        self.set_property_int('m_iClip1', value)

    # Set the "clip" property methods
    clip = property(
        get_clip, set_clip,
        doc="""Property to get/set the weapon's primary clip.""")

    def get_clip2(self):
        """
        Return the amount of ammo in the secondary clip for the given weapon.
        """
        return self.get_property_int('m_iClip2')

    def set_clip2(self, value):
        """Set the player's secondary clip value for the given weapon."""
        self.set_property_int('m_iClip2', value)

    # Set the "clip2" property methods
    clip2 = property(
        get_clip2, set_clip2,
        doc="""Property to get/set the weapon's secondary clip.""")
