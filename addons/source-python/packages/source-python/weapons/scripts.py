# ../weapons/scripts.py

"""Provides weapon scripts functionalities."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
#   Core
from core import GameConfigObj
#   Paths
from paths import SP_DATA_PATH
#   Memory
from memory import NULL
from memory.manager import manager


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('BaseWeaponInfo',
           'get_weapon_info',
           'WeaponInfo'
           )


# =============================================================================
# >> GLOBAL VARIABLES
# =============================================================================
_path = SP_DATA_PATH.joinpath('weapons', 'scripts')

# Get the BaseWeaponInfo class...
BaseWeaponInfo = manager.create_type_from_dict(
    'BaseWeaponInfo',
    GameConfigObj(_path.joinpath('BaseWeaponInfo.ini')))

# Get the WeaponInfo class...
WeaponInfo = manager.create_type_from_dict(
    'WeaponInfo',
    GameConfigObj(_path.joinpath('WeaponInfo.ini')), (BaseWeaponInfo,))

# Get the _get_weapon_info function...
_get_weapon_info = manager.create_pipe_from_dict(
    GameConfigObj(SP_DATA_PATH.joinpath('weapons', 'scripts',
    'get_weapon_info.ini'))).get_weapon_info


# =============================================================================
# >> HELPER FUNCTIONS
# =============================================================================
def get_weapon_info(weapon_id):
    """Returns the WeaponInfo instance from the given identifier."""

    # Get the WeaponInfo instance...
    weapon_info = _get_weapon_info(weapon_id)

    # Was the given identifier not valid?
    if weapon_info == NULL:

        # Raise an error...
        raise ValueError('"{0}" is not a valid identifier.'.format(weapon_id))

    # Return the WeaponInfo instance...
    return weapon_info