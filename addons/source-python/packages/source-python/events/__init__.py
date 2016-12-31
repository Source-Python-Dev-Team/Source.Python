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
from _events import EventVarType
from _events import GameEvent
from _events import GameEventDescriptor


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('Event',
           'EventVarType',
           'GameEvent',
           'GameEventDescriptor',
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

    def __init__(self, *event_names):
        """Store the event names."""
        self._event_names = ()
        self.callback = None

        # Validate event names
        if not event_names:
            raise ValueError('At least one event name is required.')

        for event_name in event_names:
            if not isinstance(event_name, str):
                raise ValueError('Event name must be a string.')

        self._event_names = event_names

    def __call__(self, callback):
        """Store the callback and register the events."""
        # Store the callback
        self.callback = callback

        # Loop through all event names
        for event_name in self._event_names:

            # Register the event
            event_manager.register_for_event(event_name, self.callback)

        # Return the callback
        return self.callback

    def _unload_instance(self):
        """Unregister the events."""
        # Was no callback registered?
        if self.callback is None:
            return

        # Loop through all event names
        for event_name in self._event_names:

            # Unregister the event
            event_manager.unregister_for_event(event_name, self.callback)
