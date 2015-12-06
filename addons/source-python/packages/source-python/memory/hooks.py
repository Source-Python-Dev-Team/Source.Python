# ../memory/hooks.py

"""Provides memory hooking functionality."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
#   Core
from core import AutoUnload
#   Memory
from _memory import HookType
from memory import Function


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('HookType',
           'PostHook',
           'PreHook',
           )


# =============================================================================
# >> CLASSES
# =============================================================================
class _Hook(AutoUnload):
    """Create pre and post hooks that auto unload."""

    def __init__(self, function):
        """Verify the given function is a Function object and store it."""
        # Is the function to be hooked a Function instance?
        if not isinstance(function, Function):

            # Raise an error as we can only hook Function instances
            raise TypeError(
                "'" + type(function).__name__ +
                "' object is not a Function instance.")

        # Store the function
        self.callback = None
        self.function = function

    def __call__(self, callback):
        """Store the callback and hook it."""
        # Store the callback
        self.callback = callback

        # Hook the callback to the Function
        self.function.add_hook(self.hook_type, self.callback)

        # Return the callback
        return self.callback

    @property
    def hook_type(self):
        """Raise an error if the inheriting class does not have their own."""
        raise NotImplementedError('No hook_type defined for class.')

    def _unload_instance(self):
        """Unregister the hook on script unload."""
        self.function.remove_hook(self.hook_type, self.callback)


class PreHook(_Hook):
    """Decorator class used to create pre hooks that auto unload."""

    hook_type = HookType.PRE


class PostHook(_Hook):
    """Decorator class used to create post hooks that auto unload."""

    hook_type = HookType.POST
