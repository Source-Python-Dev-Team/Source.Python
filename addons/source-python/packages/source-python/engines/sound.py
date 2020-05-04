# ../engines/sound.py

"""Provides access to the Sound and StreamSound interfaces."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Python Imports
#   Contextlib
from contextlib import closing
#   Enum
from enum import Enum
#   Os
import os
#   Wave
import wave

# Site Package Imports
#   Mutagen
from mutagen import mp3
from mutagen import oggvorbis

# Source.Python Imports
#   Core
from core import WeakAutoUnload
#   Engines
from engines import engines_logger
#   Entities
from entities.constants import INVALID_ENTITY_INDEX
#   Filesystem
from filesystem import SourceFile
#   Mathlib
from mathlib import NULL_VECTOR
#   Paths
from paths import GAME_PATH
#   Stringtables
from stringtables import INVALID_STRING_INDEX, string_tables
from stringtables.downloads import Downloadables


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


# =============================================================================
# >> CLASSES
# =============================================================================
class _BaseSound(WeakAutoUnload):
    """Base class for sound classes."""

    # Set the base _downloads attribute to know whether
    #   or not the sample was added to the downloadables
    _downloads = None

    def __init__(
            self, sample, index=SOUND_FROM_WORLD, volume=VOL_NORM,
            attenuation=Attenuation.NONE, channel=Channel.AUTO,
            flags=SoundFlags.NO_FLAGS, pitch=Pitch.NORMAL,
            origin=NULL_VECTOR, direction=NULL_VECTOR, origins=(),
            update_positions=True, sound_time=0.0,
            speaker_entity=INVALID_ENTITY_INDEX, download=False):
        """Store all the given attributes and set the module for unloading.

        :param str sample:
            Path to the sound file.
        :param int index:
            An entity index to play the sound from. Use
            :data:`SOUND_FROM_WORLD` if you want to play the sound from the
            world.
        :param float volume:
            Volume to play the sound with.
        :param Attenuation attenuation:
            Define how the sound attenuates.
        :param Channel channel:
            Channel to use when playing the sound.
        :param SoundFlags flags:
        :param Pitch pitch:
            Pitch of the sound.
        :param Vector origin:
            Location to play the sound from.
        :param Vector direction:
            Direction to play the sound.
        :param tuple origins:
            Locations to play the sound from.
        :param bool update_positions:
        :param float sound_time:
        :param int speaker_entity:
        :param bool download:
            If ``True`` the sound file will be added to the ``downloadables``
            list.
        """
        # Set sample as a private attribute, since it should never change
        # Added replacing \ with / in paths for comformity
        self._sample = sample.replace('\\', '/')

        self._duration = None

        # Set all the base attributes
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
            self._downloads.add(self.relative_path)

    def play(self, *recipients):
        """Play the sound.

        :param recipients:
            Players who will hear the sound.
        """
        # Done here to fix a cyclic import...
        from filters.recipients import RecipientFilter

        # Get the recipients to play the sound to
        recipients = RecipientFilter(*recipients)

        # Is the sound precached?
        if not self.is_precached:

            # Precache the sound
            self.precache()

        # Play the sound
        self._play(recipients)

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
        self._stop(index, channel)

    def _play(self, recipients):
        """Play the sound (internal)."""
        raise NotImplementedError

    def _stop(self, index, channel):
        """Stop a sound from being played (internal)."""
        raise NotImplementedError

    def precache(self):
        """Precache the sample."""
        raise NotImplementedError

    @property
    def is_precached(self):
        """Return whether or not the sample is precached."""
        raise NotImplementedError

    @property
    def sample(self):
        """Return the filename of the Sound instance.

        :rtype: str
        """
        return self._sample

    @property
    def extension(self):
        """Return the type of sound.

        :rtype: str
        """
        return self.full_path.ext[1:]

    @property
    def full_path(self):
        """Return the full path to the file.

        :rtype: path.Path
        """
        return GAME_PATH / 'sound' / self.sample

    @property
    def relative_path(self):
        """Return the relative path to the file.

        :rtype: str
        """
        return 'sound' + os.sep + self.sample

    @property
    def duration(self):
        """Return the duration of the sample.

        :rtype: float
        """
        if self._duration is not None:
            return self._duration

        with closing(SourceFile.open(self.relative_path, 'rb')) as f:
            if self.extension == 'ogg':
                value = oggvorbis.Open(f).info.length
            elif self.extension == 'mp3':
                value = mp3.Open(f).info.length
            elif self.extension == 'wav':
                with closing(wave.open(f)) as open_file:
                    value = open_file.getnframes() / open_file.getframerate()
            else:
                raise NotImplementedError(
                    'Sound extension "{extension}" is not supported.'.format(
                        extension=self.extension,
                    )
                )

        self._duration = value
        return value

    def _unload_instance(self):
        """Remove the sample from the downloads list."""
        if self._downloads is not None:
            self._downloads._unload_instance()


class Sound(_BaseSound):
    """Class used to interact with precached sounds.

    .. note::

       On some engines (e.g. CS:GO) server is unable to precache the sound,
       thus the sound won't be played. StreamSound is recommended in that case.
       However, sounds located in sound/music/ directory are always streamed
       on those engines, and this class will be able to play them.
    """
    def _play(self, recipients):
        """Play the sound (internal)."""
        engine_sound.emit_sound(
            recipients, self.index, self.channel, self.sample,
            self.volume, self.attenuation, self.flags, self.pitch,
            self.origin, self.direction, self.origins,
            self.update_positions, self.sound_time, self.speaker_entity)

    def _stop(self, index, channel):
        """Stop a sound from being played (internal)."""
        engine_sound.stop_sound(index, channel, self.sample)

    def precache(self):
        """Precache the sample."""
        engine_sound.precache_sound(self.sample)

    @property
    def is_precached(self):
        """Return whether or not the sample is precached.

        :rtype: bool
        """
        # We can't use engine_sound.is_sound_precached here because it always
        #   returns True.
        return self.sample in string_tables.soundprecache


class StreamSound(_BaseSound):
    """Class used to interact with streamed sounds.

    .. note::

       This class is a recommended choice on some engines (e.g. CS:GO),
       however, it's unable to play *.wav-files.

    .. note::

        On some engines (e.g. CS:GO) files that are located in sound/music/
        directory are already streamed, so simple Sound class can be used
        instead.
    """
    @property
    def _stream_sample(self):
        """Return the streamed sample path of the Sound instance."""
        return "*/{}".format(self.sample)

    def _play(self, recipients):
        """Play the sound (internal)."""
        engine_sound.emit_sound(
            recipients, self.index, self.channel, self._stream_sample,
            self.volume, self.attenuation, self.flags, self.pitch,
            self.origin, self.direction, self.origins,
            self.update_positions, self.sound_time, self.speaker_entity)

    def _stop(self, index, channel):
        """Stop a sound from being played (internal)."""
        engine_sound.stop_sound(index, channel, self._stream_sample)

    def precache(self):
        """Precache the sample."""
        string_tables.soundprecache.add_string(
            self._stream_sample, self._stream_sample)

    @property
    def is_precached(self):
        """Return whether or not the sample is precached.

        :rtype: bool
        """
        # We can't use engine_sound.is_sound_precached here because it always
        #   returns True.
        return self._stream_sample in string_tables.soundprecache
