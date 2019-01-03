Getting started
===============

If you would like to start developing server plugins with Source.Python this is
the right place for you!

In order to use Source.Python you first need to install it on your game server. To
do so, please follow the instructions described :doc:`here <../general/installation>`.

As soon as you have successfully installed Source.Python you can start writing
your first plugin.


Writing your first plugin
-------------------------

The first plugin will be a very simple one. But before writing any code, you
have to create the plugin files.

To do so, please create a directory in Source.Python's plugin directory
(``../addons/source-python/plugins``). All plugins will be located in this
directory and must have their own sub-directory. Give the new created
directory an abitrary name (e.g. test1). Now, you need to create the actual
plugin file. It must be named like its directory. So, if you have created a
``test1`` directory, you have to create a ``test1.py`` in that directory.

The first plugin should simply print a message to the server console, when the
plugin has been loaded and another message when the plugin has been unloaded.
You can easily do that by adding a ``load`` and an ``unload`` function to your
plugin file. These two functions will be called by Source.Python when the
plugin is loaded or unloaded.

To print a message to the console you can use Python's
`print <https://docs.python.org/3.4/library/functions.html#print>`_ function.

.. note:: This only prints a message to the server console. It will not appear in any
    log files.

Your plugin should now look like this.

.. code-block:: python

    def load():
        print('Plugin has been loaded successfully!')

    def unload():
        print('Plugin has been unloaded successfully!')


To load your plugin enter ``sp plugin load test1`` in your server console. To
unload or reload it, you can use ``sp plugin reload test1`` or
``sp plugin unload test1``.
Source.Python plugins are not getting loaded automatically. Thus, you need to
add the statement ``sp plugin load test1`` to your ``autoexec.cfg`` if you wish
this behaviour.


Modifying your first plugin
---------------------------

Just sending a message to the server console is not very exciting. Moreover,
players on your server don't even noticed that. Therefore, this section will
show you how to send the message to the chat of all players.

To send a message to a player you can make use of the :mod:`messages` module.
It provides various classes to send different kinds of messages. For example
you can send messages to the player's chat or directly in the center of the
player's screen. In this example we will use :class:`messages.SayText2`,
because we want to print the message in the chat of each player. The basic
procedure to send a message is very simple.

1. Create an object of the desired message class.
2. Send the message by using its :meth:`~.messages.base.UserMessageCreator.send` method.

Your plugin should now look like this.

.. code-block:: python

    from messages import SayText2

    def load():
        SayText2('Plugin has been loaded successfully!').send()

    def unload():
        SayText2('Plugin has been unloaded successfully!').send()


Using events in your first plugin
---------------------------------

Admittedly, this plugin is still very boring, but that will change immediately
when you listen to events. Before continuing please read the
`event introduction <events.html#introduction>`__. It will give you a short
overview of what events are and how to listen to them.

We will now listen to the ``player_spawn`` event to give every player a little
extra HP when they spawn. To modify the player's health you need an object of
the :class:`players.entity.Player` class. Its constructor only requires
a player index. Since the ``player_spawn`` event provides the user ID of the
player that spawned, you can use :func:`players.helpers.index_from_userid` to
convert the user ID into a player index.

Your plugin should now look like this.

.. code-block:: python

    from events import Event
    from players.entity import Player
    from players.helpers import index_from_userid
    from messages import SayText2

    # Extra amount of health every player should get on spawn
    EXTRA_HP = 25

    def load():
        SayText2('Plugin has been loaded successfully!').send()

    def unload():
        SayText2('Plugin has been unloaded successfully!').send()

    @Event('player_spawn')
    def on_player_spawn(game_event):
        # Get the user ID of the spawned player
        userid = game_event['userid']

        # Convert the user ID into a player index
        index = index_from_userid(userid)

        # Create a Player object...
        player = Player(index)

        # ... to add some extra HP
        player.health += EXTRA_HP


Alternatively, you can use the classmethod
:meth:`players.entity.Player.from_userid`. It's a wrapper around
:func:`players.helpers.index_from_userid` and will shorten your code in
events.

.. code-block:: python

    from events import Event
    from players.entity import Player
    from messages import SayText2

    # Extra amount of health every player should get on spawn
    EXTRA_HP = 25

    def load():
        SayText2('Plugin has been loaded successfully!').send()

    def unload():
        SayText2('Plugin has been unloaded successfully!').send()

    @Event('player_spawn')
    def on_player_spawn(game_event):
        # Create a Player object from the user ID...
        player = Player.from_userid(game_event['userid'])

        # ... and add some extra HP
        player.health += EXTRA_HP


What's next?
------------

You should definitely take a look at the :doc:`module tutorials section <module_tutorials>`.
It contains detailed tutorials about some Source.Python modules/packages.

Moreover, you should take a look at the :ref:`modindex`. It's a list of all
Source.Python modules/packages and contains the API documentation.
