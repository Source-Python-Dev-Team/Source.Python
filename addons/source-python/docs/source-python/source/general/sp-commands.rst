Console commands
================

The core.command module registers the "sp" console commands and several sub-commands.


load
----

Load a plugin by name.

.. code-block:: none

    // Usage
    // sp load <plugin>

    // Load a plugin
    sp load test


unload
------

Unload a plugin by name

.. code-block:: none

    // Usage
    // sp unload <plugin>

    // Unload a plugin
    sp unload test


reload
------

Reload a plugin by name

.. code-block:: none

    // Usage
    // sp reload <plugin>

    // Reload a plugin
    sp reload test


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


dump
----

Dump data to log file. The filename given will be created at ../logs/source-python/<filename>.txt

.. code-block:: none

    // Usage
    // sp dump <dump_type> <filename>

    // Dump the StringTables
    sp dump string_tables test


list
----

List all currently loaded plugins.

.. code-block:: none

    // Usage
    // sp list

    // List all currently loaded plugins
    sp list


version
-------

Print Source.Python version information.

.. code-block:: none

    // Usage
    // sp version

    // Print the Source.Python version information
    sp version


credits
-------

List all credits for the Source.Python plugin.

.. code-block:: none

    // Usage
    // sp credits

    // List all credits for the Source.Python plugin
    sp credits


help
----

List all 'sp' sub-commands.

.. code-block:: none

    // Usage
    // sp help

    // List all 'sp' sub-commands
    sp help


docs
----

Create, generate or build a Sphinx project.

.. code-block:: none

    // Usage
    // sp docs <create/generate/build> <source-python/custom-package/plugin>

    // Build the documentation for Source.Python
    sp docs build source-python


auth
----

The auth.commands module registers further sub-commands for the "auth" sub-command.


set
^^^

Sets the provided authorization service(s).

.. code-block:: none

    // Usage
    // sp auth set <provider>
    sp auth set sql


list
^^^^

Lists the available authorization services.

.. code-block:: none

    // Usage
    // sp auth list

    // Lists the available authorization services.
    sp auth list


permission player add
^^^^^^^^^^^^^^^^^^^^^

Adds a permission node to a player

.. code-block:: none

    // Usage
    // sp auth permission player add <playerfilter> <permission node> [server_id]

    // Adds a permission node to a player
    sp auth permission player add all admin.permission.node 1



permission player remove
^^^^^^^^^^^^^^^^^^^^^^^^

Removes a permission node from a player

.. code-block:: none

    // Usage
    // sp auth permission player remove <playerfilter> <permission node> [server_id]

    // Removes a permission node from a player
    permission player remove all admin.permission.node 1



permission player add_parent
^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Adds a parent to a player

.. code-block:: none

    // Usage
    // sp auth permission player add_parent <playerfilter> <parent>

    // Adds a parent to a player
    permission player add_parent all administrator



permission player remove_parent
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Removes a parent from a player

.. code-block:: none

    // Usage
    // sp auth permission player remove_parent <playerfilter> <parent>

    // Removes a parent from a player
    permission player remove_parent all administrator



permission player test
^^^^^^^^^^^^^^^^^^^^^^

Tests to see if a players has a permission node

.. code-block:: none

    // Usage
    // sp auth permission player test <playerfilter> <permission node>

    // Tests to see if a players has a permission node
    permission player test all admin.permission.node



permission parent add
^^^^^^^^^^^^^^^^^^^^^

Adds a permission node to a parent group

.. code-block:: none

    // Usage
    // sp auth permission parent add <parent> <permission node>

    // Adds a permission node to a parent group
    permission parent add administrator admin.permission.node



permission parent remove
^^^^^^^^^^^^^^^^^^^^^^^^

Removes a permission node from a parent group

.. code-block:: none

    // Usage
    // sp auth permission parent remove <parent> <permission node>

    // Removes a permission node from a parent group
    permission parent remove administrator admin.permission.node



permission parent add_parent
^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Adds a parent group to a parent

.. code-block:: none

    // Usage
    // sp auth permission parent add_parent <parent> <parent to add>

    // Adds a parent group to a parent
    permission parent add_parent administrator moderator



permission parent remove_parent
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Removes a parent group from a parent

.. code-block:: none

    // Usage
    // sp auth permission parent remove_parent <parent> <parent to add>

    // Removes a parent group from a parent
    permission parent remove_parent administrator moderator


help
^^^^

List all 'sp auth' sub-commands.

.. code-block:: none

    // Usage
    // sp auth help

    // List all 'sp auth' sub-commands
    sp auth help
