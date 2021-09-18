# ../effects/hooks.py

"""Provides hooking effect functionality."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
#   Core
from core import AutoUnload
#   Effects
from effects.base import TempEntity
from effects.templates import temp_entity_templates
#   Engines
from engines.server import engine_server
#   Filters
from filters.recipients import RecipientFilter
#   Memory
from memory import get_virtual_function
from memory import make_object
from memory.hooks import HookType
from memory.hooks import PreHook


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('TempEntityPreHook',
           )


# =============================================================================
# >> CLASSES
# =============================================================================
class TempEntityPreHook(AutoUnload):
    """Decorator used to create temp entity pre hooks that auto unload."""

    def __init__(self, temp_entity_name):
        """Initialize the hook object.

        :param str temp_entity_name:
            The name of the temp entity to hook.
        """
        # Get and store the temp entity template...
        self.template = temp_entity_templates[temp_entity_name]

        # Set the callback to None...
        self.callback = None

    def __call__(self, callback):
        """Store the callback and try initialize the hook."""
        # Store the callback...
        self.callback = callback

        # Initialize the hook...
        self.template.add_hook(callback)

        # Return the callback...
        return callback

    def _unload_instance(self):
        """Unload the hook."""
        # Exit the call if the callback wasn't registered...
        if self.callback is None:
            return

        # Unregister the hook...
        self.template.remove_hook(self.callback)


class TempEntityPostHook(TempEntityPreHook):
    """Decorator used to create temp entity post hooks that auto unload."""

    def __init__(self, temp_entity_name):
        """Initialize the hook object.

        :param str temp_entity_name:
            The name of the temp entity to hook.
        """
        from warnings import warn
        warn(
            'TempEntityPostHook has been deprecated and will be entirely '
            'unsupported in a future update. Use TempEntityPreHook instead '
            'or register your own post hook on '
            'CEngineServer::PlaybackTempEntity/CBaseTempEntity::Create.')

        super().__init__(temp_entity_name)


# =============================================================================
# >> HOOKS
# =============================================================================
@PreHook(get_virtual_function(engine_server, 'PlaybackTempEntity'))
def pre_playback_temp_entity(stack_data):
    """Handle pre hooks."""
    temp_entity = TempEntity(stack_data[3])
    try:
        recipients = make_object(RecipientFilter, stack_data[1])
    except RuntimeError:
        global _recipients
        _recipients = RecipientFilter.from_abstract_pointer(stack_data[1])
        stack_data[1] = recipients = _recipients
    return temp_entity.template.handle_hook(temp_entity, recipients)
