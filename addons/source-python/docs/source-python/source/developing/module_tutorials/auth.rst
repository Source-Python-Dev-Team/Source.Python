auth
====

This page contains tutorials about the :mod:`auth` package.


Check if a player is granted a permission
-----------------------------------------

You can easily check if a player is granted a specific permission by accessing
the :attr:`players.entity.Player.permissions` attribute.

.. code-block:: python

    from filters.players import PlayerIter

    # Loop through all human players and test if the player is granted the
    # permission 'my_plugin.something'
    for player in PlayerIter('human'):
        print('"my_plugin.something" is granted to "{}"?: {}'.format(
            player.name, 'my_plugin.something' in player.permissions))


Granting a permission
---------------------

By using the :attr:`players.entity.Player.permissions` attribute you can also
easily grant a player a permission.

.. code-block:: python

    from filters.players import PlayerIter

    # Loop through all human players and grant the permission 'my_plugin.something'
    for player in PlayerIter('human'):
        player.permissions.add('my_plugin.something')


Removing a permission
---------------------

Removing a permission is as easy as adding one.

.. code-block:: python

    from filters.players import PlayerIter

    # Loop through all human players and remove the permission 'my_plugin.something'
    for player in PlayerIter('human'):
        player.permissions.remove('my_plugin.something')
