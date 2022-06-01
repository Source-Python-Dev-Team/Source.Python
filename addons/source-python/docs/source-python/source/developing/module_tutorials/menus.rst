Menus
======

This page contains tutorials about the :mod:`menus` package.


Creating PagedMenu
--------------------------

This is a simple example to create paged menu

.. code-block:: python

    import random

    from commands.say import SayCommand

    from menus import PagedMenu
    from menus import PagedOption

    from players.entity import Player


    # Register menu command
    @SayCommand(['menu', '/menu', '!menu'])
    def say_command(command, index, teamonly):
        # Send the menu
        menu.send(index)
        return False

    menu = PagedMenu(title='Welcome menu', description='Choose an option:', select_callback=my_select_callback)

    # Add options from 1 to 20
    for i in range(1, 20):
        menu.append(PagedOption(f'{i}', i))

    def my_select_callback(menu, index, option):
        '''
        Called whenever a selection was made.
        '''
    
        # Shuffle the menu : D
        random.shuffle(menu)

        # Make it sticky
        return menu


Creating SimpleMenu
--------------------------

This is a simple example to create simple menu

.. code-block:: python

    import time

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
        menu.send(index)
        return False

    
    menu = SimpleMenu()
    # Tell the current time
    menu.append(Text(f"Current Time: {time.strftime('%H:%M:%S')}"))
    # Add empty line
    menu.append(Text(' '))
    menu.append(Text('Do you accept the rules?'))
    menu.append(Text(' '))
    # Add in menu options
    menu.append(SimpleOption(1, 'Yes', 'yes'))
    menu.append(SimpleOption(2, 'No', 'no'))
    menu.select_callback=my_menu_select_callback


    def my_menu_select_callback(menu, index, option):
        if option.value == 'yes':
            SayText2('Thank you for accepting the rules!').send(index)
        # Player selected no option
        else:
            Player(index).kick('You have to accept the rules!')

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
        menu.send(index)
        return False

    menu = ListMenu()
    # Add in menu text
    menu.append(Text('This is a example text'))