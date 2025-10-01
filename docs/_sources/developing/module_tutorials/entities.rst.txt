entities
========

This page contains tutorials about the :mod:`entities` package. Basically, the
entities package provides functions and classes to create, modify, access and
delete entities. An entity is an object somewhere on the map that has a specific
type (class name) and different properties (e. g. health, position, team index,
speed, model, etc.).

There are two types of entities:

* Networked entities
* Non-networked entities

``Networked`` already indicates that those entities are sent over the network.
Their information is shared with all players on the server. Some common examples
for those entities are players, weapons, barrels or trigger entities.
Non-networked entities are entities that only exist on the server or client, but
not on both sides. Entities are uniquely identified by an index that
starts at 0 and can go up to the
`entity limit <https://developer.valvesoftware.com/wiki/Entity_limit>`_.

.. note::

    Indexes uniquely identify an entity. However, if the entity is deleted, a
    new entity can reuse that index.


You will most likely want to access networked entities. Those are the most
common and interesting ones.

There are multiple classes in Source.Python to access the various entity types:

* :class:`entities.entity.BaseEntity` - A very basic, but fast class to access networked and non-networked entities.
* :class:`entities.entity.Entity` - An extended version of the :class:`entities.entity.BaseEntity` class. It provides a lot convenience methods, but is only able to access networked entities.
* :class:`players.entity.Player` - An extended version of the :class:`entities.entity.Entity` class that provides additional methods to manipulate players.
* :class:`weapons.entity.Weapon` - An extended version of the :class:`entities.entity.Entity` class that provides additional methods for weapons.

This tutorial will only cover :class:`entities.entity.BaseEntity` and
:class:`entities.entity.Entity`. For the other two classes, there will be
separate tutorials.


Accessing entities
------------------

If you know the index of an entity, it's quite simple to access it.

.. code-block:: python

    from entities.entity import Entity

    # Access the entity with index 0. The world entity always has the index 0.
    entity = Entity(0)

    # Print the class name of the entity. This will print "worldspawn".
    print(entity.classname)


However, often you need to search for entities of a specific type. E. g. to get
all buy zones:

.. code-block:: python

    # Import the entity iterator
    from filters.entities import EntityIter

    # Loop through all buy zones
    for buy_zone in EntityIter('func_buyzone'):

        # Print the team index of the buy zone. Only teams with the same index can
        # use this buy zone.
        print(f'Buy zone for team: {buy_zone.team_index}')

        # If you are not used to Python 3.6, the print statement might be
        # confusing at first. Alternatively, you could also write:
        # print('Buy zone for team: {}'.format(buy_zone.team_index))


You might wonder how to know which entity types are currently available on the
map. You can easily discover that by using the following snippet.

.. code-block:: python

    # Import the entity iterator.
    from filters.entities import EntityIter

    # Get the class name of every entity on the map. Use set() to get a distinct
    # set.
    classnames = set(entity.classname for entity in EntityIter())

    # Sort all class names alphabetically and then print them.
    for classname in sorted(classnames):
        print(classname)


Modifying entities
------------------

In the previous chapter we looped through all buy zones to print their team
indexes. Now, let's loop through all doors on the map and open them.

.. code-block:: python

    # Import the entity iterator
    from filters.entities import EntityIter

    # Loop through all doors
    for door in EntityIter('prop_door_rotating'):

        # Open the door
        door.open()

        # Or, if you are tired of the doors, simply remove them with:
        # door.remove()


If you just tried to find information about ``door.open()`` on the wiki, you
probably didn't find anything except this tutorial. This is because of all the
different entity types. They all have different properties and methods, which
also differs from game to game. Thus, you can only print these information at
runtime. To do so, you can use the following snippet.

.. code-block:: python

    from entities.entity import Entity

    # Find an entity with the class name "prop_door_rotating". If no entity was
    # found, it will be created.
    entity = Entity.find_or_create('prop_door_rotating')

    # Loop through all attributes
    for attr in dir(entity):

        # Print the attribute
        print(attr)


Creating new entities
---------------------

Creating new entities is quite simple. Often you just need to create an entity
of the desired type, assign it a model and then spawn it. Here is how to do
that.

.. code-block:: python

    # Import the required classes.
    from entities.entity import Entity
    from players.entity import Player
    from engines.precache import Model
    from events import Event

    # Wrap the model (a barrel) with the Model class. It will ensure that the model
    # is precached, when you want to use it. Precaching is important. Otherwise
    # the model might not work.
    my_model = Model('models/props_c17/oildrum001.mdl')

    # Listen to the player_say event. It gets fired everytime a player says
    # something in the chat.
    @Event('player_say')
    def on_player_say(event):
        # Get a Player instance for the player who typed something into the chat.
        player = Player.from_userid(event['userid'])

        # Create an entity of the type "prop_physics_multiplayer".
        entity = Entity.create('prop_physics_multiplayer')

        # Assign the model.
        entity.model = my_model

        # Set the location where the entity should be created. In this case we want
        # to spawn it where the player is currently looking at.
        entity.origin = player.view_coordinates

        # Finally spawn the entity.
        entity.spawn()


.. note::

    The model ``models/props_c17/oildrum001.mdl`` already exists in CS:S. In
    other games this might be different.
