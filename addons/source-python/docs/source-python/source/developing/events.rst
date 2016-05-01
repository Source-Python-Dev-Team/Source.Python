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
    def on_player_hurt(game_event):
        # Retrieve the user ID of the victim
        userid = game_event['userid']

        # Retrieve the user ID of the attacker
        attacker = game_event['attacker']

        print('user ID "{}" was hurt by attacker ID "{}"'.format(userid, attacker))

In order to listen to an event, you have to import the :class:`events.Event`
decorator. The decorator requires a name, which is the name of the event you
want to listen to. All event callbacks require exactly one argument, which is
a :class:`events.GameEvent` object. This object can be used to access the
specific event variables.

.. note::

    :meth:`events.GameEvent.__getitem__` will raise a :class:`KeyError` if you
    are trying to access an event variable that doesn't exist.
    
.. note::

    :meth:`events.GameEvent.__getitem__` will return the value in the type
    that has been defined in the event resource files.

.. warning::

    All games provide different events. Some events might exist on all games,
    but they can still provide different event variables.
    
    
Printing event variables
------------------------

Sometimes you might want to print all event variables and their values.
Fortunately, there is a :attr:`events.GameEvent.variables` attribute, which
contains all event variables. You can use this little snippet to display them.

.. code-block:: python

    from events import Event
    from pprint import pprint

    @Event('player_hurt')
    def on_player_hurt(game_event):
        pprint(game_event.variables.as_dict())

    
Available events
----------------
    
Below you can find a list of games and their supported events.

.. toctree::
   :titlesonly:
   :glob:

   events/*
