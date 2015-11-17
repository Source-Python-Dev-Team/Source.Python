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
    """Event decorator class.

    .. py:decorator:: Event(*event_names)

            Fired when any event in *event_names* is fired on the server

            Passed arguments: :class:`GameEvent`

    .. code-block:: python

        from events import Event


        @Event('player_death')
        def player_died(game_event):
            ...


        @Event('round_start', 'round_freeze_end')
        def some_function(game_event):
            ...

    .. seealso:: :doc:`../events` for a list of supported events per game.

    .. seealso:: :class:`GameEvent` for
        game_event argument functionality.
    """

    def __init__(self, *event_names):
        """Store the event names."""
        # Validate event names
        if not event_names:
            raise ValueError('At least one event name is required.')

        for event_name in event_names:
            if not isinstance(event_name, str):
                raise ValueError('Event name must be a string.')

        self._event_names = event_names
        self._callback = None

    def __call__(self, callback):
        """Store the callback and register the events."""
        # Store the callback
        self._callback = callback

        # Loop through all event names
        for event_name in self._event_names:

            # Register the event
            event_manager.register_for_event(event_name, self._callback)

        # Return the instance so that it unloads properly
        return self

    def _unload_instance(self):
        """Unregister the events."""
        # Loop through all event names
        for event_name in self._event_names:

            # Unregister the event
            event_manager.unregister_for_event(event_name, self._callback)
