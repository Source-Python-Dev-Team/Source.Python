# ../effects/constants.py

"""Provides constants for effects."""

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
#   Effects
from _effects._constants import DONT_BLEED
from _effects._constants import BLOOD_COLOR_RED
from _effects._constants import BLOOD_COLOR_YELLOW
from _effects._constants import BLOOD_COLOR_GREEN
from _effects._constants import BLOOD_COLOR_MECH


# ============================================================================
# >> ALL DECLARATION
# ============================================================================
__all__ = ('BloodColor',
           )


# =============================================================================
# >> ENUMERATORS
# =============================================================================
class BloodColor(IntEnum):
    """Blood colors wrapper enumerator."""

    DONT_BLEED = DONT_BLEED
    RED = BLOOD_COLOR_RED
    YELLOW = BLOOD_COLOR_YELLOW
    GREEN = BLOOD_COLOR_GREEN
    MECH = BLOOD_COLOR_MECH
