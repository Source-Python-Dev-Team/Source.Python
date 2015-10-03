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
