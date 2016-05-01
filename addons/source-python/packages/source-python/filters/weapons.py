# ../filters/weapons.py

"""Provides weapon filtering functionality."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
#   Entities
from entities import EntityGenerator
from entities.helpers import index_from_edict
#   Filters
from filters.iterator import _IterObject
#   Weapons
from weapons.default import NoWeaponManager
from weapons.manager import weapon_manager


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('WeaponClassIter',
           'WeaponIter',
           )


# =============================================================================
# >> WEAPON ITERATION CLASSES
# =============================================================================
class WeaponIter(_IterObject):
    """Weapon iterate class."""

    @staticmethod
    def iterator():
        """Iterate over all :class:`weapons.entity.Weapon` objects."""
        # Import the Weapon class.
        # This is done here to avoid circular imports
        from weapons.entity import Weapon

        # Loop through all entities on the server
        for edict in EntityGenerator():

            # Is the entity a weapon?
            if edict.classname in weapon_manager:

                # Yield the Weapon instance for the current edict
                yield Weapon(index_from_edict(edict))


class WeaponClassIter(_IterObject):
    """Weapon tag iterate class."""

    @staticmethod
    def iterator():
        """Iterate over all :class:`weapons.instance.WeaponClass` objects."""
        # Loop through all weapons for the current game
        for weapon in weapon_manager:

            # Yield the weapon
            yield weapon_manager[weapon]


# =============================================================================
# >> WEAPON TAG CLASSES
# =============================================================================
class _WeaponTags(dict):
    """Class used to store weapon tags for the current game."""

    def __missing__(self, item):
        """Called when a new item is added to the dictionary."""
        # Get the _Tag instance for the current item
        instance = self[item] = _Tag(item)

        # Return the instance
        return instance

# Get the _WeaponTags instance
_weapon_tags = _WeaponTags()


class _Tag(object):
    """Class used to store a tag and compare to a given weapon."""

    def __init__(self, tag):
        """Store the tag for future use."""
        self.tag = tag

    def _tag_exists_for_weapon(self, weapon):
        """Return whether the weapon contains the tag."""
        return self.tag in weapon_manager[weapon.classname].tags

    def _tag_exists_for_weapon_class(self, weapon):
        """Return whether the given weapon contains the tag."""
        return self.tag in weapon.tags


# =============================================================================
# >> FILTER REGISTRATION
# =============================================================================
# Is the game supported?
if not isinstance(weapon_manager, NoWeaponManager):

    # Loop through all tags for the current game
    for _tag in weapon_manager.tags:

        # Get the _weapon_tags instance for the current tag
        _instance = _weapon_tags[_tag]

        # Register the tag's filter for WeaponIter
        WeaponIter.register_filter(
            _tag, _instance._tag_exists_for_weapon)

        # Register the tag's filter for WeaponClassIter
        WeaponClassIter.register_filter(
            _tag, _instance._tag_exists_for_weapon_class)
