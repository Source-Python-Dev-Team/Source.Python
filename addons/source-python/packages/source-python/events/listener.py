# ../events/listener.py

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
#   Hooks
from hooks.exceptions import ExceptHooks
#   Loggers
from loggers import _SPLogger


# =============================================================================
# >> FORWARD IMPORTS
# =============================================================================
# Source.Python Imports
#   Events
from _events import GameEventListener


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
# Set all to an empty list
__all__ = [
    'GameEventListener',
]


# =============================================================================
# >> GLOBAL VARIABLES
# =============================================================================
# Get the sp.events.listener logger
EventsListenerLogger = _SPLogger.events.listener


# =============================================================================
# >> CLASSES
# =============================================================================
class _EventListener(list):
    '''Stores callbacks for the given event'''

    def __init__(self, event):
        '''Called when an instance is created'''

        # Store the event name
        self.event = event

        # Store the listener instance
        self.listener = GameEventListener()

        # Set the listener's fire_game_event method to the instance's method
        self.listener.fire_game_event = self.fire_game_event

    def append(self, callback):
        '''Adds the callback to the list'''

        # Is the callback already in the list?
        if callback in self:

            # Raise an error
            raise ValueError(
                'Event callback "{0}" is already registered '
                'for event "{1}"'.format(callback, self.event))

        # Add the callback to the list
        super(_EventListener, self).append(callback)

    def remove(self, callback):
        '''Removes the callback from the event's list'''

        # Is the callback in the list?
        if not callback in self:

            # Raise an error
            raise ValueError(
                'Event callback "{0}" is not registered for '
                'the event "{1}"'.format(callback, self.event))

        # Remove the callback from the list
        super(_EventListener, self).remove(callback)

    def fire_game_event(self, game_event):
        '''Loops through all callbacks for an event and calls them'''

        # Loop through each callback in the event's list
        for callback in self:

            # Try to call the callback
            try:

                # Call the callback
                callback(game_event)

            # Was an error encountered?
            except:

                # Print the exception to the console
                ExceptHooks.print_exception()
