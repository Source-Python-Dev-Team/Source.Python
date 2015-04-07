# ../weapons/dictionary.py

"""Provides helper class to store weapon instances."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
#   Entities
from entities.dictionary import EntityDictionary
#   Weapons
from weapons.entity import WeaponEntity


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('WeaponDictionary',
           )


# ============================================================================
# >> CLASSES
# ============================================================================
class WeaponDictionary(EntityDictionary):

    """Helper class used to store weapon instances."""

    def __init__(self, weapon_class=WeaponEntity):
        """Initialize the dictionary."""
        super(WeaponDictionary, self).__init__(weapon_class)
