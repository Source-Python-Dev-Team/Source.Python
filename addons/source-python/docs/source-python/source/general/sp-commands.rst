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


load
^^^^

Loads the provided authorization service(s).

.. code-block:: none

    // Usage
    // sp auth load <provider> [provider] ...

    // Load one provider
    sp auth load simple

    // Multiples can be loaded at the same time.
    // Note that currently only "simple" is included in Source.Python.
    sp auth load simple sourcemod


unload
^^^^^^

Unloads the provided authorization service(s).

.. code-block:: none

    // Usage
    // sp auth unload <provider> [provider] ...

    // Unload one provider
    sp auth unload simple

    // Multiples can be unloaded at the same time.
    // Note that currently only "simple" is included in Source.Python.
    sp auth unload simple sourcemod


reload
^^^^^^

Reloads the provided authorization service(s). If no service(s) are provided, it will reload all currently loaded services.

.. code-block:: none

    // Usage
    // sp auth reload [provider] [provider] ...

    // Reload one provider
    sp auth reload simple

    // Multiples can be reloaded at the same time.
    // Note that currently only "simple" is included in Source.Python.
    sp auth reload simple sourcemod

    // Reload all currently loaded authorization services
    sp auth reload


list
^^^^

List all currently loaded authorization services.

.. code-block:: none

    // Usage
    // sp auth list

    // List all currently loaded authorization services
    sp auth list


help
^^^^

List all 'sp auth' sub-commands.

.. code-block:: none

    // Usage
    // sp auth help

    // List all 'sp auth' sub-commands
    sp auth help
