# ../events/__init__.py

"""Provides event based functionality."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
#   Core
from core import AutoUnload
#   Events
from events.manager import event_registry
#   Loggers
from loggers import _sp_logger


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
events_logger = _sp_logger.events


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
        event_registry.register_for_event(
            self.callback.__name__, self.callback)

    def _unload_instance(self):
        """Unregister the event."""
        event_registry.unregister_for_event(
            self.callback.__name__, self.callback)
