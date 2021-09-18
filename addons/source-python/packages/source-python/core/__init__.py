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
#   Hashlib
import hashlib
#   Inspect
from inspect import getmodule
from inspect import getmodulename
from inspect import currentframe
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
#   Paths
from paths import GAME_PATH
from paths import PLUGIN_PATH



# =============================================================================
# >> FORWARD IMPORTS
# =============================================================================
# Source.Python Imports
#   Core
from _core import BoostPythonClass
from _core import console_message
from _core import get_interface
from _core import get_core_modules
from _core import OutputReturn
from _core import SOURCE_ENGINE
from _core import SOURCE_ENGINE_BRANCH


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('AutoUnload',
           'BoostPythonClass',
           'GameConfigObj',
           'WeakAutoUnload',
           'GAME_NAME',
           'OutputReturn',
           'PLATFORM',
           'SOURCE_ENGINE',
           'SOURCE_ENGINE_BRANCH',
           'check_info_output',
           'console_message',
           'create_checksum',
           'echo_console',
           'get_core_modules',
           'get_interface',
           'get_public_ip',
           'ignore_unicode_errors',
           'server_output',
           )


# =============================================================================
# >> GLOBAL VARIABLES
# =============================================================================
# Get the specific game for the server
GAME_NAME = GAME_PATH.namebase

# Get the platform the server is on
PLATFORM = system().lower()


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

        # Get the calling frame
        frame = currentframe().f_back

        # Get the calling path
        path = frame.f_code.co_filename

        # Don't keep hostage instances that will never be unloaded
        while not path.startswith(PLUGIN_PATH):
            frame = frame.f_back
            if frame is None:
                return self
            path = frame.f_code.co_filename
            if path.startswith('<frozen'):
                return self

        # Resolve the calling module name
        try:
            name = frame.f_globals['__name__']
        except KeyError:
            try:
                name = getmodule(frame).__name__
            except AttributeError:
                name = getmodulename(path)

        # Call class-specific logic for adding the instance.
        if name is not None:
            self._add_instance(name)

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

    .. note::

        Unlike ``console_message``, this function automatically adds a newline
        at the end of the message.

    :param str text:
        Message to print to the console.
    """
    console_message(text + '\n')


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

        This functions makes a call to ``http://api.ipify.org`` to retrieve the public IP.
    """
    return urlopen('http://api.ipify.org/').read().decode()


@contextmanager
def server_output(action=OutputReturn.CONTINUE):
    """Gather all server output sent during the execution of the with-statement.

    :param OutputReturn action:
        Determines what happens with the output.
    :rtype: list
    :return:
        A list that will be filled with a tuple for every line that is being
        logged. The tuple contains the severity and the message.

    Example:

    .. code:: python

        from cvars import cvar
        from core import server_output
        from core import OutputReturn

        status = cvar.find_command('status')

        with server_output(OutputReturn.BLOCK) as output:
            status()

        # Print everything that was logged by the 'status' command
        print(output)


    Output:

    .. code:: python

        [(_core.MessageSeverity.MESSAGE, 'hostname: Counter-Strike: Global Offensive\\n'),
         (_core.MessageSeverity.MESSAGE, 'version : 1.35.8.4/13584 513/6771 secure  [A:1:2435270659:8640] \\n'),
         (_core.MessageSeverity.MESSAGE, 'udp/ip  : 192.168.178.60:27015  (public ip: 46.83.158.27)\\n'),
         (_core.MessageSeverity.MESSAGE, 'os      :  Windows\\n'),
         (_core.MessageSeverity.MESSAGE, 'type    :  community dedicated\\n'),
         (_core.MessageSeverity.MESSAGE, 'players : 0 humans, 0 bots (20/0 max) (hibernating)\\n\\n'),
         (_core.MessageSeverity.MESSAGE, '# userid name uniqueid connected ping loss state rate'),
         (_core.MessageSeverity.MESSAGE, ' adr'),
         (_core.MessageSeverity.MESSAGE, '\\n'),
         (_core.MessageSeverity.MESSAGE, '#end\\n')]
    """
    # Import this here to fix a cyclic import
    from listeners import OnServerOutput

    msg_buffer = []

    def intercepter(severity, msg):
        msg_buffer.append((severity, msg))
        return action

    OnServerOutput.manager.register_listener(intercepter)
    try:
        yield msg_buffer
    finally:
        OnServerOutput.manager.unregister_listener(intercepter)

def create_checksum(data, ignore_wchars=True):
    """Create an MD5 checksum for the given string.

    :param str data:
        The string for which a checksum should be created.
    :param bool ignore_wchars:
        If ``True`` whitespace characters are ignored.
    :rtype: str
    """
    if ignore_wchars:
        data = ''.join(data.split())

    return hashlib.new('md5', bytes(data, encoding='utf-8')).hexdigest()

def check_info_output(output):
    """Return whether the output of ``sp info`` has been modified.

    :param str output:
        The output of ``sp info``.
    :raise ValueError:
        Raised if the checksum was not found in the output.
    :return:
        ``True`` if the output has been modified.
    :rtype: bool
    """
    checksum = None
    lines = output.strip().split('\n')

    # Search the checksum entry
    while lines:
        line = lines.pop(0)
        if line.startswith('Checksum'):
            checksum = line.split(':', 1)[1].strip()
            break

    if checksum is None:
        raise ValueError('Checksum not found.')

    # Ignore last line if it's the separator
    if lines[-1].startswith('-'):
        lines.pop()

    return create_checksum(''.join(lines)) != checksum