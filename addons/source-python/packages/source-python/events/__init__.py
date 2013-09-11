# ../events/__init__.py

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
from core import AutoUnload
from loggers import _SPLogger
from public import public
#   Events
from events.manager import EventRegistry


# =============================================================================
# >> GLOBAL VARIABLES
# =============================================================================
# Get the sp.events logger
EventsLogger = _SPLogger.events


# =============================================================================
# >> CLASSES
# =============================================================================
@public
class Event(AutoUnload):
    '''Event decorator class'''

    def __init__(self, callback):
        '''Store the callback and register the event'''

        # Is the callback callable?
        if not callable(callback):

            # Raise an error
            raise TypeError(
                "'" + type(callback).__name__ + "' object is not callable")

        # Store the callback
        self.callback = callback

        # Register the event
        EventRegistry.register_for_event(self.callback.__name__, self.callback)

    def __call__(self, GameEvent):
        '''Calls the Event callback with the GameEvent instance'''
        return self.callback(GameEvent)

    def _unload_instance(self):
        '''Unregisters the event'''
        EventRegistry.unregister_for_event(
            self.callback.__name__, self.callback)
