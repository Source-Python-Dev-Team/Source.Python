commands
========

This page contains tutorials about the :mod:`commands` package.


Typed commands
--------------

The typed commands API allows you to create robust server, say and client
commands that are protected against users entering invalid data. If a user has
entered invalid data, he will be notified with an understandable error
message and your callback won't get called. Invalid data can be an invalid
value for an argument or simply too many/less arguments. A quick example will
make this more clear.

.. code-block:: python

    from commands.typed import TypedServerCommand

    @TypedServerCommand('multiply')
    def on_test(command_info, x:int, y:int):
        """Multiply x and y and print the result."""
        print('Result:', x * y)


Example input/output:

.. code-block:: none

    multiply
    Not enough arguments:
      multiply <x:int> <y:int>

    multiply 10 5
    Result: 50

    multiply 10 5 3
    Too many arguments:
      multiply <x:int> <y:int>

    multiply 10 a
    'a' is an invalid value for 'y:int'.

    sp help multiply
    multiply <x:int> <y:int>
      Multiply x and y and print the result.


As you can see the API does all the error checking for you.

There are 3 typed command decorator classes in total.

1. :class:`commands.typed.TypedServerCommand` to create server commands.
2. :class:`commands.typed.TypedSayCommand` to create say commands.
3. :class:`commands.typed.TypedClientCommand` to create client commands.

These 3 classes share the same constructor
(:meth:`commands.typed._TypedCommand.__init__`). The only difference is that
:class:`commands.typed.TypedServerCommand` ignores the ``permission`` and
``fail_callback`` arguments. Another thing to remember is which attributes of
the :class:`commands.typed.CommandInfo` instance are set in each of these
decorator types.

1. :class:`commands.typed.TypedServerCommand` only sets the ``command`` attribute.
2. :class:`commands.typed.TypedSayCommand` sets the attributes ``command`` and ``index``.
3. :class:`commands.typed.TypedClientCommand` sets all of the 3 attributes (``command``, ``index`` and ``team_only``).


Creating optional arguments
^^^^^^^^^^^^^^^^^^^^^^^^^^^

You can create optional arguments by assigning a default value to an argument.

.. code-block:: python

    from commands.typed import TypedServerCommand

    @TypedServerCommand('add')
    def on_add(command_info, x:int, y:int=10):
        """Add x and y and print the result."""
        print('Result:', x + y)


Example input/output:

.. code-block:: none

    add
    Not enough arguments:
      add <x:int> [y:int=10]

    add 5
    Result: 15

    add 5 3
    Result: 8


Creating variadic commands
^^^^^^^^^^^^^^^^^^^^^^^^^^

You can create variadic commands by making the decorated function variadic using the asterisk (*).

.. code-block:: python

    @TypedServerCommand('add')
    def on_add(command_info, *args:int):
        """Add all passed arguments and print the result."""
        print('Result:', sum(args))


Example input/output:

.. code-block:: none

    add
    Result: 0

    add 3
    Result: 3

    add 3 5
    Result: 8

    add 3 5 2
    Result: 10

    add 3 a 4
    'a' is an invalid value for 'args:int'.

    sp help add
    add [*args:int]
      Add all passed arguments and print the result.


If you create variadic commands, you are not restricted to only use variadic
arguments. You can still use optional and required arguments.

.. code-block:: python

    from commands.typed import TypedServerCommand

    @TypedServerCommand('add')
    def on_add(command_info, x, y=None, *args:int):
        """Add all passed arguments and print the result."""
        print('Result:', (x, y), sum(args))


Example input/output:

.. code-block:: none

    add
    Not enough arguments:
      add <x> [y=None] [*args:int]

    add a
    Result: ('a', None) 0

    add a b
    Result: ('a', 'b') 0

    add a b 5
    Result: ('a', 'b') 5

    add a b 5 10
    Result: ('a', 'b') 15


Creating sub-commands
^^^^^^^^^^^^^^^^^^^^^

You can easily create sub-commands. All you need to do is passing an iterable as the first argument.

.. code-block:: python

    from commands.typed import TypedServerCommand

    @TypedServerCommand(['test', 'a'])
    def on_test(command_info, x):
        print('a', x)

    @TypedServerCommand(['test', 'b'])
    def on_test(command_info, x, y):
        print('b', (x, y))


Example input/output:

.. code-block:: none

    test
    A sub-command is required:
      test a <x>
      test b <x> <y>

    test a
    Not enough arguments:
      test a <x>

    test b "Hello, world!"
    Not enough arguments:
      test b <x> <y>

    test b "Hello, world!" bla
    b ('Hello, world!', 'bla')


There is no limit on the number or depth of the sub-commands. That means you
can create an abitrary number of sub-commands and these sub-commands can have
sub-commands as well.

.. seealso:: The ``sp auth`` sub-command has a high sub-command depth.


Adding value validation
^^^^^^^^^^^^^^^^^^^^^^^

As you might have noticed in the very first example, it's quite easy to add
value validation to your command. All you need to do is adding a colon (:) and
a callable object behind your argument. When the command gets triggered, the
API will call the given object with the passed value. If any exception was
raised during this call, the validation is considered as a failure. Thus, an
error message is printed to the user. If the call has been finished
successfully, its return value will be passed to your callback.

.. code-block:: python

    from commands.typed import TypedServerCommand

    @TypedServerCommand('test')
    def on_test(command_info, x:float):
        print('Got:', x)


Example input/output:

.. code-block:: none

    test
    Not enough arguments:
      test <x:float>

    test a
    'a' is an invalid value for 'x:float'.

    test 3
    Got: 3.0

    test 3.3
    Got: 3.3


Using custom error messages
"""""""""""""""""""""""""""

If you are not happy with the automatic error message, you can simply raise
your own :class:`commands.typed.ValidationError` exception in your validator.

.. code-block:: python

    from commands.typed import TypedServerCommand
    from commands.typed import ValidationError

    def my_float(value):
        try:
            return float(value)
        except:
            raise ValidationError('"{}" is not a floating value.'.format(value))

    @TypedServerCommand('test')
    def on_test(command_info, x:my_float):
        print('Got:', x)


Example input/output:

.. code-block:: none

    test
    Not enough arguments:
      test <x:my_float>

    test a
    'a' is not a floating value.


Creating custom validators
""""""""""""""""""""""""""

Just like we created custom error messages, you can also use the same
principle to create a validator that doesn't use the built-in types like
:class:`int` or :class:`float`.

.. code-block:: python

    from commands.typed import TypedServerCommand

    def positive_int(value):
        value = int(value)
        if value < 0:
            raise Exception

        return value

    @TypedServerCommand('test')
    def on_test(command_info, x:positive_int):
        print('Got:', x)


Example input/output:

.. code-block:: none

    test
    Not enough arguments:
      test <x:positive_int>

    test -1
    '-1' is an invalid value for 'x:positive_int'.

    test a
    'a' is an invalid value for 'x:positive_int'.

    test 0
    Got: 0


Source.Python has one custom validator built-in to allow iterating over
players easily (:func:`commands.typed.filter_str`).

.. code-block:: python

    from commands.typed import TypedServerCommand
    from commands.typed import filter_str

    @TypedServerCommand('test')
    def on_test(command_info, players:filter_str):
        for player in players:
            print('{player.name} in team {player.team}. Dead?: {player.dead}'.format(player=player))


Example input/output:

.. code-block:: none

    status
    # userid name                uniqueid
    #      2 "Xavier"            BOT
    #      3 "Shawn"             BOT
    #      4 "Rick"              BOT
    #      5 "Ian"               BOT
    #      6 "Ayuto"             [U:1:39094154]

    test all
    Xavier in team 3. Dead?: False
    Shawn in team 2. Dead?: True
    Rick in team 3. Dead?: False
    Ian in team 2. Dead?: True
    Ayuto in team 2. Dead?: False

    test human
    Ayuto in team 2. Dead?: False

    test bot
    Xavier in team 3. Dead?: False
    Shawn in team 2. Dead?: False
    Rick in team 3. Dead?: False
    Ian in team 2. Dead?: False

    test t
    Shawn in team 2. Dead?: False
    Ian in team 2. Dead?: False
    Ayuto in team 2. Dead?: False

    test ct
    Xavier in team 3. Dead?: False
    Rick in team 3. Dead?: False

    test dead
    Ayuto in team 2. Dead?: True

    test alive
    Xavier in team 3. Dead?: False
    Shawn in team 2. Dead?: False
    Rick in team 3. Dead?: False
    Ian in team 2. Dead?: False

    test 2+6
    Xavier in team 3. Dead?: False
    Ayuto in team 2. Dead?: False

    test all-ct
    Shawn in team 2. Dead?: False
    Ian in team 2. Dead?: False
    Ayuto in team 2. Dead?: False

    test bot-2-4
    Shawn in team 2. Dead?: True
    Ian in team 2. Dead?: True

    test ct+t
    Xavier in team 3. Dead?: False
    Shawn in team 2. Dead?: True
    Rick in team 3. Dead?: False
    Ian in team 2. Dead?: True
    Ayuto in team 2. Dead?: False

    test asd
    'asd' is an invalid value for 'players:filter_str'.


This custom validator is also cappable to parse complex expressions that use
parentheses and multiple plus and minus operators. The plus sign stands for
the set operation `Union` and the minus sign for `Complement`.

.. seealso:: https://en.wikipedia.org/wiki/Set_%28mathematics%29#Basic_operations


Using permissions
^^^^^^^^^^^^^^^^^

It's quite common that some commands should only be executable by specific
players. Thus, there is the ``permission`` parameter in the constructor of the
typed command decorators.

.. code-block:: python

    from commands.typed import TypedClientCommand

    @TypedClientCommand('test', 'my_plugin.test')
    def on_test(command_info):
        print('Executed!')


Example input/output:

.. code-block:: none

    test
    Unknown command: test
    You are not authorized to use this command.
    Required permission: my_plugin.test


In this case the player requires the permission ``my_plugin.test`` to execute
the command.

.. note:: The permission is checked before arguments are validated.

You can also auto-generate the permission string by setting the second parameter to ``True``.

.. code-block:: python

    from commands.typed import TypedClientCommand

    @TypedClientCommand('test', True)
    def on_test(command_info):
        print('Executed!')


Example input/output:

.. code-block:: none

    test
    Unknown command: test
    You are not authorized to use this command.
    Required permission: test


.. note::

    You should only use this feature if it generates a permission string that
    doesn't conflict with our guideline on permission names. That means you
    should only use it if you are using sub-commands. E.g. for the sub-command
    ``xy kick`` it would generate the permission string ``xy.kick``.

You can also add a callback that gets called when an unauthorized player tries
to execute the command.

.. code-block:: python

    from commands.typed import TypedClientCommand

    def on_test_failed(command_info, args):
        print('Not authorized.', args)

    @TypedClientCommand('test', 'my_plugin.test', on_test_failed)
    def on_test(command_info):
        print('Executed!')


Example input/output:

.. code-block:: none

    test
    Unknown command: test
    Not authorized. []

    test 1 2
    Unknown command: test
    Not authorized. ['1', '2']
