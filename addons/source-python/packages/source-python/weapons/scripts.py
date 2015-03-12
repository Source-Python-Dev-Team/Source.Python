# ../weapons/scripts.py

"""Provides weapon scripts functionalities."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Python Imports
#   Contexts
from contextlib import suppress

# Source.Python Imports
#   Core
from core import GameConfigObj
#   Paths
from paths import SP_DATA_PATH
#   Memory
from memory.manager import manager
#   Weapons
from weapons.constants import MuzzleFlashStyle
from weapons.constants import WeaponID
from weapons.constants import WeaponSlot
from weapons.constants import WeaponType


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('get_weapon_info',
           'WeaponInfo'
           )


# =============================================================================
# >> GLOBAL VARIABLES
# =============================================================================
# Get the WeaponInfo class...
_WeaponInfo = manager.create_type_from_dict('WeaponInfo',
    GameConfigObj(SP_DATA_PATH.joinpath('weapons', 'scripts').joinpath(
    'WeaponInfo.ini')))

# Get the _get_weapon_info function...
_get_weapon_info = manager.create_pipe_from_dict(
    GameConfigObj(SP_DATA_PATH.joinpath('weapons', 'scripts',
    'get_weapon_info.ini'))).get_weapon_info


# =============================================================================
# >> CLASSES
# =============================================================================
class WeaponInfo(_WeaponInfo):
    '''WeaponInfo extension class.'''

    @property
    def muzzle_flash_style(self):
        '''Return the muzzle flash style as a MuzzleFLashStyle instance.'''
        return MuzzleFlashStyle(self.muzzle_flash_style_index)

    @property
    def slot(self):
        '''Return the weapon slot as a WeaponSlot instance.'''
        return WeaponSlot(self.slot_index)

    @property
    def weapon_type(self):
        '''Return the weapon type index as a WeaponType instance.'''
        return WeaponType(self.weapon_type_index)

    @property
    def weapon_id(self):
        '''Return the WeaponID matching the instance class name.'''

        # Loop through all WeaponID values...
        for weapon_id in list(WeaponID):

            # Skip invalid identifiers suppressing ValueError...
            with suppress(ValueError):

                # Is the class names not matching?
                if self.class_name != get_weapon_info(weapon_id).class_name:

                    # If so, skip the current identifier...
                    continue

                # We found a match! Return the current identifier...
                return weapon_id

        # Unfortunately, we didn't find any match...
        return WeaponID.NONE

# Override the original class with our extension...
manager['WeaponInfo'] = WeaponInfo


# =============================================================================
# >> HELPER FUNCTIONS
# =============================================================================
def get_weapon_info(weapon_id):
    """Return the WeaponInfo instance from the given identifier."""

    # Get the WeaponInfo instance...
    weapon_info = _get_weapon_info(weapon_id)

    # Was the given identifier valid?
    if weapon_info:

        # If so, return the retrieved WeaponInfo instance...
        return weapon_info

    # The given identifier was not valid so raise an error...
    raise ValueError('"{0}" is not a valid identifier.'.format(weapon_id))
