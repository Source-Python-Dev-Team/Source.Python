# ../engines/sound.py

"""Provides access to the Sound interface."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Python Imports
#   Enum
from enum import IntEnum

# Source.Python Imports
#   Core
from core import AutoUnload
#   Engines
from engines import engines_logger
#   Filters
from filters.recipients import RecipientFilter
#   Mathlib
from mathlib import NULL_VECTOR
#   Stringtables
from stringtables import string_tables
from stringtables.downloads import Downloadables


# =============================================================================
# >> FORWARD IMPORTS
# =============================================================================
# Source.Python Imports
#   Engines
from _engines import engine_sound
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
__all__ = ('Attenuations',
           'Channels',
           'engine_sound',
           'SOUND_FROM_LOCAL_PLAYER',
           'SOUND_FROM_WORLD',
           'Sound',
           'SoundFlags',
           'VOL_NORM',
           )


# =============================================================================
# >> GLOBAL VARIABLES
# =============================================================================
# Get the sp.engines.sound logger
engines_sound_logger = engines_logger.sound


# =============================================================================
# >> ENUMERATORS
# =============================================================================
class Attenuations(IntEnum):

    """Attenuation values wrapper enumerator."""

    NONE = ATTN_NONE
    NORMAL = ATTN_NORM
    IDLE = ATTN_IDLE
    STATIC = ATTN_STATIC
    RICOCHET = ATTN_RICOCHET
    GUNFIRE = ATTN_GUNFIRE
    MAXIMUM = MAX_ATTENUATION


class PitchTypes(IntEnum):

    """Pitch values wrapper enumerator."""

    NORMAL = PITCH_NORM
    LOW = PITCH_LOW
    HIGH = PITCH_HIGH


class Sound(AutoUnload):

    """Class used to interact with a specific sound file."""

    # Set the base _downloads attribute to know whether
    #   or not the sample was added to the downloadables
    _downloads = None

    def __init__(
            self, recipients, index, sample, volume=VOL_NORM,
            attenuation=Attenuations.NONE, channel=Channels.AUTO,
            flags=0, pitch=PitchTypes.NORMAL, origin=NULL_VECTOR,
            direction=NULL_VECTOR, origins=(), update_positions=True,
            sound_time=0.0, speaker_entity=-1, download=False):
        """Store all the given attributes and set the module for unloading."""
        # Set sample as a private attribute, since it should never change
        # Added replacing \ with / in paths for comformity
        self._sample = sample.replace('\\', '/')

        # Set all the base attributes
        self.recipients = recipients
        self.index = index
        self.volume = volume
        self.attenuation = attenuation
        self.channel = channel
        self.flags = flags
        self.pitch = pitch
        self.origin = origin
        self.direction = direction
        self.origins = origins
        self.update_positions = update_positions
        self.sound_time = sound_time
        self.speaker_entity = speaker_entity

        # Should the sample be downloaded by clients?
        if download:

            # Add the sample to Downloadables
            self._downloads = Downloadables()
            self._downloads.add('sound/{0}'.format(self.sample))

    def play(self, *recipients):
        """Play the sound."""
        # Get the recipients to play the sound to
        recipients = RecipientFilter(*(recipients or self.recipients))

        # Is the sound precached?
        if not self.is_precached():

            # Precache the sound
            self.precache()

        # Play the sound
        engine_sound.emit_sound(
            recipients, self.index, self.channel, self.sample,
            self.volume, self.attenuation, self.flags, self.pitch,
            self.origin, self.direction, self.origins,
            self.update_positions, self.sound_time, self.speaker_entity)

    def stop(self, index=None, channel=None):
        """Stop a sound from being played."""
        # Was an index passed in?
        if index is None:

            # Use the instance's index
            index = self.index

        # Was a channel passed in?
        if channel is None:

            # Use the instance's index
            channel = self.channel

        # Stop the sound
        engine_sound.stop_sound(index, channel, self.sample)

    def precache(self):
        """Precache the sample."""
        engine_sound.precache_sound(self.sample)

    def is_precached(self):
        """Return whether or not the sample is precached."""
        return self.sample in string_tables.soundprecache

    @property
    def sample(self):
        """Return the filename of the Sound instance."""
        return self._sample

    @property
    def duration(self):
        """Return the duration of the sample."""
        return engine_sound.get_sound_duration(self.sample)

    def _unload_instance(self):
        """Remove the sample from the downloads list."""
        if self._downloads is not None:
            self._downloads._unload_instance()
