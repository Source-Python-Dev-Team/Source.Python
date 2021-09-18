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

    def __init__(self, factory=Weapon, *args, **kwargs):
        """Initializes the dictionary.

        :param callable factory:
            Factory class or function used to create missing instances. Set to
            `None` to disable this feature.

            Factory signature: index, *args, **kwargs
        :param tuple args:
            Arguments passed to the factory class or function.
        :param dict kwargs:
            Keyword arguments passed to the factory class or function.
        """
        super().__init__(factory, *args, **kwargs)
