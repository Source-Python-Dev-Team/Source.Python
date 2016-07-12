# ../weapons/_entity/__init__.py

"""Provides simplified weapon functionality."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
#   Entities
from entities.entity import Entity
#   Weapons
from weapons.manager import weapon_manager


# =============================================================================
# >> GLOBAL VARIABLES
# =============================================================================
# Add all the global variables to __all__
__all__ = ('Weapon',
           )


# =============================================================================
# >> CLASSES
# =============================================================================
class Weapon(Entity):
    """Allows easy usage of the weapon's attributes."""

    def get_clip(self):
        """Return the amount of ammo in the weapon's clip."""
        return self._clip if self._clip != -1 else 0

    def set_clip(self, value):
        """Set the amount of ammo in the weapon's clip."""
        # Does the weapon have ammo?
        if self._clip != -1:

            # Set the clip amount
            self._clip = value

    # Set the "clip" property methods
    clip = property(
        get_clip, set_clip,
        doc="""Property to get/set the weapon's clip.""")

    def get_ammo(self):
        """Return the amount of ammo the player has for the weapon."""
        # Do we have a valid ammoprop?
        if self.ammoprop == -1:
            raise ValueError(
                'Unable to get ammoprop for {0}'.format(self.classname))

        # Get the owner of the weapon
        player = self.owner

        # Does no one currently own the weapon?
        if player is None:
            return

        # Return the ammo
        return player.get_property_int(
            weapon_manager.ammoprop + '%03d' % self.ammoprop)

    def set_ammo(self, value):
        """Set the player's ammo property for the weapon."""
        # Do we have a valid ammoprop?
        if self.ammoprop == -1:
            raise ValueError(
                'Unable to set ammoprop for {0}'.format(self.classname))

        # Get the owner of the weapon
        player = self.owner

        # Does no one currently own the weapon?
        if player is None:
            return

        # Set the ammo
        player.set_property_int(
            weapon_manager.ammoprop + '%03d' % self.ammoprop, value)

    # Set the "ammo" property methods
    ammo = property(
        get_ammo, set_ammo,
        doc="""Property to get/set the weapon's ammo.""")

    def get_secondary_fire_clip(self):
        """Return the amount of ammo in the weapon's secondary fire clip."""
        # Does the weapon have secondary fire ammo?
        if self._secondary_fire_clip != -1:

            # Return the secondary fire clip amount
            return self._secondary_fire_clip

        # If it doesn't, return zero
        return 0

    def set_secondary_fire_clip(self, value):
        """Set the amount of ammo in the weapon's secondary fire clip."""
        # Does the weapon have secondary fire ammo?
        if self._secondary_fire_clip != -1:

            # Set the secondary fire clip amount
            self._secondary_fire_clip = value

    # Set the "secondary_fire_clip" property methods
    secondary_fire_clip = property(
        get_secondary_fire_clip, set_secondary_fire_clip,
        doc="""Property to get/set the weapon's secondary fire clip.""")

    def get_secondary_fire_ammo(self):
        """Return the secondary fire ammo the player has for the weapon."""
        # Do we have a valid ammoprop?
        if self.secondary_fire_ammoprop == -1:
            raise ValueError(
                'Unable to get secondary fire ammoprop for {0}'.format(
                    self.classname))

        # Get the owner of the weapon
        player = self.owner

        # Does no one currently own the weapon?
        if player is None:
            return

        # Return the ammo
        return player.get_property_int(
            weapon_manager.ammoprop + '%03d' % self.secondary_fire_ammoprop)

    def set_secondary_fire_ammo(self, value):
        """Set the player's secondary fire ammo property for the weapon."""
        # Do we have a valid ammoprop?
        if self.secondary_fire_ammoprop == -1:
            raise ValueError(
                'Unable to set secondary fire ammoprop for {0}'.format(
                    self.classname))

        # Get the owner of the weapon
        player = self.owner

        # Does no one currently own the weapon?
        if player is None:
            return

        # Set the ammo
        player.set_property_int(
            weapon_manager.ammoprop +
            '%03d' % self.secondary_fire_ammoprop, value)

    # Set the "secondary_fire_ammo" property methods
    secondary_fire_ammo = property(
        get_secondary_fire_ammo, set_secondary_fire_ammo,
        doc="""Property to get/set the weapon's secondary fire ammo.""")
