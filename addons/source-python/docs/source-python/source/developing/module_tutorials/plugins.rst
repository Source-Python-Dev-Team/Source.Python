plugins
=======

This page contains tutorials about the :mod:`plugins` package.


PluginInfo
----------

For every plugin a :class:`plugins.info.PluginInfo` instance can be retrieved,
even if the plugin isn't loaded. The :class:`plugins.info.PluginInfo` instance
is created based on a file called ``info.ini`` that has to reside in the
plugin's main directory. This file is completely optional. So, if it doesn't
exist, the :class:`plugins.info.PluginInfo` instance will still be created,
but it won't contain much more information beside the name of the plugin.

Here is an example ``info.ini`` file containing the most basic options:

.. code:: ini

    # A verbose name of the plugin.
    # If this option is not defined, the plugin's name will be used, all
    # underscores are replaced with spaces and the first letter of every word is
    # being capitalized.
    verbose_name = "Paintball"

    # Name of the Author.
    # If this option is not defined, the plugin info will contain 'None'.
    author = "Ayuto"

    # A description of what the plugin does.
    # If this option is not defined, the plugin info will contain 'None'.
    description = "Adds paintball effects to the game."

    # Version of the plugin.
    # If this option is not defined, the plugin info will contain 'unversioned'.
    version = "1.3"

    # An link to the 'Plugin Releases' forum or the plugin's SPPM link (hopefully
    # coming some day).
    # If this option is not defined, the plugin info will contain 'None'.
    url = "http://forums.sourcepython.com/viewtopic.php?f=7&t=502"


You can also note which permissions are defined by your plugin. This helps
server owners to configure their authorization settings properly. See the
example below:

.. code:: ini

    # Permissions defined by the plugin.
    # If this option is not defined, the plugin info will contain an empty list.
    [permissions]
        admin.kick = "Ability to kick a player."
        admin.ban = "Ability to ban a player."


As soon as a plugin is being loaded, a public console variable is created that
contains the following information:

* Name: ``<plugin name>_version``
* Value: ``<version of the plugin>``
* Description: ``<verbose name> version.``


If you don't want a public console variable, you can simply use the following
option in your info file to disable that feature:

.. code:: ini

    public_convar = False


If you wish to use different values to create the public console variable, you
can use the following in your info file:

.. code:: ini

    [public_convar]
        # All of these options are optional.
        name = "my_plugin_version"
        value = "My custom value."
        description = "My custom description."


Sometimes you might also want to define some custom options for the plugin
info. Adding those is quite easy. You just need to define them:

.. code:: ini

    my_custom_option = "something"
    my_custom_option2 = "something else"


Since those are custom options, they are not displayed when the list of loaded
plugins is printed (e.g. via ``sp plugin list``). If you want to change that
behaviour, you can define the ``display_in_listing`` option:

.. code:: ini

    display_in_listing = "my_custom_option", "my_custom_option2"


Retrieving a PluginInfo instance
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. note::

    If you retrieve a :class:`plugins.info.PluginInfo` instance of a plugin
    that isn't loaded, the :class:`plugins.info.PluginInfo` instance is
    recreated everytime you retrieve it. Only loaded plugins will cache the
    instance.


The following example will show how to retrieve a
:class:`plugins.info.PluginInfo` instance for a specific plugin.

.. code:: python

    from plugins.manager import plugin_manager

    # Retrieve the plugin info of the paintball plugin. This is case sensitive!
    info = plugin_manager.get_plugin_info('paintball')

    # Print the plugin's description
    print(info.description)


You are not only restricted to the plugin's name, but you can also use the
plugin's import path. See the example below:

.. code:: python

    from plugins.manager import plugin_manager

    info = plugin_manager.get_plugin_info('paintball.paintball')


Obviously, this doesn't make much sense as the first example is shorter and
both result in the same. But this feature has been added, so plugin's can pass
their own ``__name__`` variable, which contains their import path. Thus, you
can use the following snippet to retrieve the plugin info of your own plugin,
without directly specifying the plugin's name.

.. code:: python

    from plugins.manager import plugin_manager

    info = plugin_manager.get_plugin_info(__name__)


You can also use this snippet outside of your plugin's main file (e.g. in
other sub-modules or sub-packages).


Adding sub-plugins
------------------

.. todo:: Show how to add sub-plugins
