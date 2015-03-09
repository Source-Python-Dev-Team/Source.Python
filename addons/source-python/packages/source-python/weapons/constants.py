# ../weapons/constants.py

"""Provides constant values that are weapons based."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Python Imports
#   Enum
from enum import IntEnum


# =============================================================================
# >> FORWARD IMPORTS
# =============================================================================
# Source.Python Imports
#   Weapons
from _weapons._constants import ITEM_FLAG_SELECTONEMPTY
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
from _weapons._constants import WeaponID
from _weapons._constants import WeaponSlot
from _weapons._constants import WeaponSound
from _weapons._constants import WeaponType


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
# >> ENUMERATORS
# =============================================================================
class ItemFlags(IntEnum):

    """Item flags wrapper enumerator."""

    SELECTONEMPTY = ITEM_FLAG_SELECTONEMPTY
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
