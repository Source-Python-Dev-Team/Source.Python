# ../engines/sound/sound.py

"""Provides access to the Sound interface."""

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
class Sound(_BaseSound):
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
        """Return whether or not the sample is precached."""
        return self.sample in string_tables.soundprecache
