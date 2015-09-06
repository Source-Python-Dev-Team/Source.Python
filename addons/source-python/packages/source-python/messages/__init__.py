# ../messages/__init__.py

"""Provides user message based functionality."""

# ============================================================================
# >> IMPORTS
# ============================================================================
# Source.Python Imports
#   Loggers
from loggers import _sp_logger
from filters.recipients import RecipientFilter


# ============================================================================
# >> FORWARD IMPORTS
# ============================================================================
#   Messages
from _messages import DialogType
from _messages import UserMessage


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('DialogType',
           'UserMessage',
           'UserMessageCreator',
           'VGUIMenu',
           )


# =============================================================================
# >> GLOBAL VARIABLES
# =============================================================================
# Get the sp.messages logger
messages_logger = _sp_logger.messages

# TODO: Temp "fix" to be able to load the plugin
ShowMenu = None


# =============================================================================
# >> CLASSES
# =============================================================================
class UserMessageCreator(object):

    """Provide an easy interface to create user messages."""

    def __init__(self, *args):
        """Initialize the user message."""
        self.args = args

    def send(self, *player_indexes):
        """Send the user message."""
        # TODO: Handle translations
        user_message = UserMessage(
            RecipientFilter(*player_indexes), self.message_name)

        if user_message.is_protobuf:
            self.protobuf(user_message.buffer, *self.args)
        else:
            self.bitbuf(user_message.buffer, *self.args)

        user_message.send()

    def protobuf(self, buffer, *args):
        """Protobuf implementation of this user message."""
        raise NotImplementedError('Must be implemented by a subclass.')

    def bitbuf(self, buffer, *args):
        """Bitbuf implementation of this user message."""
        raise NotImplementedError('Must be implemented by a subclass.')

    @property
    def message_name(self):
        """Return the user message name."""
        raise NotImplementedError('Must be implemented by a subclass.')


class VGUIMenu(UserMessageCreator):

    """Create a VGUIMenu."""

    message_name = 'VGUIMenu'

    def __init__(self, name, subkeys={}, show=True):
        """Initialize the VGUI menu.

        @param <name>:
        A string that defines the name of the menu.

        @param <show>:
        If True the menu will be shown, else it will be hidden.

        @param <subkeys>:
        A dictionary that defines the data for the menu.
        """
        # TODO: Which names and subkeys are available?
        super().__init__(name, subkeys, show)

    def protobuf(self, buffer, name, subkeys, show):
        buffer.set_string('name', name)
        buffer.set_bool('show', show)
        for key, value in subkeys.items():
            temp_buffer = buffer.add_message('subkeys')
            temp_buffer.set_string('name', key)
            temp_buffer.set_string('str', value)

    def bitbuf(self, buffer, name, subkeys, show):
        buffer.write_string(name)
        buffer.write_byte(show)
        buffer.write_byte(len(subkeys))
        for key, value in subkeys.items():
            buffer.write_string(key)
            buffer.write_string(value)