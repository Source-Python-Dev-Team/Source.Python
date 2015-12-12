# ../effects/constants.py

# =============================================================================
# >> IMPORTS
# =============================================================================
# Python Imports
#   Enum
from enum import Enum
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
           'TempEntityAlias',
           )


# =============================================================================
# >> ENUMERATORS
# =============================================================================
class TempEntityAlias(Enum):
    """Temp entity aliases enumerator."""

    # Aliases used by the "color" property...
    RED = 'red'
    GREEN = 'green'
    BLUE = 'blue'
    ALPHA = 'alpha'

    # Alias used by the "model" property...
    MODEL_INDEX = 'model_index'

    def __str__(self):
        """Return the value as a string instance.

        :rtype: str"""
        return str(self.value)


class BloodColor(IntEnum):
    """Blood colors wrapper enumerator."""

    DONT_BLEED = DONT_BLEED
    RED = BLOOD_COLOR_RED
    YELLOW = BLOOD_COLOR_YELLOW
    GREEN = BLOOD_COLOR_GREEN
    MECH = BLOOD_COLOR_MECH
