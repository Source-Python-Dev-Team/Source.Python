# ../core/__init__.py

"""Provides core functionality that doesn't fit into any other package."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Python Imports
#   Collections
from collections import defaultdict
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
#   Weakref
import weakref

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
from _core import SOURCE_ENGINE
from _core import SOURCE_ENGINE_BRANCH


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('AutoUnload',
           'GAME_NAME',
           'PLATFORM',
           'SOURCE_ENGINE',
           'SOURCE_ENGINE_BRANCH',
           'echo_console',
           )


# =============================================================================
# >> GLOBAL VARIABLES
# =============================================================================
# Get the specific game for the server
GAME_NAME = GAME_PATH.rsplit(sep, 2)[1]

# Get the platform the server is on
PLATFORM = system().lower()

# Get the sp.core logger
core_logger = _sp_logger.core

# Create a dictionary to store AutoUnload object in
_module_instances = defaultdict(list)


# =============================================================================
# >> CLASSES
# =============================================================================
class AutoUnload(object):
    """Class used to auto unload specific instances.

    Each inheriting class must implement an _unload_instance method.
    """

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
        """Add the instance to _module_instances."""
        _module_instances[caller].append(self)

    def _unload_instance(self):
        """Base _unload_instance implementation."""
        raise NotImplementedError(
            'Class "{0}" from file "{1}" does not '.format(
                self.__class__.__name__, sys.modules[
                    self.__class__.__module__].__file__.split(
                    'plugins', 1)[1][1:]) +
            'have its own implementation of an _unload_instance method.')


class WeakAutoUnload(AutoUnload):
    """Subclass of AutoUnload used to unload specific instances when all
    references to the instance are deleted.
    """

    def new(cls, *args, **kwargs):
        self = super().__new__(cls)
        self.___unloaded = False

        return self

    def __del__(self):
        """Overwrite __del__ to automatically unload the instance when garbage
        collected.
        """
        self._unload_instance()

    def _add_instance(self, caller):
        """Add the instance to _module_instances."""
        # Use a weakref proxy so that the instance will get garbage collected
        # when this is the only reference left.
        _module_instances[caller].append(weakref.proxy(self))

    def _unload_instance(self):
        # Prevent unloading more than once.
        if not self.___unloaded:
            self.___unloaded = True
            super()._unload_instance(self)


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
    """Echo a message to the server's console."""
    # Import engine_server
    # This is done here to fix an ImportError
    from engines.server import engine_server

    # Loop through each line in the text
    for line in text.split('\n'):

        # Echo the message
        engine_server.server_command(
            'echo "{0}"\n'.format(line.replace('"', "'")))
