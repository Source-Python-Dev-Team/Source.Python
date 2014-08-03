# ../events/__init__.py

"""Provides event based functionality."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
#   Core
from core import AutoUnload
#   Events
from events.manager import EventRegistry
#   Loggers
from loggers import _SPLogger


# =============================================================================
# >> FORWARD IMPORTS
# =============================================================================
# Source.Python Imports
#   Events
from _events import GameEvent


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('Event',
           'GameEvent',
           )


# =============================================================================
# >> GLOBAL VARIABLES
# =============================================================================
# Get the sp.events logger
EventsLogger = _SPLogger.events


# =============================================================================
# >> CLASSES
# =============================================================================
class Event(AutoUnload):

    """Event decorator class."""

    def __init__(self, callback):
        """Store the callback and register the event."""
        # Is the callback callable?
        if not callable(callback):

            # Raise an error
            raise TypeError(
                "'" + type(callback).__name__ + "' object is not callable")

        # Store the callback
        self.callback = callback

        # Register the event
        EventRegistry.register_for_event(self.callback.__name__, self.callback)

    def _unload_instance(self):
        """Unregister the event."""
        EventRegistry.unregister_for_event(
            self.callback.__name__, self.callback)
