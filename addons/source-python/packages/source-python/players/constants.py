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
from _players._constants import IN_ATTACK
from _players._constants import IN_JUMP
from _players._constants import IN_DUCK
from _players._constants import IN_FORWARD
from _players._constants import IN_BACK
from _players._constants import IN_USE
from _players._constants import IN_CANCEL
from _players._constants import IN_LEFT
from _players._constants import IN_RIGHT
from _players._constants import IN_MOVELEFT
from _players._constants import IN_MOVERIGHT
from _players._constants import IN_ATTACK2
from _players._constants import IN_RUN
from _players._constants import IN_RELOAD
from _players._constants import IN_ALT1
from _players._constants import IN_ALT2
from _players._constants import IN_SCORE
from _players._constants import IN_SPEED
from _players._constants import IN_WALK
from _players._constants import IN_ZOOM
from _players._constants import IN_WEAPON1
from _players._constants import IN_WEAPON2
from _players._constants import IN_BULLRUSH
from _players._constants import IN_GRENADE1
from _players._constants import IN_GRENADE2
from _players._constants import IN_LOOKSPIN
from _players._constants import LIFE_ALIVE
from _players._constants import LIFE_DYING
from _players._constants import LIFE_DEAD
from _players._constants import LIFE_RESPAWNABLE
from _players._constants import LIFE_DISCARDBODY


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('LifeState',
           'PlayerButtons',
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


class PlayerButtons(IntEnum):

    """Player buttons wrapper enumerator."""

    ATTACK = IN_ATTACK
    JUMP = IN_JUMP
    DUCK = IN_DUCK
    FORWARD = IN_FORWARD
    BACK = IN_BACK
    USE = IN_USE
    CANCEL = IN_CANCEL
    LEFT = IN_LEFT
    RIGHT = IN_RIGHT
    MOVELEFT = IN_MOVELEFT
    MOVERIGHT = IN_MOVERIGHT
    ATTACK2 = IN_ATTACK2
    RUN = IN_RUN
    RELOAD = IN_RELOAD
    ALT1 = IN_ALT1
    ALT2 = IN_ALT2
    SCORE = IN_SCORE
    SPEED = IN_SPEED
    WALK = IN_WALK
    ZOOM = IN_ZOOM
    WEAPON1 = IN_WEAPON1
    WEAPON2 = IN_WEAPON2
    BULLRUSH = IN_BULLRUSH
    GRENADE1 = IN_GRENADE1
    GRENADE2 = IN_GRENADE2
    LOOKSPIN = IN_LOOKSPIN
