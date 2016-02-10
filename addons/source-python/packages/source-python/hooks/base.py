# ../hooks/base.py

"""Contains a base hooking class used by the hooks package."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
#   Core
from core import AutoUnload


# =============================================================================
# >> CLASSES
# =============================================================================
class _HookBase(list):
    """Base hook class used to store callbacks for the specific hook type."""

    def append(self, callback):
        """Register the given callback to the list.

        :param callback: The function to register to the list.

        .. code-block:: python

            def function(*args, **kwargs):
                # Code...

            <_HookBase object>.append(function)
        """
        # Is the given callback callable?
        if not callable(callback):

            # Raise an exception
            raise ValueError(
                'Callback {0} is not callable.'.format(callback))

        # Is the given callback already registered?
        if callback in self:

            # Raise an exception
            raise ValueError(
                'Callback {0} is already registered to {1}.'.format(
                    callback, self.class_name))

        # Add the callback to the list
        super().append(callback)

    def remove(self, callback):
        """Unregister the given callback from the list.

        :param callback: The function to unregister from the list.

        .. code-block:: python

            def function(*args, **kwargs):
                # Code...

            <_HookBase object>.remove(function)
        """
        # Is the given callback a member of the object?
        if callback not in self:

            # Raise an exception
            raise ValueError(
                'Callback {0} is not registered to {1}.'.format(
                    callback, self.class_name))

        # Remove the callback from the list
        super().remove(callback)

    @property
    def class_name(self):
        """Return the name of the class."""
        return self.__class__.__name__


class _HookDecorator(AutoUnload):
    """Base decorator class used to add/remove hooks on load/unload."""

    def __init__(self, callback):
        """Store the given callback and register the hook."""
        self.callback = callback
        self._class_instance.append(self.callback)

    def __call__(self, *args, **kwargs):
        """Call the callback."""
        return self.callback(*args, **kwargs)

    def _unload_instance(self):
        """Unregister the hook."""
        self._class_instance.remove(self.callback)
