# ../engines/sound/base.py

"""Provides access to the _BaseSound interface."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
#   Core
from core import AutoUnload
#   Engines
from engines.sound import Attenuation
from engines.sound import Channel
from engines.sound import engine_sound
from engines.sound import Pitch
from engines.sound import SOUND_FROM_WORLD
from engines.sound import SoundFlags
from engines.sound import VOL_NORM
#   Entities
from entities.constants import INVALID_ENTITY_INDEX
#   Filters
from filters.recipients import RecipientFilter
#   Mathlib
from mathlib import NULL_VECTOR
#   Stringtables
from stringtables.downloads import Downloadables


# =============================================================================
# >> CLASSES
# =============================================================================
class _BaseSound(AutoUnload):
    """Class used to interact with a specific sound file."""

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
        """Store all the given attributes and set the module for unloading."""
        # Set sample as a private attribute, since it should never change
        # Added replacing \ with / in paths for comformity
        self._sample = sample.replace('\\', '/')

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
            self._downloads.add('sound/{0}'.format(self.sample))

    def play(self, *recipients):
        """Play the sound."""
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
