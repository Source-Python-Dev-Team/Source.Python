# ../messages/types/cstrike/vguimenu.py

"""Contains cstrike specific VGUIMenu implementation."""

# ============================================================================
# >> IMPORTS
# ============================================================================
# Source.Python Imports
#   Messages
from messages.types.vguimenu import VGUIMenu as BaseVGUIMenu


# ============================================================================
# >> CLASSES
# ============================================================================
class VGUIMenu(BaseVGUIMenu):
    """Class used to send a VGUIMenu message"""

    def _write_field_value(
            self, parameter_name, usermsg, field_type,
            field_name, field_value, field_index=-1):
        """Write the given field value to the given message"""

        # Is the given parameter "subkeys"?
        if parameter_name == 'subkeys':

            # Is the given value not a dictionnary?
            if not isinstance(field_value, dict):

                # Assume a single key
                length = 1

            # Otherwise
            else:

                # Get the given dictionnary length
                length = len(field_value)

            # Write the subkeys length
            usermsg.set_byte('', length, -1)

        # Write the given field value
        super(VGUIMenu, self)._write_field_value(
            parameter_name, usermsg, field_type,
            field_name, field_value, field_index)
