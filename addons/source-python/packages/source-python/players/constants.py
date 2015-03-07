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


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('PlayerFlags',
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
