# ../tick/__init__.py

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
from listener_c import get_tick_listener_manager
from core import AutoUnload
from loggers import _SPLogger
from public import public


# =============================================================================
# >> GLOBAL VARIABLES
# =============================================================================
# Get the CTickListenerManager instance
TickListenerManager = get_tick_listener_manager()

# Get the sp.tick logger
TickLogger = _SPLogger.tick


# =============================================================================
# >> CLASSES
# =============================================================================
@public
class Tick(AutoUnload):
    '''Decorator class used to register/unregister a tick listener'''

    def __init__(self, callback):
        '''Store the callback and register the tick listener'''

        # Log the Tick.__init__ message
        TickLogger.log_info('Tick.__init__ <{0}>'.format(callback))

        # Is the callback callable?
        if not callable(callback):

            # Raise an error
            raise TypeError(
                "'" + type(callback).__name__ + "' object is not callable")

        # Log the registering message
        TickLogger.log_info('Tick.__init__ - Registering')

        # Store the callback
        self.callback = callback

        # Register the tick listener
        TickListenerManager.register_listener(self.callback)

    def __call__(self):
        '''Calls the tick listener'''

        # Call the listener
        return self.callback()

    def _unload_instance(self):
        '''Unregisters the tick listener'''

        # Log the unregistering
        TickLogger.log_info(
            'Tick._unload_instance - Unregistering '
            '<{0}>'.format(self.callback))

        # Unregister the tick listener
        TickListenerManager.unregister_listener(self.callback)
