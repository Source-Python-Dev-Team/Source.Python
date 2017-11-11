# ../weapons/constants.py

"""Provides constant values that are weapons based."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Python Imports
#   Enum
from enum import IntEnum
from enum import IntFlag

# Source.Python Imports
#   Core
from core import GameConfigObj
#   Paths
from paths import SP_DATA_PATH


# =============================================================================
# >> FORWARD IMPORTS
# =============================================================================
# Source.Python Imports
#   Weapons
from _weapons._constants import ITEM_FLAG_SELECTONEMPTY
from _weapons._constants import ITEM_FLAG_NOAUTORELOAD
from _weapons._constants import ITEM_FLAG_NOAUTOSWITCHEMPTY
from _weapons._constants import ITEM_FLAG_LIMITINWORLD
from _weapons._constants import ITEM_FLAG_EXHAUSTIBLE
from _weapons._constants import ITEM_FLAG_DOHITLOCATIONDMG
from _weapons._constants import ITEM_FLAG_NOAMMOPICKUPS
from _weapons._constants import ITEM_FLAG_NOITEMPICKUP
from _weapons._constants import WEAPON_NOT_CARRIED
from _weapons._constants import WEAPON_IS_CARRIED_BY_PLAYER
from _weapons._constants import WEAPON_IS_ACTIVE
from _weapons._constants import WeaponProficiency
from _weapons._constants import WeaponSound


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('ItemFlags',
           'MuzzleFlashStyle',
           'WeaponID',
           'WeaponProficiency',
           'WeaponSlot',
           'WeaponSound',
           'WeaponState',
           'WeaponType',
           )


# =============================================================================
# >> GLOBAL VARIABLES
# =============================================================================
# Get the base path...
_path = SP_DATA_PATH / 'weapons' / 'constants'

# Get the MuzzleFlashStyle enumerator...
MuzzleFlashStyle = IntEnum(
    'MuzzleFlashStyle', GameConfigObj(_path / 'MuzzleFlashStyle.ini'))

# Get the WeaponID enumerator...
WeaponID = IntEnum('WeaponID', GameConfigObj(_path / 'WeaponID.ini'))

# Get the WeaponType enumerator...
WeaponType = IntEnum('WeaponType', GameConfigObj(_path / 'WeaponType.ini'))

# Get the WeaponSlot enumerator...
WeaponSlot = IntEnum('WeaponSlot', GameConfigObj(_path / 'WeaponSlot.ini'))


# =============================================================================
# >> ENUMERATORS
# =============================================================================
class ItemFlags(IntFlag):
    """Item flags wrapper enumerator."""

    SELECTONEMPTY = ITEM_FLAG_SELECTONEMPTY
    NOAUTORELOAD = ITEM_FLAG_NOAUTORELOAD
    NOAUTOSWITCHEMPTY = ITEM_FLAG_NOAUTOSWITCHEMPTY
    LIMITINWORLD = ITEM_FLAG_LIMITINWORLD
    EXHAUSTIBLE = ITEM_FLAG_EXHAUSTIBLE
    DOHITLOCATIONDMG = ITEM_FLAG_DOHITLOCATIONDMG
    NOAMMOPICKUPS = ITEM_FLAG_NOAMMOPICKUPS
    NOITEMPICKUP = ITEM_FLAG_NOITEMPICKUP


class WeaponState(IntEnum):
    """Weapon states wrapper enumerator."""

    NOT_CARRIED = WEAPON_NOT_CARRIED
    IS_CARRIED_BY_PLAYER = WEAPON_IS_CARRIED_BY_PLAYER
    IS_ACTIVE = WEAPON_IS_ACTIVE
