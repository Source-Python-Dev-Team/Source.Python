# ../messages/types/showmenu.py

"""Contains base ShowMenu implementation."""

# ============================================================================
# >> IMPORTS
# ============================================================================
# Source.Python Imports
#   Messages
from messages.base import BaseMessage


# ============================================================================
# >> CLASSES
# ============================================================================
class ShowMenu(BaseMessage):

    """Class used to send a ShowMenu message."""

    def _prepare_parameter(self, parameter_name, parameter_value):
        """Prepare the given parameter value."""
        # Is the given parameter 'slots'?
        if parameter_name == 'slots':

            # Is the given value not an integer?
            if not isinstance(parameter_value, int):

                # Get a variable to store the validated slots
                validated_slots = 0

                # Loop trhough all valid slots
                for slot in range(*map(int, self._valid_slots_range)):

                    # Is the current slot not given?
                    if slot not in parameter_value:

                        # No need to go further
                        continue

                    # Add the current slot to the validated ones
                    validated_slots += 1 << (slot - 1)

                # Override the given value
                parameter_value = validated_slots

        # Prepare the given value
        return super(ShowMenu, self)._prepare_parameter(
            parameter_name, parameter_value)

    def _send_message(self, recipient, **kwargs):
        """Send the message to the given recipient filter."""
        # TODO: Once we can listen menuselect, refresh the menu based on
        #   self._refreshing_time till we get a selection.

        # Send the message to the given recipient
        super(ShowMenu, self)._send_message(recipient, **kwargs)
