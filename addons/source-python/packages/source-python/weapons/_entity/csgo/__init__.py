# ../weapons/_entity/__init__.py

"""Provides CS:GO specific weapon functionality."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Weapons
from weapons._entity import Weapon as _Weapon
from weapons.manager import weapon_manager


# =============================================================================
# >> CLASSES
# =============================================================================
class Weapon(_Weapon):
    """Allows easy usage of the weapon's attributes."""
    def get_ammo(self):
        """Return the amount of ammo the player has for the weapon."""
        # Is the weapon a projectile?
        if 'grenade' in weapon_manager[self.classname].tags:
            return super(Weapon, self).get_ammo()

        return self.primary_ammo_count

    def set_ammo(self, value):
        """Set the player's ammo property for the weapon."""
        # Is the weapon a projectile?
        if 'grenade' in weapon_manager[self.classname].tags:
            super(Weapon, self).set_ammo(value)
            return

        self.primary_ammo_count = value

    # Set the "ammo" property methods
    ammo = property(
        get_ammo, set_ammo,
        doc="""Property to get/set the weapon's ammo.""")