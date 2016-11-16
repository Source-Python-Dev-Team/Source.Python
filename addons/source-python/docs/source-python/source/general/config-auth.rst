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
Both, players and parents, can inherit permissions from parents, allowing a
flexible and dynamic permission hierarchy.

Before granting permissions and creating parents, you need to choose an
authorization backend.


Backends
--------

The backend defines how permissions are stored. Currently, you can choose
between two built-in backends:

* Flatfile
* SQL

To define which backend should be used, please open ``core_settings.ini`` and
specify the backend you want to use in the ``AUTH_SETTINGS`` section. Backend
specific settings are provided in sub-sections within the ``BACKENDS`` section.

Example/default settings:

.. code-block:: ini

    [AUTH_SETTINGS]
    backend = flatfile
    server_id = -1

    [[BACKENDS]]

    [[[flatfile]]]
    player_config_path = ../cfg/source-python/auth/players.json
    simple_config_path = ../cfg/source-python/auth/simple.txt
    parent_config_path = ../cfg/source-python/auth/parents.json

    [[[sql]]]
    uri = sqlite:///../addons/source-python/data/source-python/permissions.db


Flatfile
^^^^^^^^

The flatfile backend is a very easy and simple backend. It's the
pre-configured/default backend in Source.Python and a good choice in the
following situations:

* You only run a single server
* You run multiple servers, but don't want cross-server permissions (each server has its own permissions)
* You just want to quickly configure authorization

The backend creates three files to store all the authorization related data:

1. ``players.json``
2. ``parents.json``
3. ``simple.txt``

.. note::

    By default these files are created in ``../cfg/source-python/auth/``, but
    you can easily configure other locations in the ``AUTH_SETTINGS`` section
    in ``core_settings.ini``.

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
                "fun.rtd"
            ]
        },
        "78944003194": {
            "parents": [
                "administrator"
            ]
        }
    }

.. note::

    The SteamID format can be either SteamID2 (STEAM_Y:X:Z), SteamID3 ([U:X])
    or SteamID64 (a long number).

The second file is used to grant parents permissions and to add parents to
other parents. The format is pretty much the same like the format in
``players.json``. The only difference is that you don't use SteamIDs, but
names for the parents.

Example content for ``parents.json``:

.. code-block:: javascript

    {
        "administrator": {
            "permissions": [
                "admin.*"
            ]
        }
    }

The above example creates a new group called ``administrator`` which is able
to execute every permission defined by the ``admin`` plugin. Every player
or parent that inherits from this parent is able to execute ``admin.kick``
and ``admin.ban``. In case the plugin author of ``admin`` adds in another
permission (e.g. ``admin.burn``) all players and parents inheriting from
``administrator`` will automatically have the permission to execute
``admin.burn``, because the asterisk symbol (*) matches all subnodes.

The third file is a simple text file that grants all players that have been
added to this file the permission to execute everything and all.

Example content for ``simple.txt``:

.. code-block:: none

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

Currently, the auth API officially only supports SQLite and MySQL, but as it is
implemented using SQLAlchemy it should work on other database engines such as
Postgre.


SQLite configuration
""""""""""""""""""""

Abstract example:

.. code-block:: ini

    [[[sql]]]
    uri = sqlite://<path to database file>

Concrete example:

.. code-block:: ini

    [[[sql]]]
    uri = sqlite://<addon path>\source-python\data\source-python\permissions.db

.. note::

    While multiple servers can use the same SQLite database it is not recommended.


MySQL configuration
"""""""""""""""""""

Abstract example:

.. code-block:: ini

    [[[sql]]]
    uri = mysql+pymysql://<username>:<password>@<host>/<database>``

Concrete example:

.. code-block:: ini

    [[[sql]]]
    uri = mysql+pymysql://user:1234@127.0.0.1/permissions

Any number of servers can be pointed to the same database.


Adding, modifying and deleting permissions and parents
------------------------------------------------------

You can always add, modify and delete permissions and parents by accessing the
JSON files or SQL database directly. However, Source.Python also provides
server commands to do these tasks. :doc:`You might want to try them. <sp-commands>`


Assigning permissions to guests
-------------------------------

There is a special parent called ``guest`` which can be used to assign
permissions to anonymous players. Every player on the server is a member of
that group.
