# ../memory/hooks.py

"""Provides memory hooking functionality."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Python
from contextlib import contextmanager

# Source.Python Imports
#   Core
from core import AutoUnload
#   Memory
from _memory import HookType
from _memory import set_hooks_disabled
from _memory import get_hooks_disabled
from memory import Function


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('HookType',
           'PostHook',
           'PreHook',
           'set_hooks_disabled',
           'get_hooks_disabled',
           'hooks_disabled'
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


# =============================================================================
# >> FUNCTIONS
# =============================================================================
@contextmanager
def hooks_disabled(disabled=True):
    """Temporarily disable or enable all hook callbacks. By default hooks are
    enabled. Thus, this context manager is mainly used to temporarily disable
    hook callbacks. If the context ends, the original value is restored. This
    can be used e. g. to avoid recursive calls when damaging a player in a
    ``on_take_damage`` hook or ``player_hurt`` event.

    .. note::

        This would only disable hooks created with Source.Python. Hooks that
        have been created by other server plugins will still be called.

    Example:

    .. code:: python

        from players.entity import Player
        from memory.hooks import hooks_disabled

        # Get a Player instance of the player with index 1
        player = Player(1)

        # Damage the player. This would call e. g. on_take_damage hooks
        player.take_damage(5)

        # To avoid calling the on_take_damage hooks, use the following:
        with hooks_disabled()
            player.take_damage(5)
    """
    old = get_hooks_disabled()
    set_hooks_disabled(disabled)
    try:
        yield
    finally:
        set_hooks_disabled(old)
