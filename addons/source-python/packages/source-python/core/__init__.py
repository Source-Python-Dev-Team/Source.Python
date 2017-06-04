# ../core/__init__.py

"""Provides core functionality that doesn't fit into any other package."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Python Imports
#   Codecs
import codecs
#   Collections
from collections import defaultdict
#   Contextlib
from contextlib import contextmanager
#   Inspect
from inspect import getmodule
from inspect import stack
#   OS
from os import sep
#   Path
from path import Path
#   Platform
from platform import system
#   Sys
import sys
#   Urllib
from urllib.request import urlopen
#   Weakref
from weakref import WeakValueDictionary

# Site-Packages Imports
#   ConfigObj
from configobj import ConfigObj

# Source.Python Imports
#   Loggers
from loggers import _sp_logger
#   Paths
from paths import GAME_PATH



# =============================================================================
# >> FORWARD IMPORTS
# =============================================================================
# Source.Python Imports
#   Core
from _core import console_message
from _core import get_interface
from _core import SOURCE_ENGINE
from _core import SOURCE_ENGINE_BRANCH


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('AutoUnload',
           'GameConfigObj',
           'WeakAutoUnload',
           'GAME_NAME',
           'PLATFORM',
           'SOURCE_ENGINE',
           'SOURCE_ENGINE_BRANCH',
           'console_message',
           'echo_console',
           'get_interface',
           'get_public_ip',
           'ignore_unicode_errors',
           )


# =============================================================================
# >> GLOBAL VARIABLES
# =============================================================================
# Get the specific game for the server
GAME_NAME = GAME_PATH.namebase

# Get the platform the server is on
PLATFORM = system().lower()

# Get the sp.core logger
core_logger = _sp_logger.core


# =============================================================================
# >> CLASSES
# =============================================================================
class AutoUnload(object):
    """Class used to auto unload specific instances.

    Each inheriting class must implement an _unload_instance method.
    """

    # Create a dictionary to store AutoUnload objects in
    _module_instances = defaultdict(list)

    def __new__(cls, *args, **kwargs):
        """Overwrite __new__ to store the calling module."""
        # Get the class instance
        self = super().__new__(cls)

        # Get the calling module
        caller = getmodule(stack()[1][0])

        # Call class-specific logic for adding the instance.
        self._add_instance(caller.__name__)

        # Return the instance
        return self

    def _add_instance(self, caller):
        """Add the instance to self._module_instances."""
        self._module_instances[caller].append(self)

    def _unload_instance(self):
        """Base _unload_instance implementation."""
        raise NotImplementedError(
            'Class "{0}" from file "{1}" does not '.format(
                self.__class__.__name__, sys.modules[
                    self.__class__.__module__].__file__.split(
                    'plugins', 1)[1][1:]) +
            'have its own implementation of an _unload_instance method.')


class WeakAutoUnload(AutoUnload):
    """Subclass of AutoUnload used to store weak references to instances."""

    # Create a dictionary to store WeakAutoUnload objects in
    _module_instances = defaultdict(WeakValueDictionary)

    def _add_instance(self, caller):
        """Add the instance to self._module_instances."""
        self._module_instances[caller][id(self)] = self


class GameConfigObj(ConfigObj):
    """Class used to parse specific game data."""

    def __init__(self, infile, *args, **kwargs):
        """Helper class that merges the given file with engine/game files."""
        # Get the file directory/name...
        path, name = Path(infile).splitpath()

        # Call ConfigObj's __init__ method...
        super().__init__(infile, *args, **kwargs)

        # Move the path to the current engine sub-directory...
        path = path / SOURCE_ENGINE

        # Parse and merge the specific engine file...
        self.merge(ConfigObj(path / name, *args, **kwargs))

        # Finally, parse the specific game file...
        self.merge(ConfigObj(path / GAME_NAME / name, *args, **kwargs))


# =============================================================================
# >> FUNCTIONS
# =============================================================================
def echo_console(text):
    """Echo a message to the server's console.

    :param str text:
        Message to print to the console.
    """
    for line in text.split('\n'):
        console_message(line + '\n')

@contextmanager
def ignore_unicode_errors(errors='ignore'):
    """Overwrite the ``strict`` codecs error handler temporarily.

    This is useful e.g. if the engine truncates a string, which results in a
    string that contains a splitted multi-byte character at the end of the
    string.

    :param str errors:
        Error handler that will be looked up via :func:`codecs.lookup_error`.
    :raise LookupError:
        Raised if the error handler was not found.

    Example:

    .. code:: python

        import memory

        # Allocate four bytes to create an erroneous string
        ptr = memory.alloc(4)

        # Write data to the memory that will usually result in a
        # UnicodeDecodeError
        ptr.set_uchar(ord('a'), 0)
        ptr.set_uchar(ord('b'), 1)
        ptr.set_uchar(226, 2) # Add the invalid byte
        ptr.set_uchar(0, 3) # Indicate the end of the string

        with ignore_unicode_errors():
            # Read the data as a string. Now, it will only print 'ab', because
            # the invalid byte has been removed/ignored.
            print(ptr.get_string_array())
    """
    old_handler = codecs.lookup_error('strict')
    codecs.register_error('strict', codecs.lookup_error(errors))
    try:
        yield
    finally:
        codecs.register_error('strict', old_handler)


def get_public_ip():
    """Return the server's public IPv4.

    :rtype: str

    .. note::

        This functions makes a call to http://ip.42.pl/raw to retrieve the public IP.
    """
    return urlopen('http://ip.42.pl/raw').read().decode()
