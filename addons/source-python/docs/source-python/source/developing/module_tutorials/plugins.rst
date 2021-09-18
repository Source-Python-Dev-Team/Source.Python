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

Adding sub-plugins to your plugin is done a very few steps. All you actually
need is a new instance of the :class:`plugins.manager.PluginManager` class.
This instance allows you to load plugins from a specific directory.

Imagine your plugin resides in ``../addons/source-python/plugins/my_plugin``
and within that directory you have created a new directory called ``plugins``,
which should contain all sub-plugins of ``my_plugin``. Then the plugin manager
could be created using the following code:

.. code:: python

    from plugins.manager import PluginManager

    my_plugin_manager = PluginManager('my_plugin.plugins.')


That's all you need! Now you can load sub-plugins using ``my_plugin_manager``
from your sub-plugins directory with the following code:

.. code:: python

    # Load the plugin 'my_sub_plugin' from
    # ../addons/source-python/plugins/my_plugin/plugins
    my_plugin_manager.load('my_sub_plugin')


However, this doesn't print any messages like Source.Python does when you load
a plugin via ``sp plugin load``. If you would like to have those messages as
well, without implementing them on your own, you can simply create an instance
of ``plugins.command.SubCommandManager``.

.. code:: python

    from plugins.command import SubCommandManager

    my_sub_command_manager = SubCommandManager(
        # Tell the sub command manager to use this plugin manager to load plugins
        my_plugin_manager,

        # If you create sub-commands, they will use 'my_plugin' as the base
        # command like Source.Python uses 'sp'
        'my_plugin'
    )


Now, you can load your sub-plugin using the following code:

.. code:: python

    my_sub_command_manager.load_plugin('my_sub_plugin')


So far, so good. But what if you want to load your plugins via a server
command? Well, just add it using the following code:

.. code:: python

    @my_sub_command_manager.server_sub_command(['plugin', 'load'])
    def plugin_load(command_info, plugin):
        my_sub_command_manager.load_plugin(plugin)

    @my_sub_command_manager.server_sub_command(['plugin', 'unload'])
    def plugin_unload(command_info, plugin):
        my_sub_command_manager.unload_plugin(plugin)


Now you can also load your sub-plugins using ``my_plugin plugin load`` and
unload them using ``my_plugin plugin unload``.

There is only one last thing left to do. If your main plugin is being
unloaded, you should also unload all of your sub-plugins. It doesn't cause any
problems with Source.Python if you don't do that, because Source.Python also
unloads all :class:`core.AutoUnload` and :class:`core.WeakAutoUnload`
instances of your sub-plugins. But if you don't do that the ``unload``
functions of your sub-plugins are never getting called. To avoid this issue
use the following code:

.. code:: python

    def unload():
        for plugin in my_plugin_manager.loaded_plugins:
            plugin.unload()


Here is the full example code to implement sub-plugins:

.. code:: python

    from plugins.manager import PluginManager
    from plugins.command import SubCommandManager

    my_plugin_manager = PluginManager('my_plugin.plugins.')
    my_sub_command_manager = SubCommandManager(
        # Tell the sub command manager to use this plugin manager to load plugins
        my_plugin_manager,

        # If you create sub-commands, they will use 'my_plugin' as the base
        # command like Source.Python uses 'sp'
        'my_plugin'
    )

    @my_sub_command_manager.server_sub_command(['plugin', 'load'])
    def plugin_load(command_info, plugin):
        my_sub_command_manager.load_plugin(plugin)

    @my_sub_command_manager.server_sub_command(['plugin', 'unload'])
    def plugin_unload(command_info, plugin):
        my_sub_command_manager.unload_plugin(plugin)

    def unload():
        for plugin in my_plugin_manager.loaded_plugins:
            plugin.unload()
