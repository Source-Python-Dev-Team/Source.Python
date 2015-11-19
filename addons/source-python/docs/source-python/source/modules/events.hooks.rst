events.hooks module
====================

.. automodule:: events.hooks
    :members:
    :undoc-members:
    :show-inheritance:
    :exclude-members: EventAction, PreEvent, pre_event_manager

.. class:: EventAction

    Bases: :class:`enum.IntEnum`

    Enum class used to know what to do with a pre-hooked event.

    :attr: CONTINUE: Allow the event to be fired on the
        server and transmitted to clients.

    :attr: STOP_BROADCAST: Allow the event to be fired on the server
        but not to be transmitted to clients.

    :attr: BLOCK: Stop the event from being fired on the server.

.. decorator:: PreEvent(*event_names)

    Bases: :class:`core.AutoUnload`

    Fired when any event in ``event_names`` is about to be fired on the server.

    :param str event_names: An event name or any number of
        event names to register to the decorated function.

    The decorated function is passed the following parameters when
    an event in ``event_names`` is going to be fired on the server:

    :param GameEvent game_event: The fired event object.

    :rtype: EventAction

    Examples:

    .. code-block:: python

        from events import PreEvent

        @PreEvent('player_death')
        def pre_player_died(game_event):
            # Code...

    .. code-block:: python

        @PreEvent('round_start', 'round_freeze_end')
        def some_pre_function(game_event):
            # Code...

            # Stop the event from being fired at all on the server
            return EventAction.BLOCK

    .. code-block:: python

        @PreEvent('player_team')
        def pre_player_team(game_event):
            # Code...

            # Stop the event from being transmitted to clients
            return EventAction.STOP_BROADCAST

    .. seealso:: :doc:`../events` for a list of supported events per game.

.. data:: pre_event_manager

    The singleton object of the :class:`_PreEventManager` class.
