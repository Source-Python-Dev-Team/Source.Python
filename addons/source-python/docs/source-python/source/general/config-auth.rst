Configuring authorization
=========================


Introduction
------------

Source.Python provides an authorization package that can be used by plugins to
check whether a player is granted a specific permission. There are two object
types that can have permissions:

* Players
* Parents

Parents can be used to create a set of permissions (like groups or roles).
Then you can add parents to players, so they inherit the permissions of the
parents. You can also add parents to other parents! This allows a flexible and
clear permission hierachy.

Before granting permissions and creating parents, you need to choose an
authorization backend.


Backends
--------

The backend defines how permissions are stored. Currently, you can choose
between two built-in backends:

* Flatfile
* SQL

To define which backend should be used, please open ``core_settings.ini`` and
specify the backend you want to use in the ``AUTH_SETTINGS`` section.


Flatfile
^^^^^^^^

The flatfile backend is a very easy backend and is a good choice in the
following situations:

* You only run a single server
* You run multiple servers, but don't want cross-server permissions
* You just want to quickly configure authorization

The backend creates three files to store all the authorization related data:

1. ``players.json``
2. ``parents.json``
3. ``simple.txt``

.. note::
    By default these files are created in ``../cfg/source-python/auth/``, but
    you can easily configure other locations in the ``AUTH_SETTINGS`` section
    in ``core_settings.ini``. This makes sense if you run multiple srcds
    instances on the same server and want to share permissions across the
    servers.

The first file is used to grant players permissions and add parents to
players. All data is stored in the JSON format.

Example content for ``players.json``:

.. code-block:: javascript

    {
        "STEAMID1": {
            "permissions": [
                "plugin1.do1",
                "plugin1.do2"
            ]
        },
        "STEAMID2": {
            "permissions": [
                "plugin1.do1",
                "plugin2.do1"
            ]
        },
        "STEAMID3": {
            "parents": [
                "plugin1_admin"
            ]
        }
    }

In the example above ``STEAMID1``, ``STEAMID2`` and ``STEAMID3`` would be
replaced with real SteamIDs, obviously. The SteamID format can be either
SteamID2 (STEAM_Y:X:Z), SteamID3 ([U:X]) or SteamID64 (a long number).

The second file is used to grant parents permissions and to add parents to
other parents. The format is pretty much the same like the format in
``players.json``. The only difference is that you don't use SteamIDs, but
names for the parents.

Example content for ``parents.json``:

.. code-block:: javascript

    {
        "plugin1_admin": {
            "permissions": [
                "plugin1.*"
            ]
        }
    }

The above example creates a new group called ``plugin1_admin`` which is able
to execute every permission defined by ``plugin1``. Every player or parent
that inherits from this parent is able to execute ``plugin1.do1`` and
``plugin1.do2``. In case the plugin author of ``plugin1`` adds in another
permission (e.g. ``plugin1.do3``) all players and parents inheriting from
``plugin1_admin`` will automatically have the permission to execute
``plugin1.do3``, because the asterisk matches everything.

The third file is a simple text file that grants all players that have been
added to this file the permission to execute everything and all.

Example content for ``simple.txt``:

.. code-block:: none

    STEAMID1
    STEAMID2
    STEAMID3

The equivalent for this configuration by using ``players.json`` would look
like this:

.. code-block:: javascript

    {
        "STEAMID1": {
            "permissions": [
                "*"
            ]
        },
        "STEAMID2": {
            "permissions": [
                "*"
            ]
        },
        "STEAMID3": {
            "permissions": [
                "*"
            ]
        }
    }

Another possibility would be to create a super admin parent and add the
parents to all SteamIDs.

Example content for ``players.json``:

.. code-block:: javascript

    {
        "STEAMID1": {
            "parents": [
                "super_admin"
            ]
        },
        "STEAMID2": {
            "parents": [
                "super_admin"
            ]
        },
        "STEAMID3": {
            "parents": [
                "super_admin"
            ]
        }
    }

Example content for ``parents.json``:

.. code-block:: javascript

    {
        "super_admin": {
            "permissions": [
                "*"
            ]
        }
    }


SQL
^^^

The SQL backend is a more advanced backend and is a good choice in the
following situations:

* You run multiple server and want to share the permissions across all servers.
* You want to use SP-Webmin


.. todo::

    Which databases are supported?
    How to use one database for multiple servers? What are the requirements?
    How to use one database and have global and server specific permissions?

.. todo::

    Update console command documentation and mention it in this file.