config
======

This page contains tutorials about the :mod:`config` package.


Creating a configuration file with a single variable
----------------------------------------------------

The entry point to the config module is the :class:`config.manager.ConfigManager`
class. This class is effectively a representation of your configuration file
in Python. It allows you to perform a number of operations to your config
file. You can add console variables as well as commands to execute. The
:class:`config.manager.ConfigManager` class will also place information about
each console variable in a comment block above the actual variable. Here is an
example script that creates a configuration file called ``addon.cfg`` in
``../cfg/source-python/``. This configuration file contains a single variable
called ``addon_player_damage`` which defaults to 64:

.. code-block:: python

    # =============================================================================
    # >> IMPORTS
    # =============================================================================
    # Source.Python Imports
    #   Config
    from config.manager import ConfigManager
    #   Cvars
    from cvars.flags import ConVarFlags

    # Create the configuration file.
    addon_config = ConfigManager('addon')

    # Modify the header of the entire configuration file.
    addon_config.header = 'Addon configuration file.'

    # Create a console variable to add to the configuration file.
    addon_player_damage = addon_config.cvar(
        'addon_player_damage', '64',
        'How much damage to cause to each player.', ConVarFlags.CHEAT
    )

    # Write the configuration file out to the folder.
    addon_config.write()

    # Execute the configuration file.
    addon_config.execute()

And here is the resulting addon.cfg file:

.. code-block:: none

    // ######################################################################### //
    //                         Addon configuration file.                         //
    // ######################################################################### //

    // Default Value: 64
    // How much damage to cause to each player.
       addon_player_damage "64"


Adding additional documentation to console variables
----------------------------------------------------

The config package allows you to write in additional information about a given
console variable. These appear as a header comment section above the variable.
The following example script adds a comment block above ``addon_player_damage``
called ``Description``:

.. code-block:: python

    # =============================================================================
    # >> IMPORTS
    # =============================================================================
    # Source.Python Imports
    #   Config
    from config.manager import ConfigManager
    #   Cvars
    from cvars.flags import ConVarFlags

    # =============================================================================
    # >> IMPLEMENTATION
    # =============================================================================

    # Create the configuration file.
    addon_config = ConfigManager('addon')

    # Modify the header of the entire configuration file.
    addon_config.header = 'Addon configuration file.'

    # Create a console variable. Note that this returns a
    # _CvarManager instance which I can then add additional
    # lines of text to. Note: This actually creates the
    # console variable.
    addon_player_damage = addon_config.cvar(
        'addon_player_damage', '64',
        'How much damage to cause to each player.', ConVarFlags.CHEAT
    )

    # I want to add some extra documentation for this variable.
    # This creates a comment block called 'Description' right above
    # the console command.
    addon_player_damage.Description.append(
        'Here is additional documentation on addon_player_damage.')

    addon_player_damage.Description.append(
        'Even more documentation.')

    # Write the configuration file out to the folder.
    # Any values modified by the server administrator will
    # remain unchanged.
    addon_config.write()

    # Execute the configuration file.
    addon_config.execute()

Which results in the following configuration file:

.. code-block:: none

    // ######################################################################### //
    //                         Addon configuration file.                         //
    // ######################################################################### //

    // Description
    //   * Here is additional documentation on addon_player_damage.
    //   * Even more documentation.
    // Default Value: 64
    // How much damage to cause to each player.
       addon_player_damage "64"

Couple of key takeaways from this example:

1. The :meth:`config.manager.ConfigManager.cvar` and :meth:`config.manager.ConfigManager.command` methods return objects which you can add string fields to. These fields get written as comment blocks above your console variable.
2. A server operator can change the default values in the configuration file. These changes will remain even if you call :meth:`config.manager.ConfigManager.write`.


Adding sections
---------------

Adding sections to your configuration file is extremely simple. Just call the
:meth:`config.manager.ConfigManager.section` method. You can also pass in a
character to use in order to create the section boundaries:

.. code-block:: python

    # =============================================================================
    # >> IMPORTS
    # =============================================================================
    # Source.Python Imports
    #   Config
    from config.manager import ConfigManager
    #   Cvars
    from cvars.flags import ConVarFlags

    # =============================================================================
    # >> IMPLEMENTATION
    # =============================================================================

    # Create the configuration file.
    addon_config = ConfigManager('addon')

    # Modify the header of the entire configuration file.
    addon_config.header = 'Addon configuration file.'

    # Create a console variable. Note that this returns a
    # _CvarManager instance which I can then add additional
    # lines of text to. Note: This actually creates the
    # console variable.
    addon_player_damage = addon_config.cvar(
        'addon_player_damage', '64',
        'How much damage to cause to each player.', ConVarFlags.CHEAT
    )

    # I want to add some extra documentation for this variable.
    # This creates a comment block called 'Description' right above
    # the console command.
    addon_player_damage.Description.append(
        'Here is additional documentation on addon_player_damage.')

    addon_player_damage.Description.append(
        'Even more documentation.')

    # Add in a custom section now. Use the '$' sign to designate
    # boundaries.
    custom_section = addon_config.section('My custom section', '$')

    # Add another variable.
    addon_player_model = addon_config.cvar(
        'addon_player_model', 'models/player.mdl',
        'Model to set on the player.', ConVarFlags.CHEAT
    )

    # Write the configuration file out to the folder.
    # Any values modified by the server administrator will
    # remain unchanged.
    addon_config.write()

    # Execute the configuration file.
    addon_config.execute()

This generates the following configuration file:

.. code-block:: none

    // ######################################################################### //
    //                         Addon configuration file.                         //
    // ######################################################################### //

    // Description
    //   * Here is additional documentation on addon_player_damage.
    //   * Even more documentation.
    // Default Value: 64
    // How much damage to cause to each player.
       addon_player_damage "64"


    // $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ //
    //                             My custom section                             //
    // $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ //

    // Default Value: models/player.mdl
    // Model to set on the player.
       addon_player_model "models/player.mdl"


Using context management
------------------------

All of the above examples show explicitly calling the :meth:`config.manager.ConfigManager.write`
and :meth:`config.manager.ConfigManager.execute` methods. You can also
automatically call these by using context management:

.. code-block:: python

    # =============================================================================
    # >> IMPORTS
    # =============================================================================
    # Source.Python Imports
    #   Config
    from config.manager import ConfigManager
    #   Cvars
    from cvars.flags import ConVarFlags

    # Create the configuration file.
    # Using 'with' will utilize the __enter__ and __exit__ methods to
    # automatically write and execute the configuration file.
    with ConfigManager('addon') as addon_config:

        # Modify the header of the entire configuration file.
        addon_config.header = 'Addon configuration file.'

        # Create a console variable to add to the configuration file.
        addon_player_damage = addon_config.cvar(
            'addon_player_damage', '64',
            'How much damage to cause to each player.', ConVarFlags.CHEAT
        )

For reference: :mod:`contextlib`
