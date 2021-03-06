# ../messages/hooks.py

"""Provides user message hooking functionality."""

# TODO:
# - Implement more user messages
# - Use these message implementations for the UserMessageCreator subclasses.

# =============================================================================
# >> IMPORTS
# =============================================================================
# Python
#   Collections
from collections import defaultdict

# Source.Python
#   Core
from core import AutoUnload
#   Engines
from engines.server import engine_server
#   Filters
from filters.recipients import BaseRecipientFilter
from filters.recipients import RecipientFilter
#   Bitbuffers
from bitbuffers import BitBufferWrite
from bitbuffers import BitBufferRead
#   Listeners
from listeners import ListenerManager
#   Memory
from memory import make_object
from memory import get_object_pointer
from memory import get_size
from memory import get_virtual_function
from memory.hooks import PreHook
from memory.hooks import PostHook
#   Messages
from messages import UserMessage
from messages import get_message_index
from messages import get_message_name
from messages.impl import get_user_message_impl

if UserMessage.is_protobuf():
    from _messages import ProtobufMessage


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = (
    'HookUserMessageBase',
    'HookBitBufferUserMessage',
    'HookProtobufUserMessage',
    'HookUserMessage',
)


# =============================================================================
# >> GLOBAL VARIABLES
# =============================================================================
_user_message_data = None
_recipients = RecipientFilter()


# =============================================================================
# >> CLASSES
# =============================================================================
class HookUserMessageBase(AutoUnload):
    """Base decorator for user message hooks."""

    def __init__(self, user_message):
        """Create a new user message hook.

        :param int/str user_message:
            The user message index or name to hook.
        :raise TypeError:
            Raised if ``user_message`` is not and int or str.
        :raise ValueError:
            Raised if the user message does not exist.
        """
        if isinstance(user_message, int):
            index = user_message
        elif isinstance(user_message, str):
            index = get_message_index(user_message)
        else:
            raise TypeError(
                'Invalid type for "user_message". int or str required.')

        self.message_index = index
        self.message_name = get_message_name(index)
        self.callback = None

        # Verify that it's a valid index
        if self.message_name is None:
            raise ValueError(f'Invalid user message: {user_message}')

    def __call__(self, callback):
        """Finalize the hook registration by registering the callback.

        :param object callback:
            A callable object that will be called when a user message is
            created.
        :return:
            The callback that has been passed.
        """
        if not callable(callback):
            raise ValueError('Callback must be callable.')

        self.callback = callback
        self.hooks[self.message_index].register_listener(callback)
        return self.callback

    def _unload_instance(self):
        """Unregister the user message hook."""
        if self.callback is None:
            return

        self.hooks[self.message_index].unregister_listener(self.callback)

    @property
    def hooks(self):
        """Return all hooks for a user message.

        :rtype: ListenerManager
        """
        raise NotImplementedError('Must be implemented by a subclass.')


class HookBitBufferUserMessage(HookUserMessageBase):
    """Decorator to register a raw user message hook for bitbuffer messages."""

    hooks = defaultdict(ListenerManager)


class HookProtobufUserMessage(HookUserMessageBase):
    """Decorator to register a raw user message hook for protobuf messages."""

    hooks = defaultdict(ListenerManager)


class HookUserMessage(HookUserMessageBase):
    """Decorator to register a convenient user message hook."""

    hooks = defaultdict(ListenerManager)

    def __init__(self, user_message):
        """Create a new user message hook.

        :raise NotImplementedError:
            Raised if the user message has not been implemented yet in
            Source.Python.

        .. seealso:: :meth:`HookUserMessageBase.__init__`
        """
        super().__init__(user_message)

        # Verify that the user message is supported/implemented. This will
        # raise a NotImplementedError if it isn't.
        self.impl = get_user_message_impl(self.message_index)


# =============================================================================
# >> HOOKS
# =============================================================================
if UserMessage.is_protobuf():
    @PreHook(get_virtual_function(engine_server, 'SendUserMessage'))
    def _pre_send_user_message(args):
        message_index = args[2]

        user_message_hooks = HookUserMessage.hooks[message_index]
        protobuf_user_message_hooks = HookProtobufUserMessage.hooks[message_index]

        # No need to do anything behind this if no listener is registered
        if not user_message_hooks and not protobuf_user_message_hooks:
            return

        try:
            # Replace original recipients filter
            tmp_recipients = make_object(BaseRecipientFilter, args[1])
        except RuntimeError:
            # Patch for issue #314
            tmp_recipients = RecipientFilter.from_abstract_pointer(args[1])
        _recipients.update(*tuple(tmp_recipients), clear=True)
        args[1] = _recipients

        buffer = make_object(ProtobufMessage, args[3])

        protobuf_user_message_hooks.notify(_recipients, buffer)

        # No need to do anything behind this if no listener is registered
        if not user_message_hooks:
            return

        try:
            impl = get_user_message_impl(message_index)
        except NotImplementedError:
            return

        data = impl.read(buffer)
        user_message_hooks.notify(_recipients, data)

        # Update buffer if data has been changed
        if data.has_been_changed():
            buffer.clear()
            impl.write(buffer, data)

else:
    @PreHook(get_virtual_function(engine_server, 'UserMessageBegin'))
    def _pre_user_message_begin(args):
        try:
            # Replace original recipients filter
            tmp_recipients = make_object(BaseRecipientFilter, args[1])
        except RuntimeError:
            # Patch for issue #314
            tmp_recipients = RecipientFilter.from_abstract_pointer(args[1])
        _recipients.update(*tuple(tmp_recipients), clear=True)
        args[1] = _recipients

    @PostHook(get_virtual_function(engine_server, 'UserMessageBegin'))
    def _post_user_message_begin(args, return_value):
        global _user_message_data
        _user_message_data = (args[2], return_value)

    @PreHook(get_virtual_function(engine_server, 'MessageEnd'))
    def _pre_message_end(args):
        # This happens when we initialize our hooks, while a user message is
        # currently being created
        if _user_message_data is None:
            return

        message_index, buffer_write_ptr = _user_message_data

        # Retrieve the ListenerManager instances
        user_message_hooks = HookUserMessage.hooks[message_index]
        bitbuffer_user_message_hooks = HookBitBufferUserMessage.hooks[message_index]

        # No need to do anything behind this if no listener is registered
        if not user_message_hooks and not bitbuffer_user_message_hooks:
            return

        buffer_write = make_object(BitBufferWrite, buffer_write_ptr)
        buffer_read = BitBufferRead(buffer_write, False)

        org_current_bit = buffer_write.current_bit

        # For bitbuffers we need to make sure every callback starts reading and
        # writing from the very first bit.
        for callback in bitbuffer_user_message_hooks:
            buffer_read.seek_to_bit(0)
            buffer_write.seek_to_bit(0)
            callback(_recipients, buffer_read, buffer_write)

        # If none of the above callbacks wrote to the buffer, we need to restore
        # the current_bit to the original value.
        if buffer_write.current_bit == 0:
            buffer_write.seek_to_bit(org_current_bit)

        # No need to do anything behind this if no listener is registered
        if not user_message_hooks:
            return

        try:
            impl = get_user_message_impl(message_index)
        except NotImplementedError:
            return

        buffer_read.seek_to_bit(0)
        data = impl.read(buffer_read)
        user_message_hooks.notify(_recipients, data)

        # Update buffer if data has been changed
        if data.has_been_changed():
            buffer_write.seek_to_bit(0)
            impl.write(buffer_write, data)