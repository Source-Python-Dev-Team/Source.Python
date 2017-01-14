# ../commands/filter.py

"""Contains a base decorator class for registering filters."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
#   Core
from core import AutoUnload


# =============================================================================
# >> CLASSES
# =============================================================================
class _BaseFilter(AutoUnload):
    """Class used to register a filter."""

    def __init__(self, callback):
        """Store the callback and registers the filter.
        
        :param callable callback:
            A callback that does filter logic.
        """
        # Is the callback callable?
        if not callable(callback):

            # Raise an error
            raise TypeError(
                "'" + type(callback).__name__ + "' object is not callable")

        # Store the callback
        self.callback = callback

        # Register the filter
        self._manager_class.register_filter(self.callback)

    def __call__(self, *args, **kwargs):
        """Call the callback."""
        return self.callback(*args, **kwargs)

    def _unload_instance(self):
        """Unregister the filter."""
        self._manager_class.unregister_filter(self.callback)
