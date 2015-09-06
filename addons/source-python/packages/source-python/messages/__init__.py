# ../messages/__init__.py

"""Provides user message based functionality."""

# ============================================================================
# >> IMPORTS
# ============================================================================
# Python Imports
import collections

# Source.Python Imports
#   Loggers
from loggers import _sp_logger
#   Filters
from filters.recipients import RecipientFilter
from filters.players import PlayerIter
#   Players
from players.helpers import get_client_language
from players.helpers import playerinfo_from_index
#   Translations
from translations.strings import TranslationStrings


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
           'ShowMenu',
           'SayText2',
           'HintText',
           'SayText',
           'Shake',
           'ResetHUD',
           'TextMsg',
           'KeyHintText',
           'Fade'
           )


# =============================================================================
# >> GLOBAL VARIABLES
# =============================================================================
# Get the sp.messages logger
messages_logger = _sp_logger.messages


# =============================================================================
# >> CLASSES
# =============================================================================
class UserMessageCreator(object):

    """Provide an easy interface to create user messages."""

    def __init__(self, *args):
        """Initialize the user message."""
        self.args = args

    def send(self, *player_indexes, **tokens):
        """Send the user message."""
        if not player_indexes:
            player_indexes = PlayerIter()

        # Categorize the players by their language
        languages = collections.defaultdict(set)
        for index in player_indexes:
            if playerinfo_from_index(index).is_fake_client():
                # No need to send a user message to bots
                continue

            languages[get_client_language(index)].add(index)

        for language, indexes in languages.items():
            translated_args = []

            # Translate the arguments
            for arg in self.args:
                if isinstance(arg, TranslationStrings):
                    translated_args.append(arg.get_string(language, **tokens))
                else:
                    translated_args.append(arg)

            # Send the user message
            user_message = UserMessage(
                RecipientFilter(*indexes), self.message_name)

            if user_message.is_protobuf():
                self.protobuf(user_message.buffer, *translated_args)
            else:
                self.bitbuf(user_message.buffer, *translated_args)

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


class ShowMenu(UserMessageCreator):

    """Create a radio menu."""

    message_name = 'ShowMenu'
    CHUNK_SIZE = 62

    def __init__(self, menu_string, valid_slots=1023, display_time=4):
        """Initialize the radio menu."""
        super().__init__(menu_string, valid_slots, display_time)

    def send(self, *player_indexes):
        """Send the user message."""
        # We need to handle the ShowMenu user message with bitbuffers
        # differently, because the maximum size is 255. If the message exceeds
        # this length, we need to sent it in several parts.
        if UserMessage.is_protobuf():
            user_message = UserMessage(
                RecipientFilter(*player_indexes), self.message_name)
            self.protobuf(user_message.buffer, *self.args)
            user_message.send()
        else:
            self.bitbuf(player_indexes, *self.args)

    def protobuf(self, buffer, menu_string, valid_slots, display_time):
        buffer.set_int32('bits_valid_slots', valid_slots)
        buffer.set_int32('display_time', display_time)
        buffer.set_string('menu_string', menu_string)

    def bitbuf(self, player_indexes, menu_string, valid_slots, display_time):
        length = len(menu_string)
        while True:
            user_message = UserMessage(
                RecipientFilter(*player_indexes), self.message_name)

            buffer = user_message.buffer
            buffer.write_word(valid_slots)
            buffer.write_char(display_time)
            buffer.write_byte(length > self.CHUNK_SIZE)
            buffer.write_string(menu_string[:self.CHUNK_SIZE])

            user_message.send()

            if length > self.CHUNK_SIZE:
                menu_string = menu_string[self.CHUNK_SIZE:]
                length -= self.CHUNK_SIZE
            else:
                break


class SayText2(UserMessageCreator):

    """Create a SayText2."""

    message_name = 'SayText2'

    def __init__(self, message, index=0, chat=False, param1='', param2='',
            param3='', param4=''):
        super().__init__(message, index, chat, param1, param2, param3, param4)

    def protobuf(self, buffer, message, index, chat, param1, param2, param3,
            param4):
        buffer.set_string('msg_name', message)
        buffer.set_bool('chat', chat)
        buffer.set_int32('ent_idx', index)
        buffer.add_string('params', param1)
        buffer.add_string('params', param2)
        buffer.add_string('params', param3)
        buffer.add_string('params', param4)
        # TODO: Handle textchatall

    def bitbuf(self, buffer, message, index, chat, param1, param2, param3,
            param4):
        buffer.write_byte(index)
        buffer.write_byte(chat)
        buffer.write_string(message)
        buffer.write_string(param1)
        buffer.write_string(param2)
        buffer.write_string(param3)
        buffer.write_string(param4)


class HintText(UserMessageCreator):

    """Create a HintText."""

    message_name = 'HintText'

    def __init__(self, message):
        super().__init__(message)

    def protobuf(self, buffer, message):
        buffer.set_string('text', message)

    def bitbuf(self, buffer, message):
        buffer.write_string(message)


class SayText(UserMessageCreator):

    """Create a SayText."""

    message_name = 'SayText'

    def __init__(self, message, index=0, chat=False):
        super().__init__(message, index, chat)

    def protobuf(self, buffer, message, index, chat):
        buffer.set_int32('ent_idx', index)
        buffer.set_bool('chat', chat)
        buffer.set_string('text', message)

    def bitbuf(self, buffer, message, index, chat):
        buffer.write_byte(index)
        buffer.write_string(message)
        buffer.write_byte(chat)


class Shake(UserMessageCreator):

    """Create a Shake."""

    message_name = 'Shake'

    def __init__(self, shake_command, amplitude, frequency, duration):
        super().__init__(shake_command, amplitude, frequency, duration)

    def protobuf(self, buffer, shake_command, amplitude, frequency, duration):
        buffer.set_enum('command', shake_command)
        buffer.set_float('local_amplitude', amplitude)
        buffer.set_float('frequency', frequency)
        buffer.set_float('duration', duration)

    def bitbuf(self, buffer, shake_command, amplitude, frequency, duration):
        buffer.write_byte(shake_command)
        buffer.write_float(amplitude)
        buffer.write_float(frequency)
        buffer.write_float(duration)


class ResetHUD(UserMessageCreator):

    """Create a ResetHUD."""

    message_name = 'ResetHUD'

    def __init__(self, reset=True):
        super().__init__(reset)

    def protobuf(self, buffer, reset):
        buffer.set_bool('reset', reset)

    def bitbuf(self, buffer, reset):
        buffer.write_byte(reset)


class TextMsg(UserMessageCreator):

    """Create a TextMsg."""

    message_name = 'TextMsg'

    def __init__(self, destination, name, param1='', param2='', param3='',
            param4=''):
        super().__init__(destination, name, param1, param2, param3, param4)

    def protobuf(self, buffer, destination, name, param1, param2, param3,
            param4):
        buffer.set_int32('msg_dst', destination)
        buffer.add_string('params', param1)
        buffer.add_string('params', param2)
        buffer.add_string('params', param3)
        buffer.add_string('params', param4)

    def bitbuf(self, buffer, destination, name, param1, param2, param3,
            param4):
        buffer.write_byte(destination)
        buffer.write_string(name)
        buffer.write_string(param1)
        buffer.write_string(param2)
        buffer.write_string(param3)
        buffer.write_string(param4)


class KeyHintText(UserMessageCreator):

    """Create a KeyHintText."""

    message_name = 'KeyHintText'

    def __init__(self, *hints):
        super().__init__(*hints)

    def protobuf(self, buffer, *hints):
        for hint in hints:
            buffer.add_string('hints', hint)

    def bitbuf(self, buffer, *hints):
        buffer.write_byte(len(hints))
        for hint in hints:
            buffer.write_string(hint)


class Fade(UserMessageCreator):

    """Create a Fade."""

    message_name = 'Fade'

    def __init__(self, duration, hold_time, flags, r=255, g=255, b=255, a=255):
        super().__init__(duration, hold_time, flags, r, g, b, a)

    def protobuf(self, buffer, duration, hold_time, flags, r, g, b, a):
        buffer.set_int32(duration)
        buffer.set_int32(hold_time)
        buffer.set_int32(flags)
        color = buffer.mutable_message('clr')
        color.set_int32('r', r)
        color.set_int32('g', g)
        color.set_int32('b', b)
        color.set_int32('a', a)

    def bitbuf(self, buffer, duration, hold_time, flags, r, g, b, a):
        buffer.write_short(duration)
        buffer.write_short(hold_time)
        buffer.write_short(flags)
        buffer.write_byte(r)
        buffer.write_byte(g)
        buffer.write_byte(b)
        buffer.write_byte(a)
