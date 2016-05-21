auth
====

This page contains tutorials about the :mod:`auth` package.


Naming convention on permissions
--------------------------------

Permissions should always start with the full name or short name of the plugin
that defines the permission. This avoids name clashes with other plugins. E.g.
if your plugin is called ``admin`` and you want to define the two permissions
``kick`` and ``ban``, the names of the permissions should be ``admin.kick``
and ``admin.ban``. This also allows server owners to grant people all
permissions for your plugin by simply using ``admin.*``.


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
