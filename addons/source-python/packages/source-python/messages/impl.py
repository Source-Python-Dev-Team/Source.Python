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
        buffer.set_repeated_string('params', 0, data.param1)
        buffer.set_repeated_string('params', 1, data.param2)
        buffer.set_repeated_string('params', 2, data.param3)
        buffer.set_repeated_string('params', 3, data.param4)

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


# =============================================================================
# >> FUNCTIONS
# =============================================================================
implemented_usermessages = {
    get_message_index('SayText2'): SayText2Impl
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
