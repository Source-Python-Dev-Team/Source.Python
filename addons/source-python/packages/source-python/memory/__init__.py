# ../memory/__init__.py

"""Provides binary/memory based functionality."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python
#   Loggers
from loggers import _SPLogger


# =============================================================================
# >> FORWARD IMPORTS
# =============================================================================
# Source.Python
#   memory
from _memory import Argument
from _memory import BinaryFile
from _memory import Callback
from _memory import Convention
from _memory import EXPOSED_CLASSES
from _memory import Function
from _memory import NULL
from _memory import Pointer
from _memory import Return
from _memory import StackData
from _memory import TYPE_SIZES
from _memory import alloc
from _memory import find_binary
from _memory import get_object_pointer
from _memory import get_size
from _memory import make_object


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('Argument',
           'BinaryFile',
           'Callback',
           'Convention',
           'EXPOSED_CLASSES',
           'Function',
           'NULL',
           'Pointer',
           'Return',
           'StackData',
           'TYPE_SIZES',
           'alloc',
           'callback',
           'find_binary',
           'get_object_pointer',
           'get_size',
           'make_object',
           )


# =============================================================================
# >> GLOBAL VARIABLES
# =============================================================================
# Get the sp.memory logger
MemoryLogger = _SPLogger.memory


# =============================================================================
# >> FUNCTIONS
# =============================================================================
def callback(convention=Convention.CDECL, args=(), return_type=Return.VOID):
    """Create a C++ callback that calls back to the decorated function.

    EXAMPLE:

    @callback(Convention.CDECL, (Argument.INT, Argument.INT), Return.INT)
    def add(x, y, ebp):
        return x + y

    <add> is now a Callback instance, but you can still call it like a normal
    Python function:

    assert add(4, 6) == 10
    """
    def wait_for_func(func):
        """Return a memory callback instance."""
        return Callback(func, convention, args, return_type)

    return wait_for_func
