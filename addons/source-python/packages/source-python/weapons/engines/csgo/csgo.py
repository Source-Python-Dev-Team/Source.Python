# ../weapons/engines/csgo/csgo.py

"""Provides CS:GO game specific weapon functionality."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python
from . import Weapon as _Weapon
from weapons.manager import weapon_manager


# =============================================================================
# >> GLOBAL VARIABLES
# =============================================================================
_item_definition_indexes = {
    values.get('item_definition_index'): weapon_manager[weapon].name
    for weapon, values in weapon_manager.ini['weapons'].items()
    if values.get('item_definition_index')
}


# =============================================================================
# >> CLASSES
# =============================================================================
class Weapon(_Weapon):
    """Allows easy usage of the weapon's attributes."""

    # Instances of this class will be cached by default
    caching = True

    def get_ammo(self):
        """Return the amount of ammo the player has for the weapon.
        
        :rtype: int
        """
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

    @property
    def weapon_name(self):
        """Return the full class name of the weapon.
        
        :rtype: str
        """
        return _item_definition_indexes.get(
            self.item_definition_index,
            self.classname,
        )
