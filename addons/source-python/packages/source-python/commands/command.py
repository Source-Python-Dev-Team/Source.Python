# ../commands/command.py

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
#   Core
from core import AutoUnload


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ()


# =============================================================================
# >> CLASSES
# =============================================================================
class _BaseCommand(AutoUnload):
    '''Base decorator class used to register commands'''

    def __init__(self, names, *args, **kwargs):
        '''Stores the base values for the decorator'''

        # Store the names
        self.names = names

        # Store the arguments
        self.args = args

        # Store the keyword arguments
        self.kwargs = kwargs

    def __call__(self, callback):
        '''Registers the commands to the given callback'''

        # Store the callback
        self.callback = callback

        # Register the commands
        self._manager_class.register_commands(
            self.names, self.callback, *self.args, **self.kwargs)

        # Return the object
        return self

    def _unload_instance(self):
        '''Unregisters the commands'''
        self._manager_class.unregister_commands(self.names, self.callback)
