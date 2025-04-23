menus
======

This page contains tutorials about the :mod:`menus` package.


Creating PagedMenu
--------------------------

This is a simple example to create a :class:`menus.PagedMenu`, which shows 20 options.
After selecting an option the menu will be shuffled and shown again.

You can use paged menus if you have many menu entries and want Source.Python
to take care about the pagination. If you need more control about the menu
take a look at :class:`menus.SimpleMenu`.

.. code-block:: python

    import random

    from commands.say import SayCommand

    from menus import PagedMenu
    from menus import PagedOption

    from players.entity import Player


    # Register menu command
    @SayCommand(['menu', '/menu', '!menu'])
    def say_command(command, index, teamonly):
        # Send the menu to the player who issued the say command.
        menu.send(index)
        return False

    def my_select_callback(menu, index, option):
        """Called whenever a selection was made."""
    
        # Shuffle the menu : D
        random.shuffle(menu)

        # Make it sticky. If you return a menu, it will be shown immediately.
        return menu

    menu = PagedMenu(
        title='Welcome menu',
        description='Choose an option:',
        select_callback=my_select_callback
    )

    # Add options from 1 to 20
    for i in range(1, 20):
        menu.append(PagedOption(f'Option {i}', i))

    # Instead of passing the select, build and close callbacks to the constructor
    # of PagedMenu, you can also use a decorator to register the callbacks.
    @menu.register_close_callback
    def _on_close_menu(menu, index):
        """Called when the player closes the menu."""
        
        # Send the menu again
        menu.send(index)


Creating SimpleMenu
--------------------------

This is an example to create :class:`menus.SimpleMenu` which asks a player to accept the server rules.
If he declines, he will be kicked from the server.

.. code-block:: python

    import time

    from commands.say import SayCommand

    from menus import SimpleMenu
    from menus import SimpleOption

    from messages import SayText2

    from players.entity import Player

    # Register menu command
    @SayCommand(['menus', '/menus', '!menus'])
    def say_menus_command(command, index, teamonly):
        # Send the menu
        menu.send(index)
        return False

    def my_menu_select_callback(menu, index, option):
        """Called whenever a selection was made."""

        if option.value == 'yes':
            SayText2('Thank you for accepting the rules!').send(index)

        # Player has selected no option
        else:
            # Kick player for selecting no option
            Player(index).kick('You have to accept the rules!')
    
    menu = SimpleMenu()

    # Tell the current time
    menu.append(f"Current Time: {time.strftime('%H:%M:%S')}")

    # Add empty line
    menu.append(' ')
    menu.append('Do you accept the rules?')
    menu.append(' ')

    # Add menu options
    menu.append(SimpleOption(1, 'Yes', 'yes'))
    menu.append(SimpleOption(2, 'No', 'no'))

    # Another way to define a select callback.
    menu.select_callback = my_menu_select_callback


Creating ListMenu
--------------------------

This example shows how to create a :class:`menus.ListMenu`.
The goal of a list menu is to show a lot of data, but the menu entries are not selectable.

.. code-block:: python

    from commands.say import SayCommand

    from menus import ListMenu

    # Register menu command
    @SayCommand(['menus', '/menus', '!menus'])
    def say_menus_command(command, index, teamonly):
        # Send the menu
        menu.send(index)
        return False

    menu = ListMenu()
    
    # Add menu text
    menu.append('This is an example text')