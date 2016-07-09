# ../engines/sound/__init__.py

"""Provides access to the Sound and StreamSound interfaces."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Python Imports
#   Enum
from enum import Enum

# Source.Python Imports
#   Engines
from engines import engines_logger


# =============================================================================
# >> FORWARD IMPORTS
# =============================================================================
# Source.Python Imports
#   Engines
from _engines._sound import Channel
from _engines._sound import VOL_NORM
from _engines._sound import ATTN_NONE
from _engines._sound import ATTN_NORM
from _engines._sound import ATTN_IDLE
from _engines._sound import ATTN_STATIC
from _engines._sound import ATTN_RICOCHET
from _engines._sound import ATTN_GUNFIRE
from _engines._sound import MAX_ATTENUATION
from _engines._sound import SoundFlags
from _engines._sound import Pitch
from _engines._sound import SOUND_FROM_LOCAL_PLAYER
from _engines._sound import SOUND_FROM_WORLD
from _engines._sound import engine_sound


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('Attenuation',
           'Channel',
           'Pitch',
           'SOUND_FROM_LOCAL_PLAYER',
           'SOUND_FROM_WORLD',
           'Sound',
           'SoundFlags',
           'StreamSound',
           'VOL_NORM',
           'engine_sound',
           )


# =============================================================================
# >> GLOBAL VARIABLES
# =============================================================================
# Get the sp.engines.sound logger
engines_sound_logger = engines_logger.sound


# =============================================================================
# >> ENUMERATORS
# =============================================================================
class Attenuation(float, Enum):
    """Attenuation values wrapper enumerator."""

    NONE = ATTN_NONE
    NORMAL = ATTN_NORM
    IDLE = ATTN_IDLE
    STATIC = ATTN_STATIC
    RICOCHET = ATTN_RICOCHET
    GUNFIRE = ATTN_GUNFIRE
    MAXIMUM = MAX_ATTENUATION


from engines.sound.sound import Sound
from engines.sound.stream import StreamSound
