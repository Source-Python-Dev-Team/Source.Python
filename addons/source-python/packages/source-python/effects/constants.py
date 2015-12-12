# ../effects/constants.py

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
__all__ = ('ALIAS_ALPHA_NAME',
           'ALIAS_BLUE_NAME',
           'ALIAS_GREEN_NAME',
           'ALIAS_MODEL_INDEX_NAME',
           'ALIAS_RED_NAME'
           )


# ============================================================================
# >> GLOBAL VARIABLES
# ============================================================================
# Aliases used by the "color" property...
ALIAS_RED_NAME = 'red'
ALIAS_GREEN_NAME = 'green'
ALIAS_BLUE_NAME = 'blue'
ALIAS_ALPHA_NAME = 'alpha'

# Alias used by the "model" property...
ALIAS_MODEL_INDEX_NAME = "model_index"


# =============================================================================
# >> IMPORTS
# =============================================================================
class BloodColor(IntEnum):
    """Blood colors wrapper enumerator."""

    DONT_BLEED = DONT_BLEED
    RED = BLOOD_COLOR_RED
    YELLOW = BLOOD_COLOR_YELLOW
    GREEN = BLOOD_COLOR_GREEN
    MECH = BLOOD_COLOR_MECH
