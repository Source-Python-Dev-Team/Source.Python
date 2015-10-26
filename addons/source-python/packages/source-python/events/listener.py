# ../events/listener.py

"""Contains event listening functionality."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
#   Hooks
from hooks.exceptions import except_hooks
#   Loggers
from loggers import _sp_logger


# =============================================================================
# >> FORWARD IMPORTS
# =============================================================================
# Source.Python Imports
#   Events
from _events import GameEventListener


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('GameEventListener',
           )


# =============================================================================
# >> GLOBAL VARIABLES
# =============================================================================
# Get the sp.events.listener logger
events_listener_logger = _sp_logger.events.listener


# =============================================================================
# >> CLASSES
# =============================================================================
class _EventListener(list):
    """Stores callbacks for the given event."""

    def __init__(self, event_name):
        """Called when an instance is created."""
        # Initialize the list
        super().__init__()

        # Store the event name
        self.event_name = event_name

        # Store the listener instance
        self.listener = GameEventListener()

        # Set the listener's fire_game_event method to the instance's method
        self.listener.fire_game_event = self.fire_game_event

    def append(self, callback):
        """Add the callback to the list."""
        # Is the callback already in the list?
        if callback in self:

            # Raise an error
            raise ValueError(
                'Event callback "{0}" is already registered '
                'for event "{1}"'.format(callback, self.event_name))

        # Add the callback to the list
        super().append(callback)

    def remove(self, callback):
        """Remove the callback from the event's list."""
        # Is the callback in the list?
        if callback not in self:

            # Raise an error
            raise ValueError(
                'Event callback "{0}" is not registered for '
                'the event "{1}"'.format(callback, self.event_name))

        # Remove the callback from the list
        super().remove(callback)

    def fire_game_event(self, game_event):
        """Loop through all callbacks for an event and calls them."""
        # Loop through each callback in the event's list
        for callback in self:

            # Try to call the callback
            try:

                # Call the callback
                callback(game_event)

            # Was an error encountered?
            except:

                # Print the exception to the console
                except_hooks.print_exception()
