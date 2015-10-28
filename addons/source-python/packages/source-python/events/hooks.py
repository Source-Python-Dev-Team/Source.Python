# ../events/hooks.py

"""Provides event hooking functionality."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Python Imports
#   Enum
from enum import IntEnum

# Source.Python Imports
#   Core
from core import AutoUnload
#   Events
from events import GameEvent
from events.manager import game_event_manager
#   Hooks
from hooks.exceptions import except_hooks
#   KeyValues
from keyvalues import KeyValues
#   Memory
from memory import get_object_pointer
from memory import get_virtual_function
from memory import make_object
from memory.hooks import PreHook


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('EventAction',
           'PreEvent',
           'pre_event_manager',
           )


# =============================================================================
# >> CLASSES
# =============================================================================
class EventAction(IntEnum):
    """Enum class used to know what to do with a pre-hooked event."""

    CONTINUE = 0
    STOP_BROADCAST = 1
    BLOCK = 2


class PreEvent(AutoUnload):
    """Pre-Event decorator class."""

    def __init__(self, *event_names):
        """Store the event names."""
        self._event_names = event_names
        self._callback = None

    def __call__(self, callback):
        """Store the callback and register the pre-events."""
        # Store the callback
        self._callback = callback

        # Loop through all pre-event names
        for event_name in self._event_names:

            # Register the pre-event
            pre_event_manager.register_for_event(event_name, self._callback)

        # Return the instance so that it unloads properly
        return self

    def _unload_instance(self):
        """Unregister the pre-events."""
        # Loop through all pre-event names
        for event_name in self._event_names:

            # Unregister the pre-event
            pre_event_manager.unregister_for_event(event_name, self._callback)


class _PreEventDictionary(dict):
    """Dictionary class used to store pre-events with their callbacks."""

    def __missing__(self, event_name):
        """Add the event to the dictionary and return its instance."""
        # Add the event to the dictionary as a new list
        value = self[event_name] = _PreEventList(event_name)

        # Return the instance
        return value

    def register_for_event(self, event_name, callback):
        """Register the callback for the given event."""
        # Is the callback callable?
        if not callable(callback):

            # Raise an error
            raise TypeError(
                "'" + type(callback).__name__ + "' object is not callable.")

        # Add the callback to the pre-event's registered callback list
        self[event_name].append(callback)

    def unregister_for_event(self, event_name, callback):
        """Unregister the callback for the given event."""
        # Is the event registered?
        if event_name not in self:

            # Raise an error
            raise ValueError(
                'Pre-Event "{0}" is not registered'.format(event_name))

        # Remove the callback from the pre-event's list
        self[event_name].remove(callback)

        # Are there any callbacks remaining for the pre-event?
        if not self[event_name]:

            # Remove the pre-event from the dictionary
            del self[event_name]

# Get the _PreEventDictionary instance
pre_event_manager = _PreEventDictionary()


class _PreEventList(list):
    """List class used to store callbacks for a given event."""

    def __init__(self, event_name):
        """Initialize the list and store the event name."""
        # Initialize the list
        super().__init__()

        # Store the event name
        self.event_name = event_name

    def append(self, callback):
        """Add the callback to the list."""
        # Is the callback already in the list?
        if callback in self:

            # Raise an error
            raise ValueError(
                'Pre-Event callback "{0}" is already registered '
                'for event "{1}"'.format(callback, self.event_name))

        # Add the callback to the list
        super().append(callback)

    def remove(self, callback):
        """Remove the callback from the list."""
        # Is the callback in the list?
        if callback not in self:

            # If not, raise an error
            raise ValueError(
                'Pre-Event callback "{0}" is not registered for '
                'event "{1}"'.format(callback, self.event_name))

        # Remove the callback from the list
        super().remove(callback)


# =============================================================================
# >> PRE-HOOK FUNCTIONS
# =============================================================================
@PreHook(get_virtual_function(game_event_manager, 'FireEvent'))
def _pre_game_event(args):
    """Call pre-event functions if the event is registered."""
    # Get the GameEvent object
    game_event = make_object(GameEvent, args[1])

    # Get the name of the event
    event_name = game_event.get_name()

    # If the current event is not in the dictionary, return
    if event_name not in pre_event_manager:
        return

    # Create a variable to know what to do after all pre-events are called
    event_action = EventAction.CONTINUE

    # Convert the GameEvent object into a dictionary
    # TODO: use data or some other means to get the offset
    kwargs = make_object(
        KeyValues, get_object_pointer(game_event).get_pointer(8)).as_dict()
    kwargs['event_name'] = event_name

    # Loop through all callbacks in the pre-event's list
    for callback in pre_event_manager[event_name]:

        # Use try/except in case an error occurs during in the callback
        try:

            # Call the callback and get its return value
            current_action = callback(**kwargs)

            # Is the return value invalid?
            if (current_action is not None and
                    not isinstance(current_action, EventAction)):

                # Raise an error to exit the try
                raise ValueError(
                    'Invalid return value for pre-event "{0}".'.format(
                        current_action))

        # Was an error encountered?
        except:

            # Print the exception to the console
            except_hooks.print_exception()

        # Was no error encountered?
        else:

            # Does the current action have a higher priority?
            if current_action is not None and current_action > event_action:

                # Change the event action
                event_action = current_action

    # Does the return value want to set the dontbroadcast value?
    if event_action is EventAction.STOP_BROADCAST:

        # Set the dontbroadcast value
        args[2] = True

    # Does the return value want to block the event?
    elif event_action is EventAction.BLOCK:

        # Free the event
        game_event_manager.free_event(game_event)

        # Block the event
        return False
