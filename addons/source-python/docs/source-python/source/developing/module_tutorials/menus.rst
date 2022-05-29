Menus
======

This page contains tutorials about the :mod:`menus.radio` package.


Creating PagedMenu
--------------------------

This is a simple example to create paged menu

.. code-block:: python

    from commands.say import SayCommand

    from menus import PagedMenu
    from menus import PagedOption

    from players.entity import Player


    # Register menu command
    @SayCommand(['menu', '/menu', '!menu'])
    def say_command(command, index, teamonly):
        # Send the menu
        build_paged_menu(index)
        return False

    def build_paged_menu(index):
        # Create the Paged Menu and set title to My Menu
        menu = PagedMenu(title='My Menu')
        # Add options from 1 to 20
        for i in range(1, 20):
            menu.append(PagedOption(f'{i}', i))
        menu.select_callback=my_select_callback
        menu.send(index)

    def my_select_callback(menu, index, option):
        # Create player object from index
        player = Player(index)
        # Print the player name who have selected a menu option
        print(f'{player.name} have selected a menu option!')


Creating SimpleMenu
--------------------------

This is a simple example to create simple menu

.. code-block:: python

    from commands.say import SayCommand

    from menus import SimpleMenu
    from menus import SimpleOption
    from menus import Text

    from messages import SayText2

    from players.entity import Player

    # Register menu command
    @SayCommand(['menus', '/menus', '!menus'])
    def say_menus_command(command, index, teamonly):
        # Send the menu
        build_simple_menu(index)
        return False

    def build_simple_menu(index):
        # Create the SimpleMenu
        menu = SimpleMenu
        # Add in menu text
        menu.append(Text('Do you accept the rules?'))
        menu.append(Text(' '))
        # Add in menu options
        menu.append(SimpleOption(1, 'Yes', 'yes'))
        menu.append(SimpleOption(2, 'No', 'no'))
        menu.select_callback=my_menu_select_callback
        menu.send(index)

    def my_menu_select_callback(menu, index, option):
        choice = option.value
        # Create player object from index
        player = Player(index)
        # Player did select yes option
        if choice == 'yes':
            SayText2('Thank you for accepting the rules!').send(index)
        # Player selected no option
        else:
            player.kick('You have to accept the rules!')

Creating ListMenu
--------------------------

This is a simple example to create list menu

.. code-block:: python

    from commands.say import SayCommand

    from menus import ListMenu
    from menus import Text

    # Register menu command
    @SayCommand(['menus', '/menus', '!menus'])
    def say_menus_command(command, index, teamonly):
        # Send the menu
        build_list_menu(index)
        return False

    def build_simple_menu(index):
        # Create the ListMenu
        menu = ListMenu()
        # Add in menu text
        menu.append(Text('This is a example text'))
        menu.send(index)