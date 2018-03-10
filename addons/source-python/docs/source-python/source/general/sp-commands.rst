Console commands
================

This page provides a list of all "sp" sub-commands.


auth
----

Provide authorization specific commands.

.. code-block:: none

    // Usage
    // sp auth <sub-command>


backend
^^^^^^^

Provide backend specific commands.

.. code-block:: none

    // Usage
    // sp auth backend <sub-command>


set
"""

Set the active backend.

.. code-block:: none

    // Usage
    // sp auth backend set <backend>

    // Tell Source.Python to use the SQL backend
    sp auth backend set sql


list
""""

List all available backends and mark the active backend.

.. code-block:: none

    // Usage
    // sp auth backend list


permission
^^^^^^^^^^

Provide permission specific commands.

.. code-block:: none

    // Usage
    // sp auth permission <sub-command>


parent
""""""

Provide commands to modify parent permissions.

.. code-block:: none

    // Usage
    // sp auth permission parent <sub-command>


add
~~~

Add a permission to a parent. If the given parent does not exist, it will be created.

.. code-block:: none

    // Usage
    // sp auth permission parent add <parent> <permission> [server_id:int=None]

    // Add the permission 'my_plugin.something' to the 'administrator' parent
    sp auth permission parent add administrator my_plugin.something


add_parent
~~~~~~~~~~

Add a parent to a parent. If the given parents don't exist, they will be created.

.. code-block:: none

    // Usage
    // sp auth permission parent add_parent <parent> <parent_to_add>

    // Add the parent 'moderator' to the 'administrator' parent
    sp auth permission parent add_parent administrator moderator


remove
~~~~~~

Remove a permission from a parent.

.. code-block:: none

    // Usage
    // sp auth permission parent remove <parent> <permission> [server_id:int=None]

    // Remove the permissions 'my_plugin.something' from the 'administrator' parent
    sp auth permission parent remove administrator my_plugin.something


remove_parent
~~~~~~~~~~~~~

Remove a parent from a parent.

.. code-block:: none

    // Usage
    // sp auth permission parent remove_parent <parent> <parent_to_remove>

    // Remove the parent 'moderator' from the 'administrator' parent
    permission parent remove_parent administrator moderator


player
""""""

Provide commands to modify player permissions.

.. code-block:: none

    // Usage
    // sp auth permission player <sub-command>


add
~~~

Grant a permission to players.

.. code-block:: none

    // Usage
    // sp auth permission player add <players:filter_str> <permission> [server_id:int=None]

    // Grant all players that are currently on the server the permission 'my_plugin.something'
    sp auth permission player add all my_plugin.something


add_parent
~~~~~~~~~~

Add a parent to players. If the parent does not exist, it will be created.

.. code-block:: none

    // Usage
    // sp auth permission player add_parent <players:filter_str> <parent>

    // Add the parent 'administrator' to all players that are currently on the server
    sp auth permission player add_parent all administrator


remove
~~~~~~

Remove a parent from players.

.. code-block:: none

    // Usage
    // sp auth permission player remove <players:filter_str> <permission> [server_id:int=None]

    // Remove the permission 'my_plugin.something' from all players that are currently on the server
    sp auth permission player remove all my_plugin.something


remove_parent
~~~~~~~~~~~~~

Remove a parent from players.

.. code-block:: none

    // Usage
    // sp auth permission player remove_parent <players:filter_str> <parent>

    // Remove the parent 'administrator' from all players that are currently on the server
    sp auth permission player remove_parent all administrator


test
~~~~

Test which players are granted a specific permission.

.. code-block:: none

    // Usage
    // sp auth permission player test <permission>

    // Tests who has the permission 'my_plugin.something'
    sp auth permission player test my_plugin.something


credits
-------

Display credits for the Source.Python plugin.

.. code-block:: none

    // Usage
    // sp credits

.. seealso:: :doc:`credits <credits>`


delay
-----

Execute a server command after the given delay.

.. code-block:: none

    // Usage
    // sp delay <delay> <command> [arguments]

    // Execute a command after a delay of 2 seconds
    sp delay 2 changelevel cs_office

    // Also can be used to set variables after a delay
    sp delay 2 bot_quota 5


docs
----

Create, generate or build a Sphinx project.

.. code-block:: none

    // Usage
    // sp docs <sub-command> <source-python/custom-package/plugin>


build
^^^^^

Build a Sphinx project.

.. code-block:: none

    // Usage
    // sp docs build <source-python/custom-package/plugin>

    // Build the Source.Python documentation
    sp docs build source-python

    // Build documentation for a plugin
    sp docs build my_plugin


create
^^^^^^

Create a Sphinx project.

.. code-block:: none

    // Usage
    // sp docs create <source-python/custom-package/plugin>

    // Create documentation for a plugin
    sp docs create my_plugin


generate
^^^^^^^^

Generate a Sphinx project.

.. code-block:: none

    // Usage
    // sp docs generate <source-python/custom-package/plugin>

    // Generate documentation files for a plugin
    sp docs generate my_plugin


dump
----

Dump data to a file. The filename given will be created at ../logs/source-python/<filename>.txt

.. code-block:: none

    // Usage
    // sp dump <sub-command>


class_info
^^^^^^^^^^

Dump class information to a file.

.. code-block:: none

    // Usage
    // sp dump class_info <file_name>

    // Dump class information to ../logs/source-python/class_info.txt
    sp dump class_info class_info


convars
^^^^^^^

Dump console variables to a file.

.. code-block:: none

    // Usage
    // sp dump convars <file_name>

    // Dump console variables to ../logs/source-python/convars.txt
    sp dump convars convars


datamaps
^^^^^^^^

Dump datamaps to a file.

.. code-block:: none

    // Usage
    // sp dump datamaps <file_name>

    // Dump datamaps to ../logs/source-python/datamaps.txt
    sp dump datamaps datamaps


server_classes
^^^^^^^^^^^^^^

Dump server classes to a file.

.. code-block:: none

    // Usage
    // sp dump server_classes <file_name>

    // Dump server classes to ../logs/source-python/server_classes.txt
    sp dump server_classes server_classes


string_tables
^^^^^^^^^^^^^

Dump string tables to a file.

.. code-block:: none

    // Usage
    // sp dump string_tables <file_name>

    // Dump string tables to ../logs/source-python/string_tables.txt
    sp dump string_tables string_tables


weapon_scripts
^^^^^^^^^^^^^^

Dump weapon scripts to a file.

.. code-block:: none

    // Usage
    // sp dump weapon_scripts <file_name>

    // Dump weapon scripts to ../logs/source-python/weapon_scripts.txt
    sp dump weapon_scripts weapon_scripts


help
----

List all 'sp' sub-commands or provide help on server commands registered with Source.Python.

.. code-block:: none

    // Usage
    // sp help [command=None] [*sub_commands]

    // List all 'sp' sub-commands
    sp help

    // Print help on 'sp load'
    sp help sp load
    
    
info
----

Print information about the server's operating system, Source.Python and installed plugins.

.. code-block:: none

    // Usage
    // sp info
    
    // Example output
    Date          : 2017-01-15 17:11:05.553293
    OS            : Windows-7-6.1.7601-SP1
    Game          : css
    SP version    : 528
    Server plugins:
       00: Mattie's EventScripts, http://www.eventscripts.com, Version:2.1.1.379
       01: Metamod:Source 1.10.6
       02: Source.Python, (C) 2012-2016, Source.Python Team.
    SP plugins:
       00: paintball, 1.3, http://forums.sourcepython.com/viewtopic.php?f=7&t=502


plugin
------

Provide plugin specific commands.

.. code-block:: none

    // Usage
    // sp plugin <sub-command>


list
^^^^

List all currently loaded plugins.

.. code-block:: none

    // Usage
    // sp plugin list


load
^^^^

Load a plugin by name.

.. code-block:: none

    // Usage
    // sp plugin load <plugin>

    // Load the plugin 'test'
    sp plugin load test


reload
^^^^^^

Reload a plugin by name.

.. code-block:: none

    // Usage
    // sp plugin reload <plugin>

    // Reload the plugin 'test'
    sp plugin reload test


unload
^^^^^^

Unload a plugin by name.

.. code-block:: none

    // Usage
    // sp plugin unload <plugin>

    // Unload the plugin 'test'
    sp plugin unload test


update
------

Update Source.Python to the latest version. A restart of the server is required
to apply the new update.

.. code-block:: none

    // Usage
    // sp update
