# ../filters/weapons.py

"""Provides weapon filtering functionality."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
#   Entities
from entities import EntityGenerator
from entities.helpers import basehandle_from_edict
from entities.helpers import index_from_edict
from entities.helpers import inthandle_from_edict
from entities.helpers import pointer_from_edict
#   Filters
from filters.iterator import _IterObject
from filters.manager import _BaseFilterManager
#   Weapons
from weapons.manager import weapon_manager


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('WeaponClassIter',
           'WeaponEdictIter',
           )


# =============================================================================
# >> WEAPON EDICT ITER CLASSES
# =============================================================================
class _WeaponEdictIterManager(_BaseFilterManager):

    """Filter management class specifically for weapon iterating."""

# Get the _WeaponIterManager instance
_weapon_edict_iter_manager = _WeaponEdictIterManager()


class WeaponEdictIter(_IterObject):

    """Weapon iterate class."""

    # Store the manager for the weapon iterator
    manager = _weapon_edict_iter_manager

    @staticmethod
    def iterator():
        """Iterate over only weapon entities."""
        # Loop through all entities on the server
        for edict in EntityGenerator():

            # Is the entity a weapon?
            if edict.get_class_name() in weapon_manager:

                # Yield the entity
                yield edict


# =============================================================================
# >> WEAPON CLASS ITER CLASSES
# =============================================================================
class _WeaponClassIterManager(_BaseFilterManager):

    """Filter management class specifically for weapon tag iterating."""

# Get the _WeaponClassIterManager instance
_weapon_class_iter_manager = _WeaponClassIterManager()


class WeaponClassIter(_IterObject):

    """Weapon tag iterate class."""

    # Store the manager for the weapon tag iterator
    manager = _weapon_class_iter_manager

    def __init__(
            self, is_filters=None, not_filters=None, return_types='weapon'):
        """Call main __init__ with "weapon" as the default return_types."""
        super(WeaponClassIter, self).__init__(
            is_filters, not_filters, return_types)

    @staticmethod
    def iterator():
        """Iterate over all possible weapon types."""
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

    def _edict_weapon_contains_tag(self, edict):
        """Return whether the weapon contains the tag."""
        return self.tag in weapon_manager[edict.get_class_name()].tags

    def _class_weapon_contains_tag(self, weapon):
        """Return whether the given weapon contains the tag."""
        return self.tag in weapon.tags


# =============================================================================
# >> FILTER REGISTRATION
# =============================================================================
# Loop through all tags for the current game
for _tag in weapon_manager.tags:

    # Get the _weapon_tags instance for the current tag
    _instance = _weapon_tags[_tag]

    # Register the tag's filter for _weapon_edict_iter_manager
    _weapon_edict_iter_manager.register_filter(
        _tag, _instance._edict_weapon_contains_tag)

    # Register the tag's filter for _weapon_class_iter_manager
    _weapon_class_iter_manager.register_filter(
        _tag, _instance._class_weapon_contains_tag)


# =============================================================================
# >> WEAPON EDICT RETURN TYPES
# =============================================================================
def _return_instance(edict):
    """Return the weapon's BaseEntity instance."""
    from entities.entity import BaseEntity
    return BaseEntity(index_from_edict(edict), 'weapon')

# Register the return type functions
_weapon_edict_iter_manager.register_return_type('index', index_from_edict)
_weapon_edict_iter_manager.register_return_type(
    'basehandle', basehandle_from_edict)
_weapon_edict_iter_manager.register_return_type(
    'inthandle', inthandle_from_edict)
_weapon_edict_iter_manager.register_return_type(
    'pointer', pointer_from_edict)
_weapon_edict_iter_manager.register_return_type(
    'edict', lambda edict: edict)
_weapon_edict_iter_manager.register_return_type(
    'weapon', _return_instance)


# =============================================================================
# >> WEAPON CLASS RETURN TYPES
# =============================================================================
# Register the return type functions
_weapon_class_iter_manager.register_return_type(
    'weapon', lambda weapon: weapon)
_weapon_class_iter_manager.register_return_type(
    'classname', lambda weapon: weapon.name)
_weapon_class_iter_manager.register_return_type(
    'basename', lambda weapon: weapon.basename)
_weapon_class_iter_manager.register_return_type(
    'slot', lambda weapon: weapon.slot)
_weapon_class_iter_manager.register_return_type(
    'maxammo', lambda weapon: weapon.maxammo)
_weapon_class_iter_manager.register_return_type(
    'ammoprop', lambda weapon: weapon.ammoprop)
_weapon_class_iter_manager.register_return_type(
    'clip', lambda weapon: weapon.clip)
_weapon_class_iter_manager.register_return_type(
    'tags', lambda weapon: weapon.tags)
