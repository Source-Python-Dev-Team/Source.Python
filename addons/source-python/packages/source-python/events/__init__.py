# ../events/__init__.py

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
from core import AutoUnload
from loggers import _SPLogger
#   Events
from events.manager import EventRegistry


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
# Add all the global variables to __all__
__all__ = [
    'Event',
]


# =============================================================================
# >> GLOBAL VARIABLES
# =============================================================================
# Get the sp.events logger
EventsLogger = _SPLogger.events


# =============================================================================
# >> CLASSES
# =============================================================================
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

        # Set the instance module so that the event
        # can be unregistered properly on unload
        self.__module__ = self.callback.__module__

        # Register the event
        EventRegistry.register_for_event(self.callback.__name__, self.callback)

    def _unload_instance(self):
        '''Unregisters the event'''
        EventRegistry.unregister_for_event(
            self.callback.__name__, self.callback)
