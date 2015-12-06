# ../commands/command.py

"""Contains a base decorator class for registering commands."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
#   Core
from core import AutoUnload


# =============================================================================
# >> CLASSES
# =============================================================================
class _BaseCommand(AutoUnload):
    """Base decorator class used to register commands."""

    def __init__(self, names, *args, **kwargs):
        """Store the base values for the decorator."""
        self.names = names
        self.args = args
        self.kwargs = kwargs
        self.callback = None

    def __call__(self, callback):
        """Register the commands to the given callback."""
        # Store the callback
        self.callback = callback

        # Register the commands
        self._manager_class.register_commands(
            self.names, self.callback, *self.args, **self.kwargs)

        # Return the original callback
        return self.callback

    def _unload_instance(self):
        """Unregister the commands."""
        # Unregister the commands
        self._manager_class.unregister_commands(self.names, self.callback)
