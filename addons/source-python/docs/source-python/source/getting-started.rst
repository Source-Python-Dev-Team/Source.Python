Getting started
===============

If you are new to Source.Python this is the right place for you!

In order to use Source.Python you need to install it on your game server. To
do so, please follow the instructions described :doc:`here <installation>`.

As soon as you have successfully installed Source.Python we can start writing
your first plugin.


Writing your first plugin
-------------------------

The first plugin will be a very simple one. It should simply print a message
to all players, when the plugin has been loaded, and another messages, when
the plugin has been unloaded.

.. code-block:: python

    from messages import SayText2

    def load():
        SayText2('Plugin has been loaded successfully!').send()

    def unload():
        SayText2('Plugin has been unloaded successfully!').send()


What's next?
------------

Nothing here...