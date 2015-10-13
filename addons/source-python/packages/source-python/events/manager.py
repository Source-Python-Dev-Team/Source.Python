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
__all__ = ('event_manager',
           'game_event_manager',
           )


# =============================================================================
# >> GLOBAL VARIABLES
# =============================================================================
# Get the sp.events.manager logger
events_manager_logger = _sp_logger.events.manager


# =============================================================================
# >> CLASSES
# =============================================================================
class _EventManager(dict):
    """Stores Event names with all registered callbacks."""

    def __missing__(self, event_name):
        """Add an event to the dictionary and registers for it."""
        # Get an _EventListener instance
        listener = self[event_name] = _EventListener(event_name)

        # Add the listener to the game_event_manager
        game_event_manager.add_listener(listener.listener, event_name, True)

        # Return the instance
        return listener

    def register_for_event(self, event_name, callback):
        """Register the callback for the given event."""
        # Is the callback callable?
        if not callable(callback):

            # Raise an error
            raise TypeError(
                "'" + type(callback).__name__ + "' object is not callable.")

        # Add the callback to the event's registered callback list
        self[event_name].append(callback)

    def unregister_for_event(self, event_name, callback):
        """Unregister the callback for the given event."""
        # Is the event registered?
        if event_name not in self:

            # Raise an error
            raise ValueError(
                'Event "{0}" is not registered'.format(event_name))

        # Remove the callback from the event's list
        self[event_name].remove(callback)

        # Are there any callbacks remaining for the event?
        if not self[event_name]:

            # Remove the listener from the game_event_manager
            game_event_manager.remove_listener(self[event_name].listener)

            # Remove the event from the dictionary
            del self[event_name]

# Get the _EventManager instance
event_manager = _EventManager()
