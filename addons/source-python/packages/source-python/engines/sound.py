# ../engines/sound.py

# =============================================================================
# >> FORWARD IMPORTS
# =============================================================================
# Python Imports
#   Inspect
from inspect import getmodule
from inspect import stack

# Source.Python Imports
from core import AutoUnload
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
#   Filters
from filters.recipients import RecipientFilter
#   Mathlib
from mathlib import NULL_VECTOR
#   Paths
from paths import PLUGIN_PATH
#   Stringtables
from stringtables.downloads import Downloadables


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = [
    'Attenuations',
    'Channels',
    'EngineSound',
    'SOUND_FROM_LOCAL_PLAYER',
    'SOUND_FROM_WORLD',
    'Sound',
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


class Sound(AutoUnload):
    '''Class used to interact with a specific sound file'''

    # Set the base _downloads attribute to know whether
    #   or not the sample was added to the downloadables
    _downloads = None

    def __init__(
            self, recipients, index, sample, volume=VOL_NORM,
            attenuation=Attenuations.NONE, channel=Channels.AUTO,
            flags=0, pitch=PitchTypes.HIGH, origin=NULL_VECTOR,
            direction=NULL_VECTOR, origins=(), update_positions=True,
            sound_time=0.0, speaker_entity=-1, download=False):
        '''Store all the given attributes and set the module for unloading'''

        # Get the file that called
        caller = getmodule(stack()[1][0])

        # Is the calling file in a plugin?
        if PLUGIN_PATH in caller.__file__:

            # Set the module to the plugin's module so that
            # _unload_instance will fire when the plugin is unloaded
            self.__module__ = caller.__name__

        # Set sample as a private attribute, since it should never change
        self._sample = sample

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

            # Get the file that called
            caller = getmodule(stack()[1][0])

            # Is the calling file in a plugin?
            if PLUGIN_PATH in caller.__file__:

                # Set the module to the plugin's module so that
                # _unload_instance will fire when the plugin is unloaded
                self.__module__ = caller.__name__

            # Add the sample to Downloadables
            self._downloads = Downloadables()
            self._downloads.add('sound/{0}'.format(self.sample))

    def play(self, *recipients):
        '''Plays the sound using either the given
           recipients or the instance's recipients'''

        # Get the recipients to play the sound to
        recipients = RecipientFilter(*(recipients or self.recipients))

        # Is the sound precached?
        if not self.is_precached():

            # Precache the sound
            self.precache()

        # Play the sound
        EngineSound.emit_sound(
            recipients, self.index, self.channel, self.sample,
            self.volume, self.attenuation, self.flags, self.pitch,
            self.origin, self.direction, self.origins,
            self.update_positions, self.sound_time, self.speaker_entity)

    def stop(self, index=None, channel=None):
        '''Stops a sound from being played'''

        # Was an index passed in?
        if index is None:

            # Use the instance's index
            index = self.index

        # Was a channel passed in?
        if channel is None:

            # Use the instance's index
            channel = self.channel

        # Stop the sound
        EngineSound.stop_sound(index, channel, self.sample)

    def precache(self):
        '''Precaches the sample'''
        EngineSound.precache_sound(self.sample)

    def is_precached(self):
        '''Returns whether or not the sample is precached'''
        return EngineSound.is_sound_precached(self.sample)

    @property
    def sample(self):
        '''Returns the sample (filename) of the Sound instance'''
        return self._sample

    @property
    def duration(self):
        '''Returns the duration of the sample'''
        return EngineSound.get_sound_duration(self.sample)

    def _unload_instance(self):
        '''Removes the sample from the downloads list'''
        if not self._downloads is None:
            self._downloads._unload_instance()
