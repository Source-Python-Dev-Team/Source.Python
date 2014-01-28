# ../filters/weapons.py

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
from entity_c import EntityGenerator
from public import public
#   Entities
from entities.entity import BaseEntity
from entities.helpers import basehandle_from_edict
from entities.helpers import index_from_edict
from entities.helpers import inthandle_from_edict
from entities.helpers import pointer_from_edict
#   Filters
from filters.iterator import _IterObject
from filters.manager import _BaseFilterManager
#   Weapons
from weapons.manager import WeaponManager


# =============================================================================
# >> WEAPON EDICT ITER CLASSES
# =============================================================================
class _WeaponEdictIterManager(_BaseFilterManager):
    '''Filter management class specifically for weapon iterating'''

# Get the _WeaponIterManager instance
WeaponEdictIterManager = _WeaponEdictIterManager()


@public
class WeaponEdictIter(_IterObject):
    '''Weapon iterate class'''

    # Store the manager for the weapon iterator
    manager = WeaponEdictIterManager

    @staticmethod
    def iterator():
        '''Iterates over only weapon entities'''

        # Loop through all entities on the server
        for edict in EntityGenerator():

            # Is the entity a weapon?
            if edict.get_class_name() in WeaponManager:

                # Yield the entity
                yield edict


# =============================================================================
# >> WEAPON CLASS ITER CLASSES
# =============================================================================
class _WeaponClassIterManager(_BaseFilterManager):
    '''Filter management class specifically for weapon tag iterating'''

# Get the _WeaponClassIterManager instance
WeaponClassIterManager = _WeaponClassIterManager()


@public
class WeaponClassIter(_IterObject):
    '''Weapon tag iterate class'''

    # Store the manager for the weapon tag iterator
    manager = WeaponClassIterManager

    def __init__(self, is_filters=[], not_filters=[], return_types='weapon'):
        '''Overwrite the __init__ method to re-call main __init__
            method with "weapon" as the default return_types'''

        # Re-call __init__ in case no return_types were actually passed
        # since "weapon" is the default value for this iter class
        super(WeaponClassIter, self).__init__(
            is_filters, not_filters, return_types)

    @staticmethod
    def iterator():
        '''Iterates over all possible weapon types'''

        # Loop through all weapons for the current game
        for weapon in WeaponManager:

            # Yield the weapon
            yield WeaponManager[weapon]


# =============================================================================
# >> WEAPON TAG CLASSES
# =============================================================================
class _WeaponTags(dict):
    '''Class used to store weapon tags for the current game'''

    def __missing__(self, item):
        '''Called when a new item is added to the dictionary'''

        # Get the _Tag instance for the current item
        instance = self[item] = _Tag(item)

        # Return the instance
        return instance

# Get the _WeaponTags instance
WeaponTags = _WeaponTags()


class _Tag(object):
    '''Class used to store a tag and compare to a given weapon'''

    def __init__(self, tag):
        '''Store the tag for future use'''
        self.tag = tag

    def _edict_weapon_contains_tag(self, edict):
        '''Returns whether the weapon contains the tag'''
        return self.tag in WeaponManager[edict.get_class_name()].tags

    def _class_weapon_contains_tag(self, weapon):
        '''Returns whether the given weapon contains the tag'''
        return self.tag in weapon.tags


# =============================================================================
# >> FILTER REGISTRATION
# =============================================================================
# Loop through all tags for the current game
for tag in WeaponManager.tags:

    # Get the WeaponTags instance for the current tag
    instance = WeaponTags[tag]

    # Register the tag's filter for WeaponEdictIterManager
    WeaponEdictIterManager.register_filter(
        tag, instance._edict_weapon_contains_tag)

    # Register the tag's filter for WeaponClassIterManager
    WeaponClassIterManager.register_filter(
        tag, instance._class_weapon_contains_tag)


# =============================================================================
# >> WEAPON EDICT RETURN TYPE FUNCTIONS
# =============================================================================
def _return_edict(edict):
    '''Returns the weapon's edict'''
    return edict


def _return_instance(edict):
    '''Returns the weapon's BaseEntity instance'''
    return BaseEntity(index_from_edict(edict), 'weapon')

# Register the return type functions
WeaponEdictIterManager.register_return_type('index', index_from_edict)
WeaponEdictIterManager.register_return_type(
    'basehandle', basehandle_from_edict)
WeaponEdictIterManager.register_return_type('inthandle', inthandle_from_edict)
WeaponEdictIterManager.register_return_type('pointer', pointer_from_edict)
WeaponEdictIterManager.register_return_type('edict', _return_edict)
WeaponEdictIterManager.register_return_type('weapon', _return_instance)


# =============================================================================
# >> WEAPON CLASS RETURN TYPE FUNCTIONS
# =============================================================================
def _return_weapon(weapon):
    '''Returns the weapon type's WeaponManager item'''
    return weapon


def _return_classname(weapon):
    '''Returns the weapon type's classname'''
    return weapon.name


def _return_basename(weapon):
    '''Returns the weapon type's basename'''
    return weapon.basename


def _return_slot(weapon):
    '''Returns the weapon type's slot'''
    return weapon.slot


def _return_maxammo(weapon):
    '''Returns the weapon type's maxammo'''
    return weapon.maxammo


def _return_ammoprop(weapon):
    '''Returns the weapon type's ammoprop'''
    return weapon.ammoprop


def _return_clip(weapon):
    '''Returns the weapon type's clip'''
    return weapon.clip


def _return_tags(weapon):
    '''Returns the weapon type's tags'''
    return weapon.tags

# Register the return type functions
WeaponClassIterManager.register_return_type('weapon', _return_weapon)
WeaponClassIterManager.register_return_type('classname', _return_classname)
WeaponClassIterManager.register_return_type('basename', _return_basename)
WeaponClassIterManager.register_return_type('slot', _return_slot)
WeaponClassIterManager.register_return_type('maxammo', _return_maxammo)
WeaponClassIterManager.register_return_type('ammoprop', _return_ammoprop)
WeaponClassIterManager.register_return_type('clip', _return_clip)
WeaponClassIterManager.register_return_type('tags', _return_tags)
