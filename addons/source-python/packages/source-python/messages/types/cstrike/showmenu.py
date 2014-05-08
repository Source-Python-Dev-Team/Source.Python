# ../messages/types/cstrike/showmenu.py

# ============================================================================
# >> IMPORTS
# ============================================================================
# Python Imports
#   Collections
from collections import ChainMap

# Source.Python Imports
#   Messages
from messages.types.showmenu import ShowMenu as BaseShowMenu


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
# Set all to an empty list
__all__ = []


# ============================================================================
# >> GLOBAL VARIABLES
# ============================================================================
# Store the maximum length
MAXIMUM_LENGTH = 255


# ============================================================================
# >> CLASSES
# ============================================================================
class ShowMenu(BaseShowMenu):
    '''Class used to send a ShowMenu message'''

    def _send_message(self, recipient, **kwargs):
        '''Send the message to the given recipient filter'''

        # Get the 'message' parameter value
        message = str(kwargs['message'])

        # Loop as long as the message is larger than the maximum length
        while len(message) > MAXIMUM_LENGTH:

            # Send the message
            super(ShowMenu, self)._send_message(
                recipient, **ChainMap(dict(message=message[
                    :MAXIMUM_LENGTH], _wait_for_more=True), kwargs))

            # Remove the text we just sent from the original message
            message = message[MAXIMUM_LENGTH:]

        # Send the last characters
        super(ShowMenu, self)._send_message(
            recipient, **ChainMap(dict(message=message), kwargs))
