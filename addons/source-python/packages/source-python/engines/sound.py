# ../engines/sound.py

# =============================================================================
# >> FORWARD IMPORTS
# =============================================================================
# Source.Python Imports
#   Engines
from _engines import EngineSound
from _engines import Channels
from _engines import VOL_NORM
from _engines import ATTN_NONE
from _engines import ATTN_NORM
from _engines import ATTN_IDLE
from _engines import ATTN_STATIC
from _engines import ATTN_RICOCHET
from _engines import ATTN_GUNFIRE
from _engines import MAX_ATTENUATION
from _engines import SoundFlags
from _engines import PITCH_NORM
from _engines import PITCH_LOW
from _engines import PITCH_HIGH
from _engines import SOUND_FROM_LOCAL_PLAYER
from _engines import SOUND_FROM_WORLD


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = [
    'Attenuations',
    'Channels',
    'EngineSound',
    'SOUND_FROM_LOCAL_PLAYER',
    'SOUND_FROM_WORLD',
    'SoundFlags',
    'VOL_NORM',
]


# =============================================================================
# >> ENUMERATORS
# =============================================================================
class Attenuations(object):
    '''Attenuation values wrapper enumerator.

    TODO: Inherit of "enum.IntEnum" once we have upgraded to Python 3.4+.
    '''
    NONE = ATTN_NONE
    NORMAL = ATTN_NORM
    IDLE = ATTN_IDLE
    STATIC = ATTN_STATIC
    RICOCHET = ATTN_RICOCHET
    GUNFIRE = ATTN_GUNFIRE
    MAXIMUM = MAX_ATTENUATION


class PitchTypes(object):
    '''Pitch values wrapper enumerator.

    TODO: Inherit of "enum.IntEnum" once we have upgraded to Python 3.4+.
    '''
    NORMAL = PITCH_NORM
    LOW = PITCH_LOW
    HIGH = PITCH_HIGH
