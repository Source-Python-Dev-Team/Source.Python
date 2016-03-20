events package
===============

Submodules
----------

.. toctree::
   :titlesonly:

   events.custom
   events.hooks
   events.listener
   events.manager
   events.resource
   events.variable

Module contents
---------------

.. automodule:: events
    :members:
    :undoc-members:
    :show-inheritance:
    :special-members:
    :exclude-members: Event

.. decorator:: Event(*event_names)

    Bases: :class:`core.AutoUnload`

    Fired when any event in ``event_names`` is fired on the server.

    :param str event_names: An event name or any number of
        event names to register to the decorated function.

    The decorated function is passed the following parameters when
    an event in ``event_names`` is fired on the server:

    :param GameEvent game_event: The fired event.

    Examples:

    .. code-block:: python

        from events import Event

        @Event('player_death')
        def player_died(game_event):
            # Code...

    .. code-block:: python

        @Event('round_start', 'round_freeze_end')
        def some_function(game_event):
            # Code...

    .. seealso:: :doc:`../events` for a list of supported events per game.
