events
======

This page contains tutorials about the :mod:`events` package.


Creating a custom event
-----------------------

When creating a custom event, you will also want to look at the
:mod:`events.resource` and :mod:`events.variable` modules.

The first thing you want to do is create your class. The lower-case name of
the class will be used as the name of the event. Inside the class, you will
need to declare the event variables by the type they are (using classes from
the :mod:`events.variable` module).

.. code-block:: python

    from events.custom import CustomEvent
    from events.variable import FloatVariable
    from events.variable import ShortVariable
    from events.variable import StringVariable


    # Create the event's class
    class My_New_Event(CustomEvent):

        # Declare the variables with their type
        userid = ShortVariable('The userid of the player involved in the event.')
        pos_x = FloatVariable('The x value of some location in the event.')
        pos_y = FloatVariable('The y value of some location in the event.')
        pos_z = FloatVariable('The z value of some location in the event.')
        weapon = StringVariable('The weapon used in the event.')

The next step is to create the resource file and load the events. For more
information, look to the :mod:`events.resource` module.

.. code-block:: python

    from events.resource import ResourceFile

    resource_file = ResourceFile('my_custom_events', My_New_Event)
    resource_file.write()
    resource_file.load_events()

The last step is actually using your custom event class to fire the event. You
can do this one of two ways.  First, just create an instance, set the
variables, then fire the event.

.. code-block:: python

    # Create the event
    event_instance = My_New_Event()

    # Set the variable values
    event_instance.userid = 2
    event_instance.pos_x = 1.111
    event_instance.pos_y = 2.222
    event_instance.pos_z = 3.333
    event_instance.weapon = 'weapon_knife'

    # Fire the event
    event_instance.fire()

The other way, is to set the variables on instantiation.

.. code-block:: python

    # Create the event and define the variable values on instantiation
    event_instance = My_New_Event(userid=2, pos_x=1.111, pos_y=2.222, pos_z=3.333, weapon='weapon_knife')

    # Fire the event
    event_instance.fire()

You can also use context management to automatically fire the event on exit.

.. code-block:: python

    # Create the event and fire on exit
    with My_New_Event() as event_instance:

        # Define the event variable values
        event_instance.userid = 2
        event_instance.pos_x = 1.111
        event_instance.pos_y = 2.222
        event_instance.pos_z = 3.333
        event_instance.weapon = 'weapon_knife'
