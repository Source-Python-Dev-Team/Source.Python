# ../messages/types/saytext.py

# ============================================================================
# >> IMPORTS
# ============================================================================
# Python Imports
#   Codecs
from codecs import unicode_escape_decode

# Source.Python Imports
#   Messages
from messages.base import BaseMessage


# ============================================================================
# >> CLASSES
# ============================================================================
class SayText(BaseMessage):
    '''Class used to send a SayText message'''

    def _prepare_parameter(self, parameter_name, parameter_value):
        '''Prepare the given parameter value'''

        # Prepare the given value
        return_value = super(SayText, self)._prepare_parameter(
            parameter_name, parameter_value)

        # Is the given parameter 'text'?
        if parameter_name == 'message':

            # Adding ESCSOH to the start of the message seems to fix colors
            #   passed at the begining.
            return unicode_escape_decode(self._message_prefix)[0] + return_value

        # Return the prepared value
        return return_value
