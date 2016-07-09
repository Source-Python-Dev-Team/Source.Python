# ../engines/sound/stream.py

"""Provides access to the StreamSound interface."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
#   Engines
from engines.sound import engine_sound
from engines.sound.base import _BaseSound
#   Stringtables
from stringtables import string_tables


# =============================================================================
# >> CLASSES
# =============================================================================
class StreamSound(_BaseSound):
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
        """Return whether or not the sample is precached."""
        return self._stream_sample in string_tables.soundprecache
