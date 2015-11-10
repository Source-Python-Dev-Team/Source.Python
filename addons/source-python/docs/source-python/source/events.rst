Events
======


Introduction
------------

Events are probably one of the most important tools when you want to write a
plugin for a game. The game fires them in specific situations (e.g. when a
player was hurt, died or the round started) and Source.Python is able to
listen to them. An event might provide various event variables, which contain
more information about the event. E.g. the event "player_hurt" always provides
the user ID of the attacker and the user ID of the victim, which can be used
to interact with these players. A basic example could look like this:

.. code-block:: python

    from events import Event

    @Event('player_hurt')
    def on_player_hurt(event):
        # Retrieve the user ID of the victim
        userid = event.get_int('userid')

        # Retrieve the user ID of the attacker
        attacker = event.get_int('attacker')

        print('user ID "{}" was hurt by attacker ID "{}"'.format(userid, attacker))

In order to listen to an event, you have to import the :class:`events.Event`
decorator. The decorator requires a name, which is the name of the event you
want to listen to. All event callbacks require exactly one argument, which is
a :class:`events.GameEvent` object. This object can be used to access the
specific event variables.

.. warning::

    The :class:`events.GameEvent` object doesn't know which event variables
    exist and cannot raise an error if you are trying to access an event
    variable that doesn't exist. In that case a default value will be returned.

.. warning::

    All games provide different events. Some events might exist on all games,
    but they can still provide different event variables.

    
Events
------
    
Below you can find a list of games and their supported events.

.. toctree::
   :glob:

   events/*
