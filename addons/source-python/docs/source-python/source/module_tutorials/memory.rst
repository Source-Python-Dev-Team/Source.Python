memory
======

This page contains tutorials about the :mod:`memory` package.


Calling a virtual function
--------------------------

This is a simple example for calling a virtual function in CS:S.

.. code-block:: python

    import core

    from memory import DataType
    from memory import Convention

    from mathlib import Vector

    from entities.helpers import pointer_from_index
    from entities.helpers import index_from_pointer

    # CBaseEntity* CCSPlayer::GiveNamedItem(char const*, int)
    # Index on Windows is 400. On Linux it is 401.
    GIVE_NAMED_ITEM_INDEX = 400 if core.PLATFORM == 'windows' else 401

    def give_named_item(index, item, slot=0):
        # Retrieve the player's pointer.
        ptr = pointer_from_index(index)

        # Now, we can get and wrap the virtual function. To do this, we need
        # to pass the index of the virtual function in the virtual function table
        # of the CCSPlayer class. This index might change after a game update.

        # We also need to specify the calling convention. Since it's a normal
        # member function, the calling convention is THISCALL.

        # For the third argument a tuple is required that defines the types of the
        # arguments of the function. In this case we need to pass a pointer
        # (the this-pointer), a string that specifies the item that should be
        # given and an integer that defines the slot/sub type of the item.

        # For the fourth and last argument we need to define the return type of the
        # virtual function. In this case the function returns a pointer to the
        # created entity.
        GiveNamedItem = ptr.make_virtual_function(
            GIVE_NAMED_ITEM_INDEX,
            Convention.THISCALL,
            (DataType.POINTER, DataType.STRING, DataType.INT),
            DataType.POINTER
        )

        # Finally, we can call the virtual function! Since many functions in
        # Source.Python's API require an index instead of a pointer, we will
        # convert the returned pointer to an index.
        return index_from_pointer(GiveNamedItem(ptr, item, slot))

    # Usage example:
    # entity_index = give_named_item(my_player_index, 'weapon_awp')


Calling a virtual function with non atomic arguments
----------------------------------------------------

This example will cover calling a virtual function which requires two non atomic arguments. This example was made for CS:S.

.. code-block:: python

    import core

    from memory import DataType
    from memory import Convention

    from mathlib import Vector

    from entities.helpers import pointer_from_index

    # void CBaseAnimating::GetVelocity(Vector*, Vector*)
    # Index on Windows is 140. On Linux it is 141.
    GET_VELOCITY_INDEX = 140 if core.PLATFORM == 'windows' else 141

    def get_velocity(index):
        # Retrieve the entity's pointer.
        ptr = pointer_from_index(index)

        # Now, get and wrap the virtual function. Again, we need to pass the
        # virtual function index and the calling convention.

        # The tuple that defines the types of the arguments will now contain three
        # times Argument.POINTER. The first pointer stands for the this-pointer.
        # The other two stand for the Vector* arguments.

        # This time the function doesn't return anything. Instead the return value
        # is "returned" by modifying the two Vector arguments. This is a common
        # practice in C/C++ to return multiple return values.
        GetVelocity = ptr.make_virtual_function(
            GET_VELOCITY_INDEX,
            Convention.THISCALL,
            (DataType.POINTER, DataType.POINTER, DataType.POINTER),
            DataType.VOID
        )

        # Since Source.Python exposes the Vector class, we don't even need to
        # reconstruct the memory structure of the both vectors. So, we just need
        # to create two new Vector objects.
        velocity = Vector()
        angle = Vector()

        # Finally, call the function! But what's happening here? Source.Python
        # converts both Vector objects to a Pointer object at first and then
        # accesses the memory addresses of the Vector objects which were allocated
        # internally. Pretty cool, eh? This works with every object whose class
        # was exposed by Source.Python and every object that is an instance of
        # CustomType.
        GetVelocity(ptr, velocity, angle)

        # After the function modified the vectors just return them.
        return (velocity, angle)
