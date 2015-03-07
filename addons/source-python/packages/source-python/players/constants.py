# ../players/constants.py

"""Provides constant values that are player based."""

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
#   Players
from _players._constants import FL_ONGROUND
from _players._constants import FL_DUCKING
from _players._constants import FL_WATERJUMP
from _players._constants import FL_ONTRAIN
from _players._constants import FL_INRAIN
from _players._constants import FL_FROZEN
from _players._constants import FL_ATCONTROLS
from _players._constants import FL_CLIENT
from _players._constants import FL_FAKECLIENT
from _players._constants import FL_INWATER
from _players._constants import LIFE_ALIVE
from _players._constants import LIFE_DYING
from _players._constants import LIFE_DEAD
from _players._constants import LIFE_RESPAWNABLE
from _players._constants import LIFE_DISCARDBODY


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('LifeState',
           'PlayerFlags',
           )


# =============================================================================
# >> ENUMERATORS
# =============================================================================
class PlayerFlags(IntEnum):

    """Player flags wrapper enumerator."""

    ONGROUND = FL_ONGROUND
    DUCKING = FL_DUCKING
    WATERJUMP = FL_WATERJUMP
    ONTRAIN = FL_ONTRAIN
    INRAIN = FL_INRAIN
    FROZEN = FL_FROZEN
    ATCONTROLS = FL_ATCONTROLS
    CLIENT = FL_CLIENT
    FAKECLIENT = FL_FAKECLIENT
    INWATER = FL_INWATER


class LifeState(IntEnum):

    """Player life states wrapper enumerator."""

    ALIVE = LIFE_ALIVE
    DYING = LIFE_DYING
    DEAD = LIFE_DEAD
    RESPAWNABLE = LIFE_RESPAWNABLE
    DISCARDBODY = LIFE_DISCARDBODY