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
from memory.hooks import PostHook


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('TempEntityPostHook',
           'TempEntityPreHook',
           )


# =============================================================================
# >> CLASSES
# =============================================================================
class _TempEntityHook(AutoUnload):
    """Create temp entity pre and post hooks that auto unload."""

    def __init__(self, temp_entity_name):
        """Initialize the hook object.

        :param str temp_entity_name:
            The name of the temp entity to hook.
        """
        # Store the given temp entity name...
        self.name = temp_entity_name

        # Get and store the temp entity template...
        self.template = temp_entity_templates[temp_entity_name]

        # Set the callback to None...
        self.callback = None

    def __call__(self, callback):
        """Store the callback and try initialize the hook."""
        # Store the callback...
        self.callback = callback

        # Initialize the hook...
        self.template.add_hook(self.hook_type, callback)

        # Return the callback...
        return callback

    @property
    def hook_type(self):
        """Raise an error if the inheriting class does not have their own."""
        raise NotImplementedError('No hook_type defined for class.')

    def _unload_instance(self):
        """Unload the hook."""
        self.template.remove_hook(self.hook_type, self.callback)


class TempEntityPreHook(_TempEntityHook):
    """Decorator used to create temp entity pre hooks that auto unload."""

    hook_type = HookType.PRE


class TempEntityPostHook(_TempEntityHook):
    """Decorator used to create temp entity post hooks that auto unload."""

    hook_type = HookType.POST


# =============================================================================
# >> HOOKS
# =============================================================================
@PreHook(get_virtual_function(engine_server, 'PlaybackTempEntity'))
def pre_playback_temp_entity(stack_data):
    """Handle pre hooks."""
    temp_entity = TempEntity(stack_data[3])
    return temp_entity.template.handle_hook(HookType.PRE, temp_entity,
        make_object(RecipientFilter, stack_data[1]))

@PostHook(get_virtual_function(engine_server, 'PlaybackTempEntity'))
def post_playback_temp_entity(stack_data, return_value):
    """Handle post hooks."""
    temp_entity = TempEntity(stack_data[3])
    return temp_entity.template.handle_hook(HookType.POST, temp_entity,
        make_object(RecipientFilter, stack_data[1]))
