# ../events/__init__.py

"""Provides event based functionality."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
#   Core
from core import AutoUnload
#   Events
from events.manager import event_manager
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
        # Store the callback
        self.callback = callback

        # Register the event
        event_manager.register_for_event(
            self.callback.__name__, self.callback)

    def _unload_instance(self):
        """Unregister the event."""
        event_manager.unregister_for_event(
            self.callback.__name__, self.callback)
