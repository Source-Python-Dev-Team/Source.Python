# ../messages/base.py

"""Provides user messages."""

# ============================================================================
# >> IMPORTS
# ============================================================================
# Python Imports
import collections

# Source.Python Imports
#   Colors
from colors import Color
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
from _messages import UserMessage


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('UserMessageCreator',
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
# >> CLASSES
# =============================================================================
class AttrDict(dict):

    """A dictionary that redirects __getattr__ to __getitem__ and __setattr__
    to __setitem__."""

    __getattr__ = dict.__getitem__
    __setattr__ = dict.__setitem__


class UserMessageCreator(AttrDict):

    """Provide an easy interface to create user messages."""

    def __init__(self, local_variables):
        # Remove "self" if existant, so you can simply pass locals()
        local_variables.pop('self', 0)
        super().__init__(local_variables)

    def send(self, *player_indexes, **tokens):
        """Send the user message."""
        if not player_indexes:
            player_indexes = PlayerIter()

        for language, indexes in self._categorize_players_by_language(
                player_indexes).items():
            self._send(indexes, self._get_translated_kwargs(language, tokens))

    def _send(self, player_indexes, translated_kwargs):
        """Send the user message to the given players.

        @param <player_indexes>:
        An iterable that contains players with the same language setting.

        @param <translated_kwargs>:
        An AttrDict object that contains the translated arguments.
        """
        user_message = UserMessage(
            RecipientFilter(*player_indexes), self.message_name)

        if user_message.is_protobuf():
            self.protobuf(user_message.buffer, translated_kwargs)
        else:
            self.bitbuf(user_message.buffer, translated_kwargs)

        user_message.send()

    @staticmethod
    def _categorize_players_by_language(player_indexes):
        """Categorize players by their language.

        Return a dict in the following format:
        {<language>: set([<player index>, ...])}
        """
        languages = collections.defaultdict(set)
        for index in player_indexes:
            if playerinfo_from_index(index).is_fake_client():
                # No need to send a user message to bots
                continue

            languages[get_client_language(index)].add(index)

        return languages

    def _get_translated_kwargs(self, language, tokens):
        """Return an AttrDict object that contains translated and tokenized
        arguments.
        """
        translated_kwargs = AttrDict()
        for key, value in self.items():
            if isinstance(value, TranslationStrings):
                value = value.get_string(language, **tokens)

            translated_kwargs[key] = value

        return translated_kwargs

    def protobuf(self, buffer, translated_kwargs):
        """Protobuf implementation of this user message."""
        raise NotImplementedError('Must be implemented by a subclass.')

    def bitbuf(self, buffer, translated_kwargs):
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
        super().__init__(locals())

    def protobuf(self, buffer, kwargs):
        buffer.set_string('name', kwargs.name)
        buffer.set_bool('show', kwargs.show)
        for key, value in kwargs.subkeys.items():
            temp_buffer = buffer.add_message('subkeys')
            temp_buffer.set_string('name', key)
            temp_buffer.set_string('str', value)

    def bitbuf(self, buffer, kwargs):
        buffer.write_string(kwargs.name)
        buffer.write_byte(kwargs.show)
        buffer.write_byte(len(kwargs.subkeys))
        for key, value in kwargs.subkeys.items():
            buffer.write_string(key)
            buffer.write_string(value)


class ShowMenu(UserMessageCreator):

    """Create a radio menu."""

    message_name = 'ShowMenu'
    CHUNK_SIZE = 62

    def __init__(self, menu_string, valid_slots=1023, display_time=4):
        """Initialize the radio menu."""
        super().__init__(locals())

    def send(self, *player_indexes):
        """Send the user message."""
        # We need to handle the ShowMenu user message with bitbuffers
        # differently, because the maximum size is 255. If the message exceeds
        # this length, we need to sent it in several parts.
        if UserMessage.is_protobuf():
            user_message = UserMessage(
                RecipientFilter(*player_indexes), self.message_name)
            self.protobuf(user_message.buffer, self)
            user_message.send()
        else:
            self.bitbuf(player_indexes, self)

    def protobuf(self, buffer, kwargs):
        buffer.set_int32('bits_valid_slots', kwargs.valid_slots)
        buffer.set_int32('display_time', kwargs.display_time)
        buffer.set_string('menu_string', kwargs.menu_string)

    def bitbuf(self, player_indexes, kwargs):
        menu_string = kwargs.menu_string
        length = len(menu_string)
        while True:
            user_message = UserMessage(
                RecipientFilter(*player_indexes), self.message_name)

            buffer = user_message.buffer
            buffer.write_word(kwargs.valid_slots)
            buffer.write_char(kwargs.display_time)
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
        super().__init__(locals())

    def protobuf(self, buffer, kwargs):
        buffer.set_string('msg_name', kwargs.message)
        buffer.set_bool('chat', kwargs.chat)
        buffer.set_int32('ent_idx', kwargs.index)
        buffer.add_string('params', kwargs.param1)
        buffer.add_string('params', kwargs.param2)
        buffer.add_string('params', kwargs.param3)
        buffer.add_string('params', kwargs.param4)
        # TODO: Handle textchatall

    def bitbuf(self, buffer, kwargs):
        buffer.write_byte(kwargs.index)
        buffer.write_byte(kwargs.chat)
        buffer.write_string(kwargs.message)
        buffer.write_string(kwargs.param1)
        buffer.write_string(kwargs.param2)
        buffer.write_string(kwargs.param3)
        buffer.write_string(kwargs.param4)


class HintText(UserMessageCreator):

    """Create a HintText."""

    message_name = 'HintText'

    def __init__(self, message):
        super().__init__(locals())

    def protobuf(self, buffer, kwargs):
        buffer.set_string('text', kwargs.message)

    def bitbuf(self, buffer, kwargs):
        buffer.write_string(kwargs.message)


class SayText(UserMessageCreator):

    """Create a SayText."""

    message_name = 'SayText'

    def __init__(self, message, index=0, chat=False):
        super().__init__(locals())

    def protobuf(self, buffer, kwargs):
        buffer.set_int32('ent_idx', kwargs.index)
        buffer.set_bool('chat', kwargs.chat)
        buffer.set_string('text', kwargs.message)

    def bitbuf(self, buffer, kwargs):
        buffer.write_byte(kwargs.index)
        buffer.write_string(kwargs.message)
        buffer.write_byte(kwargs.chat)


class Shake(UserMessageCreator):

    """Create a Shake."""

    message_name = 'Shake'

    def __init__(self, shake_command, amplitude, frequency, duration):
        super().__init__(locals())

    def protobuf(self, buffer, kwargs):
        buffer.set_int32('command', kwargs.shake_command)
        buffer.set_float('local_amplitude', kwargs.amplitude)
        buffer.set_float('frequency', kwargs.frequency)
        buffer.set_float('duration', kwargs.duration)

    def bitbuf(self, buffer, kwargs):
        buffer.write_byte(kwargs.shake_command)
        buffer.write_float(kwargs.amplitude)
        buffer.write_float(kwargs.frequency)
        buffer.write_float(kwargs.duration)


class ResetHUD(UserMessageCreator):

    """Create a ResetHUD."""

    message_name = 'ResetHud'

    def __init__(self, reset=True):
        super().__init__(locals())

    def protobuf(self, buffer, kwargs):
        buffer.set_bool('reset', kwargs.reset)

    def bitbuf(self, buffer, kwargs):
        buffer.write_byte(kwargs.reset)


class TextMsg(UserMessageCreator):

    """Create a TextMsg."""

    message_name = 'TextMsg'

    def __init__(self, destination, name, param1='', param2='', param3='',
            param4=''):
        super().__init__(locals())

    def protobuf(self, buffer, kwargs):
        buffer.set_int32('msg_dst', kwargs.destination)
        buffer.add_string('params', kwargs.name)
        buffer.add_string('params', kwargs.param1)
        buffer.add_string('params', kwargs.param2)
        buffer.add_string('params', kwargs.param3)
        buffer.add_string('params', kwargs.param4)

    def bitbuf(self, buffer, kwargs):
        buffer.write_byte(kwargs.destination)
        buffer.write_string(kwargs.name)
        buffer.write_string(kwargs.param1)
        buffer.write_string(kwargs.param2)
        buffer.write_string(kwargs.param3)
        buffer.write_string(kwargs.param4)


class KeyHintText(UserMessageCreator):

    """Create a KeyHintText."""

    message_name = 'KeyHintText'

    def __init__(self, *hints):
        super().__init__(locals())

    def protobuf(self, buffer, kwargs):
        for hint in kwargs.hints:
            buffer.add_string('hints', hint)

    def bitbuf(self, buffer, kwargs):
        buffer.write_byte(len(kwargs.hints))
        for hint in kwargs.hints:
            buffer.write_string(hint)


class Fade(UserMessageCreator):

    """Create a Fade."""

    message_name = 'Fade'

    def __init__(self, duration, hold_time, flags,
            color=Color(255, 255, 255, 255)):
        super().__init__(locals())

    def protobuf(self, buffer, kwargs):
        buffer.set_int32('duration', kwargs.duration)
        buffer.set_int32('hold_time', kwargs.hold_time)
        buffer.set_int32('flags', kwargs.flags)
        color_buffer = buffer.mutable_message('clr')
        color_buffer.set_int32('r', kwargs.color.r)
        color_buffer.set_int32('g', kwargs.color.g)
        color_buffer.set_int32('b', kwargs.color.b)
        color_buffer.set_int32('a', kwargs.color.a)

    def bitbuf(self, buffer, kwargs):
        buffer.write_short(kwargs.duration)
        buffer.write_short(kwargs.hold_time)
        buffer.write_short(kwargs.flags)
        buffer.write_byte(kwargs.color.r)
        buffer.write_byte(kwargs.color.g)
        buffer.write_byte(kwargs.color.b)
        buffer.write_byte(kwargs.color.a)


class HudMsg(UserMessageCreator):

    """Create a HudMsg."""

    message_name = 'HudMsg'

    # TODO: Use Vector2D for x and y?
    def __init__(self, channel=0, x=-1, y=-1,
            color1=Color(255, 255, 255, 255),
            color2=Color(255, 255, 255, 255), effect=0, fade_in=0, fade_out=0,
            hold_time=4, fx_time=0, message=""):
        super().__init__(locals())

    def protobuf(self, buffer, kwargs):
        buffer.set_int32('channel', kwargs.channel)

        pos_buffer = buffer.mutable_message('pos')
        pos_buffer.set_float('x', kwargs.x)
        pos_buffer.set_float('y', kwargs.y)

        color1_buffer = buffer.mutable_message('clr1')
        color1_buffer.set_int32('r', kwargs.color1.r)
        color1_buffer.set_int32('g', kwargs.color1.g)
        color1_buffer.set_int32('b', kwargs.color1.b)
        color1_buffer.set_int32('a', kwargs.color1.a)

        color2_buffer = buffer.mutable_message('clr2')
        color2_buffer.set_int32('r', kwargs.color2.r)
        color2_buffer.set_int32('g', kwargs.color2.g)
        color2_buffer.set_int32('b', kwargs.color2.b)
        color2_buffer.set_int32('a', kwargs.color2.a)

        buffer.set_int32('effect', kwargs.effect)
        buffer.set_float('fade_in_time', kwargs.fade_in)
        buffer.set_float('fade_out_time', kwargs.fade_out)
        buffer.set_float('hold_time', kwargs.hold_time)
        buffer.set_float('fx_time', kwargs.fx_time)
        buffer.set_string('text', kwargs.message)

    def bitbuf(self, buffer, kwargs):
        buffer.write_byte(kwargs.channel)
        buffer.write_float(kwargs.x)
        buffer.write_float(kwargs.y)
        buffer.write_byte(kwargs.color1.r)
        buffer.write_byte(kwargs.color1.g)
        buffer.write_byte(kwargs.color1.b)
        buffer.write_byte(kwargs.color1.a)
        buffer.write_byte(kwargs.color2.r)
        buffer.write_byte(kwargs.color2.g)
        buffer.write_byte(kwargs.color2.b)
        buffer.write_byte(kwargs.color2.a)
        buffer.write_byte(kwargs.effect)
        buffer.write_float(kwargs.fade_in)
        buffer.write_float(kwargs.fade_out)
        buffer.write_float(kwargs.hold_time)
        buffer.write_float(kwargs.fx_time)
        buffer.write_string(kwargs.message)
