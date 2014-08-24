# ../events/manager.py

"""Provides event registration functionality."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
#   Events
from events.listener import _EventListener
#   Loggers
from loggers import _sp_logger


# =============================================================================
# >> FORWARD IMPORTS
# =============================================================================
# Source.Python Imports
#   Events
from _events import game_event_manager


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('game_event_manager',
           'event_registry',
           )


# =============================================================================
# >> GLOBAL VARIABLES
# =============================================================================
# Get the sp.events.manager logger
events_manager_logger = _sp_logger.events.manager


# =============================================================================
# >> CLASSES
# =============================================================================
class _EventRegistry(dict):

    """Stores Event names with all registered callbacks."""

    def __missing__(self, event):
        """Add an event to the dictionary and registers for it."""
        # Get an _EventListener instance
        listener = self[event] = _EventListener(event)

        # Add the listener to the game_event_manager
        game_event_manager.add_listener(listener.listener, event, True)

        # Return the instance
        return listener

    def register_for_event(self, event, callback):
        """Register the callback for the given event."""
        # Is the callback callable?
        if not callable(callback):

            # Raise an error
            raise ValueError('Callback "{0}" is not callable'.format(callback))

        # Add the callback to the event's registered callback list
        self[event].append(callback)

    def unregister_for_event(self, event, callback):
        """Unregister the callback for the given event."""
        # Is the event registered?
        if event not in self:

            # Raise an error
            raise ValueError('Event "{0}" is not registered'.format(event))

        # Remove the callback from the event's list
        self[event].remove(callback)

        # Are there any callbacks remaining for the event?
        if not self[event]:

            # Remove the listener from the game_event_manager
            game_event_manager.remove_listener(self[event].listener)

            # Remove the event from the dictionary
            del self[event]

# Get the _EventRegistry instance
event_registry = _EventRegistry()
