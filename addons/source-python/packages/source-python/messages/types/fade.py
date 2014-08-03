# ../messages/types/fade.py

"""Contains base Fade implementation."""

# ============================================================================
# >> IMPORTS
# ============================================================================
# Source.Python Imports
#   Messages
from messages.base import BaseMessage


# ============================================================================
# >> CLASSES
# ============================================================================
class Fade(BaseMessage):
    """Class used to send a Fade message"""

    def _prepare_parameter(self, parameter_name, parameter_value):
        """Prepare the given parameter value"""

        # Prepare the given parameter
        return_value = super(Fade, self)._prepare_parameter(
            parameter_name, parameter_value)

        # Is the given parameter "duration" or "hold_time"?
        if parameter_name in ('duration', 'hold_time'):

            # Override the given parameter value
            return_value = (return_value * int(self.SCREENFADE_FRACBITS)) * 60

        # Return the prepared value
        return return_value
