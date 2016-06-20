# ../weapons/dictionary.py

"""Provides helper class to store weapon instances."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
#   Entities
from entities.dictionary import EntityDictionary
#   Weapons
from weapons.entity import Weapon


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

    def __init__(self, weapon_class=Weapon, *args, **kwargs):
        """Initialize the dictionary."""
        super().__init__(weapon_class, *args, **kwargs)
