# ../messages/types/vguimenu.py

"""Contains base VGUIMenu implementation."""

# ============================================================================
# >> IMPORTS
# ============================================================================
# Source.Python Imports
#   Messages
from messages.base import BaseMessage


# ============================================================================
# >> CLASSES
# ============================================================================
class VGUIMenu(BaseMessage):
    """Class used to send a VGUIMenu message"""

    def _prepare_parameter(self, parameter_name, parameter_value):
        """Prepare the given parameter value"""

        # Is the given parameter "subkeys"?
        if parameter_name == 'subkeys':

            # Is the given value a dictionnary?
            if isinstance(parameter_value, dict):

                # Return the given value directly
                return parameter_value

        # Return the prepared value
        return super(VGUIMenu, self)._prepare_parameter(
            parameter_name, parameter_value)

    def _write_field_value(
            self, parameter_name, usermsg, field_type,
            field_name, field_value, field_index=-1):
        """Write the given field value to the given message"""

        # Is the given parameter "subkeys" and the given value a dictionnary?
        if parameter_name == 'subkeys' and isinstance(field_value, dict):

            # Loop through all subkeys
            for key, value in field_value.items():

                # Write the current key
                super(VGUIMenu, self)._write_field_value(
                    parameter_name, usermsg, field_type,
                    field_name, (key, value), field_index)

        # Otherwise
        else:

            # Write the given field value
            super(VGUIMenu, self)._write_field_value(
                parameter_name, usermsg, field_type,
                field_name, field_value, field_index)
