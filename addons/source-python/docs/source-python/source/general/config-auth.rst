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
Both players and groups can inherit permissions from groups, allowing a
flexible and dynamic permission hierarchy.

This system allows permissions to be added to all children entities easily
when a new plugin is added.

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
        "[U:1:6456723]": {
            "permissions": [
                "admin.kick",
                "admin.ban"
            ]
        },
        "STEAM_0:323145": {
            "permissions": [
                "admin.burn",
                "fun.rtd"
            ]
        },
        "78944003194": {
            "parents": [
                "admin"
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
        "admin": {
            "permissions": [
                "admin.*"
            ]
        }
    }

The above example creates a new group called ``admin`` which is able
to execute every permission defined by the ``admin`` plugin. Every player
or parent that inherits from this parent is able to execute ``admin.kick``
and ``admin.ban``. In case the plugin author of ``admin`` adds in another
permission (e.g. ``admin.burn``) all players and parents inheriting from
``admin`` will automatically have the permission to execute
``admin.burn``, because the asterisk symbol (*) matches all subnodes.

The third file is a simple text file that grants all players that have been
added to this file the permission to execute everything and all.

Example content for ``simple.txt``:

.. code-block::

    [U:1:6456723]
    STEAM_0:323145
    78944003194

The equivalent for this configuration by using ``players.json`` would look
like this:

.. code-block:: javascript

    {
        "[U:1:6456723]": {
            "permissions": [
                "*"
            ]
        },
        "STEAM_0:323145": {
            "permissions": [
                "*"
            ]
        },
        "78944003194": {
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
        "[U:1:6456723]": {
            "parents": [
                "super_admin"
            ]
        },
        "STEAM_0:323145": {
            "parents": [
                "super_admin"
            ]
        },
        "78944003194": {
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
* You want to use `SP-Webmin <http://github.com/necavi/SP-Webmin>`_ for advanced multi-server management.


.. todo::

    Which databases are supported?
    How to use one database for multiple servers? What are the requirements?
    How to use one database and have global and server specific permissions?

.. todo::

    Update console command documentation and mention it in this file.