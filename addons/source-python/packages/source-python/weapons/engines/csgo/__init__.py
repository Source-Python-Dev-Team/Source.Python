# ../weapons/engines/csgo/__init__.py

"""Provides CS:GO specific weapon functionality."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Weapons
from weapons._base import Weapon as _Weapon
from weapons.manager import weapon_manager


# =============================================================================
# >> CLASSES
# =============================================================================
class Weapon(_Weapon):
    """Allows easy usage of the weapon's attributes."""
    def get_ammo(self):
        """Return the amount of ammo the player has for the weapon."""
        # Is the weapon not a grenade?
        if 'grenade' not in weapon_manager[self.classname].tags:
            return self.primary_ammo_count

        player = self._validate_ammo()
        return player.get_property_ushort(
            '{base}{prop:03d}'.format(
                base=weapon_manager.ammoprop,
                prop=self.ammoprop,
            )
        )


    def set_ammo(self, value):
        """Set the player's ammo property for the weapon."""
        # Is the weapon not a grenade?
        if 'grenade' not in weapon_manager[self.classname].tags:
            self.primary_ammo_count = value
            return

        player = self._validate_ammo()
        player.set_property_ushort(
            '{base}{prop:03d}'.format(
                base=weapon_manager.ammoprop,
                prop=self.ammoprop,
            ),
            value,
        )


    # Set the "ammo" property methods
    ammo = property(
        get_ammo, set_ammo,
        doc="""Property to get/set the weapon's ammo.""")
