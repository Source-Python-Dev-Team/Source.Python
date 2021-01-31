# ../messages/base.py

"""Provides user messages."""

# ============================================================================
# >> IMPORTS
# ============================================================================
# Python Imports
import collections

# Source.Python Imports
#   Colors
from colors import WHITE
#   Filters
from filters.recipients import RecipientFilter
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
from _messages import SCREENFADE_FRACBITS
from _messages import ShakeCommand
from _messages import HudDestination
from _messages import FadeFlags


# =============================================================================
# >> CLASSES
# =============================================================================
class AttrDict(dict):
    """A dictionary that redirects __getattr__ and __setattr__."""

    __getattr__ = dict.__getitem__
    __setattr__ = dict.__setitem__


class UserMessageCreator(AttrDict):
    """Provide an easy interface to create user messages.

    :attr bool reliable: Whether to send message using reliable channel.
    """

    reliable = False

    def __init__(self, **kwargs):
        """Initialize the usermessage creator.

        :param dict kwargs: All valid fields.
        """
        super().__setattr__('valid_fields', kwargs.keys())
        super().__init__(kwargs)

    def __setitem__(self, item, value):
        """Set a field value."""
        if item not in self.valid_fields:
            raise NameError('Invalid field name "{0}".'.format(item))

        super().__setitem__(item, value)

    def __setattr__(self, attr, value):
        """Set a field value."""
        self[attr] = value

    def send(self, *player_indexes, **tokens):
        """Send the user message."""
        player_indexes = RecipientFilter(*player_indexes)
        for language, indexes in self._categorize_players_by_language(
                player_indexes).items():
            translated_kwargs = AttrDict(self)
            translated_kwargs.update(
                self._get_translated_kwargs(language, tokens))
            self._send(indexes, translated_kwargs)

    def _send(self, player_indexes, translated_kwargs):
        """Send the user message to the given players.

        :param iterable player_indexes: All players with the same language
            setting.
        :param AttrDict translated_kwargs: The translated arguments.
        """
        recipients = RecipientFilter(*player_indexes)
        recipients.reliable = self.reliable
        user_message = UserMessage(recipients, self.message_name)

        if user_message.is_protobuf():
            self.protobuf(user_message.buffer, translated_kwargs)
            user_message.send()
        else:
            try:
                self.bitbuf(user_message.buffer, translated_kwargs)
            except:
                # In case of an error during writing to the buffer (e. g. by using
                # the wrong data type for the write_* methods) reset the buffer
                # and send the message. This causes the engine to silently ignore
                # the user message and the server doesn't crash upon creating
                # another user message.
                # See also:
                # https://github.com/Source-Python-Dev-Team/Source.Python/issues/315
                user_message.buffer.reset()

                # Re-raise the exception to make the user aware of the problem
                raise
            finally:
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
        """Return translated and tokenized arguments."""
        translated_kwargs = AttrDict()
        for key in self.translatable_fields:
            translated_kwargs[key] = self._translate(
                self[key], language, tokens)

        return translated_kwargs

    @staticmethod
    def _translate(value, language, tokens):
        """Translate the value if it's a :class:`TranslationStrings` object."""
        if isinstance(value, TranslationStrings):
            return value.get_string(language, **tokens)

        try:
            return value.format(**tokens)
        except (KeyError, AttributeError):
            return value

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

    @property
    def translatable_fields(self):
        """Return a list of translatable fields."""
        raise NotImplementedError('Must be implemented by a subclass.')


class VGUIMenu(UserMessageCreator):
    """Create a VGUIMenu."""

    message_name = 'VGUIMenu'
    translatable_fields = []

    def __init__(self, name, subkeys=None, show=True):
        """Initialize the VGUI menu.

        :param str name: Name of the menu.
        :param dict subkeys: Additional menu data.
        :param bool show: If True the menu will be shown, else it will be
            hidden.
        """
        if subkeys is None:
            subkeys = {}

        # TODO: Which names and subkeys are available?
        super().__init__(name=name, subkeys=subkeys, show=show)

    def protobuf(self, buffer, kwargs):
        """Send the VGUIMenu with protobuf."""
        buffer.set_string('name', kwargs.name)
        buffer.set_bool('show', kwargs.show)
        for key, value in kwargs.subkeys.items():
            temp_buffer = buffer.add_message('subkeys')
            temp_buffer.set_string('name', key)
            temp_buffer.set_string('str', value)

    def bitbuf(self, buffer, kwargs):
        """Send the VGUIMenu with bitbuf."""
        buffer.write_string(kwargs.name)
        buffer.write_byte(kwargs.show)
        buffer.write_byte(len(kwargs.subkeys))
        for key, value in kwargs.subkeys.items():
            buffer.write_string(key)
            buffer.write_string(value)


class ShowMenu(UserMessageCreator):
    """Create a radio menu."""

    message_name = 'ShowMenu'
    chunk_size = 62
    translatable_fields = []

    def __init__(self, menu_string, valid_slots=1023, display_time=4):
        """Initialize the radio menu."""
        super().__init__(
            menu_string=menu_string,
            valid_slots=valid_slots,
            display_time=display_time)

    def send(self, *player_indexes):
        """Send the user message."""
        # We need to handle the ShowMenu user message with bitbuffers
        # differently, because the maximum size is 255. If the message exceeds
        # this length, we need to sent it in several parts.
        if UserMessage.is_protobuf():
            recipients = RecipientFilter(*player_indexes)
            recipients.reliable = self.reliable
            user_message = UserMessage(recipients, self.message_name)
            self.protobuf(user_message.buffer, self)
            user_message.send()
        else:
            self.bitbuf(player_indexes, self)

    def protobuf(self, buffer, kwargs):
        """Send the ShowMenu with protobuf."""
        buffer.set_int32('bits_valid_slots', kwargs.valid_slots)
        buffer.set_int32('display_time', kwargs.display_time)
        buffer.set_string('menu_string', kwargs.menu_string)

    def bitbuf(self, player_indexes, kwargs):
        """Send the ShowMenu with bitbuf."""
        menu_string = kwargs.menu_string
        length = len(menu_string)
        recipients = RecipientFilter(*player_indexes)
        recipients.reliable = self.reliable
        while True:
            user_message = UserMessage(recipients, self.message_name)

            buffer = user_message.buffer
            buffer.write_word(kwargs.valid_slots)
            buffer.write_char(kwargs.display_time)
            buffer.write_byte(length > self.chunk_size)
            buffer.write_string(menu_string[:self.chunk_size])

            user_message.send()

            if length > self.chunk_size:
                menu_string = menu_string[self.chunk_size:]
                length -= self.chunk_size
            else:
                break


class SayText2(UserMessageCreator):
    """Create a SayText2."""

    message_name = 'SayText2'
    translatable_fields = ['message', 'param1', 'param2', 'param3', 'param4']
    reliable = True

    def __init__(
            self, message, index=0, chat=False,
            param1='', param2='', param3='', param4=''):
        """Initialize the SayText2 instance."""
        super().__init__(
            message=message, index=index, chat=chat,
            param1=param1, param2=param2, param3=param3, param4=param4)

    def protobuf(self, buffer, kwargs):
        """Send the SayText2 with protobuf."""
        buffer.set_string('msg_name', ' \x01' + kwargs.message)
        buffer.set_bool('chat', kwargs.chat)
        buffer.set_int32('ent_idx', kwargs.index)
        buffer.add_string('params', kwargs.param1)
        buffer.add_string('params', kwargs.param2)
        buffer.add_string('params', kwargs.param3)
        buffer.add_string('params', kwargs.param4)
        # TODO: Handle textchatall

    def bitbuf(self, buffer, kwargs):
        """Send the SayText2 with bitbuf."""
        buffer.write_byte(kwargs.index)
        buffer.write_byte(kwargs.chat)
        buffer.write_string('\x01' + kwargs.message)
        buffer.write_string(kwargs.param1)
        buffer.write_string(kwargs.param2)
        buffer.write_string(kwargs.param3)
        buffer.write_string(kwargs.param4)


class HintText(UserMessageCreator):
    """Create a HintText."""

    message_name = 'HintText'
    translatable_fields = ['message']

    def __init__(self, message):
        """Initialize the HintText instance."""
        super().__init__(message=message)

    def protobuf(self, buffer, kwargs):
        """Send the HintText with protobuf."""
        buffer.set_string('text', kwargs.message)

    def bitbuf(self, buffer, kwargs):
        """Send the HintText with bitbuf."""
        buffer.write_string(kwargs.message)


class SayText(UserMessageCreator):
    """Create a SayText."""

    message_name = 'SayText'
    translatable_fields = ['message']
    reliable = True

    def __init__(self, message, index=0, chat=False):
        """Initialize the SayText instance."""
        super().__init__(message=message, index=index, chat=chat)

    def protobuf(self, buffer, kwargs):
        """Send the SayText with protobuf."""
        buffer.set_int32('ent_idx', kwargs.index)
        buffer.set_bool('chat', kwargs.chat)
        buffer.set_string('text', ' \x01' + kwargs.message)

    def bitbuf(self, buffer, kwargs):
        """Send the SayText with bitbuf."""
        buffer.write_byte(kwargs.index)
        buffer.write_string('\x01' + kwargs.message)
        buffer.write_byte(kwargs.chat)


class Shake(UserMessageCreator):
    """Create a Shake."""

    message_name = 'Shake'
    translatable_fields = []

    def __init__(
            self, amplitude, duration, frequency=1,
            shake_command=ShakeCommand.START):
        """Initialize the Shake instance."""
        super().__init__(
            amplitude=amplitude, duration=duration,
            frequency=frequency, shake_command=shake_command)

    def protobuf(self, buffer, kwargs):
        """Send the Shake with protobuf."""
        buffer.set_int32('command', kwargs.shake_command)
        buffer.set_float('local_amplitude', kwargs.amplitude)
        buffer.set_float('frequency', kwargs.frequency)
        buffer.set_float('duration', kwargs.duration)

    def bitbuf(self, buffer, kwargs):
        """Send the Shake with bitbuf."""
        buffer.write_byte(kwargs.shake_command)
        buffer.write_float(kwargs.amplitude)
        buffer.write_float(kwargs.frequency)
        buffer.write_float(kwargs.duration)


class ResetHUD(UserMessageCreator):
    """Create a ResetHUD."""

    message_name = 'ResetHud'
    translatable_fields = []

    def __init__(self, reset=True):
        """Initialize the ResetHUD instance."""
        super().__init__(reset=reset)

    def protobuf(self, buffer, kwargs):
        """Send the ResetHUD with protobuf."""
        buffer.set_bool('reset', kwargs.reset)

    def bitbuf(self, buffer, kwargs):
        """Send the ResetHUD with bitbuf."""
        buffer.write_byte(kwargs.reset)


class TextMsg(UserMessageCreator):
    """Create a TextMsg."""

    message_name = 'TextMsg'
    translatable_fields = ['message', 'param1', 'param2', 'param3', 'param4']
    reliable = True

    def __init__(
            self, message, destination=HudDestination.CENTER,
            param1='', param2='', param3='', param4=''):
        """Initialize the TextMsg instance."""
        super().__init__(
            message=message, destination=destination, param1=param1,
            param2=param2, param3=param3, param4=param4)

    def protobuf(self, buffer, kwargs):
        """Send the TextMsg with protobuf."""
        buffer.set_int32('msg_dst', kwargs.destination)
        buffer.add_string('params', kwargs.message)
        buffer.add_string('params', kwargs.param1)
        buffer.add_string('params', kwargs.param2)
        buffer.add_string('params', kwargs.param3)
        buffer.add_string('params', kwargs.param4)

    def bitbuf(self, buffer, kwargs):
        """Send the TextMsg with bitbuf."""
        buffer.write_byte(kwargs.destination)
        buffer.write_string(kwargs.message)
        buffer.write_string(kwargs.param1)
        buffer.write_string(kwargs.param2)
        buffer.write_string(kwargs.param3)
        buffer.write_string(kwargs.param4)


class KeyHintText(UserMessageCreator):
    """Create a KeyHintText."""

    message_name = 'KeyHintText'

    def __init__(self, *hints):
        """Initialize the KeyHintText instance."""
        super().__init__(hints=hints)

    def protobuf(self, buffer, kwargs):
        """Send the KeyHintText with protobuf."""
        for hint in kwargs.hints:
            buffer.add_string('hints', hint)

    def bitbuf(self, buffer, kwargs):
        """Send the KeyHintText with bitbuf."""
        buffer.write_byte(len(kwargs.hints))
        for hint in kwargs.hints:
            buffer.write_string(hint)

    def _get_translated_kwargs(self, language, tokens):
        """Return translated and tokenized arguments."""
        hints = []
        for hint in self.hints:
            hints.append(self._translate(hint, language, tokens))

        return dict(hints=hints)


class Fade(UserMessageCreator):
    """Create a Fade."""

    message_name = 'Fade'
    translatable_fields = []
    moved_frac_bits = 1 << SCREENFADE_FRACBITS

    def __init__(self, duration, hold_time, color=WHITE, flags=FadeFlags.IN):
        """Initialize the Fade instance."""
        super().__init__(
            duration=duration, hold_time=hold_time, color=color, flags=flags)

    def protobuf(self, buffer, kwargs):
        """Send the Fade with protobuf."""
        buffer.set_int32('duration', kwargs.duration * self.moved_frac_bits)
        buffer.set_int32('hold_time', kwargs.hold_time * self.moved_frac_bits)
        buffer.set_int32('flags', kwargs.flags)
        color_buffer = buffer.mutable_message('clr')
        color_buffer.set_int32('r', kwargs.color.r)
        color_buffer.set_int32('g', kwargs.color.g)
        color_buffer.set_int32('b', kwargs.color.b)
        color_buffer.set_int32('a', kwargs.color.a)

    def bitbuf(self, buffer, kwargs):
        """Send the Fade with bitbuf."""
        buffer.write_short(kwargs.duration * self.moved_frac_bits)
        buffer.write_short(kwargs.hold_time * self.moved_frac_bits)
        buffer.write_short(kwargs.flags)
        buffer.write_byte(kwargs.color.r)
        buffer.write_byte(kwargs.color.g)
        buffer.write_byte(kwargs.color.b)
        buffer.write_byte(kwargs.color.a)


class HudMsg(UserMessageCreator):
    """Create a HudMsg."""

    message_name = 'HudMsg'
    translatable_fields = ['message']

    # TODO: Use Vector2D for x and y?
    def __init__(
            self, message, x=-1, y=-1, color1=WHITE, color2=WHITE, effect=0,
            fade_in=0, fade_out=0, hold_time=4, fx_time=0, channel=0):
        """Initialize the HudMsg instance."""
        super().__init__(
            message=message, x=x, y=y, color1=color1,
            color2=color2, effect=effect, fade_in=fade_in, fade_out=fade_out,
            hold_time=hold_time, fx_time=fx_time, channel=channel)

    def protobuf(self, buffer, kwargs):
        """Send the HudMsg with protobuf."""
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
        """Send the HudMsg with bitbuf."""
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
