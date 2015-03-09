# ../weapons/scripts.py

"""Provides weapon scripts functionalities."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
#   Core
from core import SOURCE_ENGINE
#   Paths
from paths import SP_DATA_PATH
#   Memory
from memory import make_object
from memory.manager import manager
#   Weapons
from weapons.constants import WeaponID
from weapons.constants import WeaponSound


# =============================================================================
# >> FORWARD IMPORTS
# =============================================================================
# Source.Python Imports
#   Weapons
from _weapons._scripts import BaseWeaponInfo
from _weapons._scripts import WeaponInfo


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('BaseWeaponInfo',
           'WeaponInfo',
           'weapon_scripts'
           )


# =============================================================================
# >> GLOBAL VARIABLES
# =============================================================================
_pipe = manager.create_pipe_from_file(SP_DATA_PATH.joinpath('weapons',
    'scripts', SOURCE_ENGINE + '.ini'))

weapon_scripts = dict()
for weapon_id in WeaponID.values:
    weapon_info = make_object(WeaponInfo, _pipe.get_weapon_info(weapon_id))
    if weapon_info is None:
        continue
    weapon_scripts[weapon_info.class_name] = weapon_info


# =============================================================================
# >> HELPER FUNCTIONS
# =============================================================================
def get_weapon_info(weapon_id):
    return make_object(WeaponInfo, _pipe.get_weapon_info(weapon_id))