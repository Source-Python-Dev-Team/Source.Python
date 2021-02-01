# ../messages/impl.py

"""Provides user message implementations."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python
from messages import UserMessage
from messages import get_message_index


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = (
    'UserMessageData',
    'UserMessageImpl',
    'SayText2Impl',
    'ShowMenuImpl',
    'VGUIMenuImpl',
    'implemented_usermessages',
    'get_user_message_impl',
)


# =============================================================================
# >> CLASSES
# =============================================================================
class UserMessageData(dict):
    """A dict that keeps track whether it has been modified."""

    def __init__(self, **kwargs):
        super().__init__(**kwargs)
        self._changed = False

    def __setitem__(self, index, value):
        super().__setitem__(index, value)
        self._changed = True

    def __setattr__(self, attr, value):
        if attr == '_changed':
            super().__setattr__(attr, value)
        else:
            self[attr] = value

    def has_been_changed(self):
        """Return whether the data has been changed.

        :rtype: bool
        """
        return self._changed

    __getattr__ = dict.__getitem__


class UserMessageImpl(object):
    """Base class for user message implementations."""

    @staticmethod
    def read_protobuf(buffer):
        """Read data from the given buffer.

        :param ProtobufMessage buffer:
            The buffer to read.
        :rtype: UserMessageData
        """
        raise NotImplementedError

    @staticmethod
    def write_protobuf(buffer, data):
        """Write data to the given buffer.

        :param ProtobufMessage buffer:
            The buffer to write to.
        :param UserMessageData data:
            User message data that should be written to the buffer.
        """
        raise NotImplementedError

    @staticmethod
    def read_bitbuffer(buffer):
        """Read data from the given buffer.

        :param BitBufferRead buffer:
            The buffer to read.
        :rtype: UserMessageData
        """
        raise NotImplementedError

    @staticmethod
    def write_bitbuffer(buffer, data):
        """Write data to the given buffer.

        :param BitBufferWrite buffer:
            The buffer to write to.
        :param UserMessageData data:
            User message data that should be written to the buffer.
        """
        raise NotImplementedError

    @classmethod
    def read(cls, buffer):
        """A wrapper for :meth:`read_protobuf` and :meth:`read_bitbuffer`.

        :param buffer:
            The buffer to read.
        :rtype: UserMessageData
        """
        if UserMessage.is_protobuf():
            return cls.read_protobuf(buffer)

        return cls.read_bitbuffer(buffer)

    @classmethod
    def write(cls, buffer, data):
        """A wrapper for :meth:`write_protobuf` and :meth:`write_bitbuffer`.

        :param buffer:
            The buffer to write.
        :rtype: UserMessageData
        """
        if UserMessage.is_protobuf():
            cls.write_protobuf(buffer, data)
        else:
            cls.write_bitbuffer(buffer, data)


class SayText2Impl(UserMessageImpl):
    """SayText2 implementation."""

    @staticmethod
    def read_protobuf(buffer):
        return UserMessageData(
            index=buffer.get_int32('ent_idx'),
            chat=buffer.get_bool('chat'),
            message=buffer.get_string('msg_name'),
            param1=buffer.get_repeated_string('params', 0),
            param2=buffer.get_repeated_string('params', 1),
            param3=buffer.get_repeated_string('params', 2),
            param4=buffer.get_repeated_string('params', 3))

    @staticmethod
    def write_protobuf(buffer, data):
        buffer.set_string('msg_name', data.message)
        buffer.set_bool('chat', data.chat)
        buffer.set_int32('ent_idx', data.index)
        buffer.add_string('params', data.param1)
        buffer.add_string('params', data.param2)
        buffer.add_string('params', data.param3)
        buffer.add_string('params', data.param4)

    @staticmethod
    def read_bitbuffer(buffer):
        return UserMessageData(
            index=buffer.read_byte(),
            chat=buffer.read_byte(),
            message=buffer.read_string(),
            param1=buffer.read_string(),
            param2=buffer.read_string(),
            param3=buffer.read_string(),
            param4=buffer.read_string())

    @staticmethod
    def write_bitbuffer(buffer, data):
        buffer.write_byte(data.index)
        buffer.write_byte(data.chat)
        buffer.write_string(data.message)
        buffer.write_string(data.param1)
        buffer.write_string(data.param2)
        buffer.write_string(data.param3)
        buffer.write_string(data.param4)


class ShowMenuImpl(UserMessageImpl):
    """ShowMenu implementation."""

    @staticmethod
    def read_protobuf(buffer):
        return UserMessageData(
            valid_slots=buffer.get_int32('bits_valid_slots'),
            display_time=buffer.get_int32('display_time'),
            menu_string=buffer.get_string('menu_string')
        )

    @staticmethod
    def write_protobuf(buffer, data):
        buffer.set_int32('bits_valid_slots', data.valid_slots)
        buffer.set_int32('display_time', data.display_time)
        buffer.set_string('menu_string', data.menu_string)

    @staticmethod
    def read_bitbuffer(buffer):
        return UserMessageData(
            valid_slots=buffer.read_word(),
            display_time=buffer.read_char(),
            chunked=buffer.read_byte(),
            menu_string=buffer.read_string()
        )

    @staticmethod
    def write_bitbuffer(buffer, data):
        buffer.write_word(data.valid_slots)
        buffer.write_char(data.display_time)
        buffer.write_byte(data.chunked)
        buffer.write_string(data.menu_string)


class VGUIMenuImpl(UserMessageImpl):
    """VGUIMenu implementation."""

    @staticmethod
    def read_protobuf(buffer):
        subkeys = {}
        for index in range(buffer.get_field_count('subkeys')):
            message = buffer.get_repeated_message('subkeys', index)
            subkeys[message.get_string('name')] = message.get_string('str')

        return UserMessageData(
            name=buffer.get_string('name'),
            show=buffer.get_bool('show'),
            subkeys=subkeys)

    @staticmethod
    def write_protobuf(buffer, data):
        buffer.set_string('name', data.name)
        buffer.set_bool('show', data.show)
        for key, value in data.subkeys.items():
            temp_buffer = buffer.add_message('subkeys')
            temp_buffer.set_string('name', key)
            temp_buffer.set_string('str', value)

    @staticmethod
    def read_bitbuffer(buffer):
        name = buffer.read_string()
        show = buffer.read_byte()
        length = buffer.read_byte()

        subkeys = {}
        for index in range(length):
            subkeys[buffer.read_string()] = buffer.read_string()

        return UserMessageData(
            name=name,
            show=show,
            subkeys=subkeys)

    @staticmethod
    def write_bitbuffer(buffer, data):
        buffer.write_string(data.name)
        buffer.write_byte(data.show)
        buffer.write_byte(len(data.subkeys))
        for key, value in data.subkeys.items():
            buffer.write_string(key)
            buffer.write_string(value)


# =============================================================================
# >> FUNCTIONS
# =============================================================================
#: A dictionary that contains all implemented user messages.
implemented_usermessages = {
    get_message_index('SayText2'): SayText2Impl,
    get_message_index('ShowMenu'): ShowMenuImpl,
    get_message_index('VGUIMenu'): VGUIMenuImpl
}


def get_user_message_impl(msg_index):
    """Return the user message implementation for the given message index.

    :param int msg_index:
        The index of the user message.
    :raise NotImplementedError:
        Raised if the given user message is not implemented.
    :rtype: UserMessageImpl
    """
    try:
        return implemented_usermessages[msg_index]
    except KeyError:
        raise NotImplementedError(
            f'User message with index {msg_index} is not implemented.')
