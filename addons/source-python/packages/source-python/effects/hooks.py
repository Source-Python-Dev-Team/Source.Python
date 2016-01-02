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
#   Filters
from filters.recipients import RecipientFilter
#   Memory
from memory import get_virtual_function
from memory import make_object
from memory.hooks import HookType


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

        :param str temp_entity_name: The name of the temp entity to hook.
        """
        # Store the given temp entity name...
        self.name = temp_entity_name

        # Store the function to hook...
        self.function = get_virtual_function(
            temp_entity_templates[temp_entity_name], 'Create')

        # Set the callback to None...
        self._callback = None

    def __call__(self, callback):
        """Store the callback and try initialize the hook."""
        def _callback(stack_data, *args):
            """Called when the hooked method is called."""
            # Get the temp entity instance...
            temp_entity = make_object(TempEntity, stack_data[0])

            # Are we looking for that temp entity?
            if temp_entity.name == self.name:

                # Call the registered callback...
                return callback(temp_entity,
                    make_object(RecipientFilter, stack_data[1]))

        # Store the callback...
        self._callback = _callback

        # Initialize the hook...
        self.function.add_hook(self.hook_type, self._callback)

        # Return the callback...
        return _callback

    @property
    def hook_type(self):
        """Raise an error if the inheriting class does not have their own."""
        raise NotImplementedError('No hook_type defined for class.')

    def _unload_instance(self):
        """Unload the hook."""
        # Unregister the hook...
        self.function.remove_hook(self.hook_type, self._callback)


class TempEntityPreHook(_TempEntityHook):
    """Decorator used to create temp entity pre hooks that auto unload."""

    hook_type = HookType.PRE


class TempEntityPostHook(_TempEntityHook):
    """Decorator used to create temp entity post hooks that auto unload."""

    hook_type = HookType.POST
