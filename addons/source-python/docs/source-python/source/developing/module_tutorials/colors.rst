colors
======

This page contains tutorials about the :mod:`colors` package.


Do's and Don'ts
---------------

When utilizing a color constant, you must not change it's RGBA values as that
will change the value of the constant itself:

.. code-block:: python

    from colors import RED

    print([x for x in RED])

    RED.r = 128

In the above example, if you reload the plugin, the value for RED.r has been
permanently changed. If you wish to get a different color, use the following
instead:

.. code-block:: python

    from colors import Color
    from colors import RED

    # You can just set the color directly using the Color object
    my_red = Color(128, 0, 0, 255)

    # Or, you can utilize RED itself to create a different value
    my_red = Color(*RED)
    my_red.r = 128

Getting a different alpha value is a little bit easier:

.. code-block:: python

    from colors import RED

    # Again, you DO NOT want to use the following
    RED.a = 128

    # Instead, use the "with_alpha" method of the Color object
    my_red = RED.with_alpha(128)
