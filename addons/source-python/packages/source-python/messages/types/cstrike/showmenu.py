# ../messages/types/cstrike/showmenu.py

"""Contains cstrike specific ShowMenu implementation."""

# ============================================================================
# >> IMPORTS
# ============================================================================
# Python Imports
#   Collections
from collections import ChainMap

# Source.Python Imports
#   Messages
from messages.types.showmenu import ShowMenu as BaseShowMenu


# ============================================================================
# >> GLOBAL VARIABLES
# ============================================================================
# Store the maximum length
MAXIMUM_LENGTH = 240


# ============================================================================
# >> CLASSES
# ============================================================================
class ShowMenu(BaseShowMenu):

    """Class used to send a ShowMenu message."""

    def _send_message(self, recipient, **kwargs):
        """Send the message to the given recipient filter."""
        # Get the 'message' parameter value
        message = str(kwargs['message'])

        # Encode the message as UTF-8
        message = message.encode('utf-8')

        # Loop as long as the message is larger than the maximum length
        while len(message) > MAXIMUM_LENGTH:

            # Safely split the message
            part, message = split(message, MAXIMUM_LENGTH)

            # Send the message
            super(ShowMenu, self)._send_message(
                recipient, **ChainMap(dict(message=part.decode('utf-8'),
                    _wait_for_more=True), kwargs))

        # Send the last characters
        super(ShowMenu, self)._send_message(
            recipient, **ChainMap(dict(message=message.decode('utf-8')),
                kwargs))


def validUTF8(s, length):
    try:
        s[:length].decode('utf-8')
        return True
    except UnicodeDecodeError:
        return False


def split(s, length):
    """Safely splits a UTF-8 string"""
    for x in range(4):
        if validUTF8(s, length - x):
            break
    return s[:length - x], s[length - x:]
