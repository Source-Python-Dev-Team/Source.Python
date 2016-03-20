Coding conventions
==================

It's very important that everyone who works in a project follows the same
coding conventions. Thus, this is an outline of the coding style that is used
in Source.Python. We are trying to follow `PEP8 <https://www.python.org/dev/peps/pep-0008/>`_
and `PEP257 <https://www.python.org/dev/peps/pep-0257/>`_ as close as possible.


Naming conventions
------------------

1. Class names should use CamelCase.

  .. code-block:: python

    class TestClass(object):
        pass

2. No underscores in class names except for a leading underscore to as "private".

  .. code-block:: python

    class _PrivateTestClass(object):
        pass

3. Constants must be ALL_CAPS with an underscore separating the words.

  .. code-block:: python

    CONSTANT_VALUE = True

4. Class attributes/properties/methods should be lower_case_with_underscores.

  .. code-block:: python

    class TestClass(object):
        def __init__(self, value1, value2):
            self.value1 = value1
            self._value2 = value2

        @property
        def value2(self):
            return self._value2

        def some_method(self):
            return self.value1 + self.value2

5. Singleton objects or "the instance" objects (objects that should be the one and only instance of a class) should also use lower_case_with_underscores.

  .. code-block:: python

    class TestClass(object):
        pass

    test_class = TestClass()

6. All global variable objects (which are not constants) should also use lower_case_with_underscores.

  .. code-block:: python

    start_value = True

7. Any global variables created in the scope which are not to be imported by other modules should use a leading underscore.

  .. code-block:: python

    _private_variable = False

8. Only built-in magic methods should have 2 leading and trailing underscores.
9. **All** names should be very descriptive as to what the object does or is.


Formatting
----------

1. Lines must be no longer than 80 characters (that includes the built-in \\n, making it 79 characters of code).
2. **Must** use 4 space indentation, no TABs.
3. **Must** have 2 blank lines prior to functions and classes.

  * If the function/class is starting a new <Section> (explained below), the blank lines need to be prior to the section separator.

4. All other lines should only have 1 blank line between.

  * An exception that is sometimes used is 2 blank lines prior to a <Section> change (explained below)

5. No extra whitespace at the end of a line.

  * This also means that any "blank" lines should have absolutely nothing, not even spaces, in them.

6. Exactly 1 space between arguments (after the comma) for all functions/classes/methods, including when calling them.

  * No leading space before first argument or trailing space after last argument.

7. Exactly one space after a colon (:) when setting an item in a dictionary.
8. There must be exactly one blank line at the end of the file.


Documentation
-------------

1. All classes/methods/properties/functions must be followed by a comment using a triple quote docstring (using quotes and not apostrophes).
2. Most lines of code should follow a one line comment (using the # character).

  * Some exceptions apply, like comments that suffice for multiple lines.

3. Files should always start with this line.

  .. code-block:: python

    # ../<path from source-python directory>

4. Different sections of code should be separated by the following lines (where <Section> should describe what is in the following section).

  .. code-block:: python

    # =============================================================================
    # >> <Section>
    # =============================================================================

  * Some examples of what <Section> types there are to be used:

    * IMPORTS
    * GLOBAL VARIABLES
    * CLASSES
    * FUNCTIONS


Import comments
---------------

1. Separate base Python imports (including site-packages) from Source.Python imports with 1 blank line.
2. Use a comment line with 1 space after the # showing whether the current section is from Python or Source.Python.
3. Python imports should always go first, then Source.Python imports.
4. Separate each module by using a comment about which module is being imported (with 3 spaces after the # before the modules name).
5. Modules should be listed in alphabetic order.

Example:

.. code-block:: python

    # =============================================================================
    # >> IMPORTS
    # =============================================================================
    # Python Imports
    #   Configobj
    from configobj import ConfigObj
    #   OS
    from os.path import dirname
    from os.path import join
    #   Traceback
    from traceback import format_exception

    # Source.Python Imports
    #   Core
    from core.version import is_newer_version_available
    from core.version import VERSION
    #   Events
    from events import Event
    #   Paths
    from paths import GAME_PATH


Do's and don'ts of importing
----------------------------

1. Never import "all" from a module.

  .. code-block:: python

    # Bad
    from something import *

    # Good
    from something import one_object
    from something import second_object

2. For "most" imports, import each object individually, and on a separate line.

  .. code-block:: python

    # Bad
    from os.path import dirname, join, curdir

    # Good
    from os.path import dirname
    from os.path import join
    from os.path import curdir

    # "Ok", but use only when necessary
    import os.path
    import sys
