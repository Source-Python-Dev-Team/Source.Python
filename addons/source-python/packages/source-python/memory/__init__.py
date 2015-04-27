# ../memory/__init__.py

"""Provides binary/memory based functionality."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python
#   Loggers
from loggers import _sp_logger


# =============================================================================
# >> FORWARD IMPORTS
# =============================================================================
# Source.Python
#   core
from core import AutoUnload
#   memory
from _memory import BinaryFile
from _memory import Convention
from _memory import DataType
from _memory import EXPOSED_CLASSES
from _memory import Function
from _memory import NULL
from _memory import Pointer
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
__all__ = ('BinaryFile',
           'Callback',
           'Convention',
           'DataType',
           'EXPOSED_CLASSES',
           'Function',
           'NULL',
           'Pointer',
           'StackData',
           'TYPE_SIZES',
           'alloc',
           'find_binary',
           'get_object_pointer',
           'get_size',
           'make_object',
           )


# =============================================================================
# >> GLOBAL VARIABLES
# =============================================================================
# Get the sp.memory logger
memory_logger = _sp_logger.memory


# =============================================================================
# >> CLASSES
# =============================================================================
class Callback(AutoUnload):

    """Create a function in memory that calls a Python callback."""

    def __init__(self, convention, arg_types, return_type):
        """Initialize the Callback object.

        @param <convention>:
        Defines the calling convention of the function.

        @param <arg_types>:
        Defines the argument types of the function.

        @param <return_type>:
        Defines the return type of the function.
        """
        self.callback = None

        # Allocate enough space for a jump, so we can hook it later. Then
        # convert it to a function. Of course, this isn't a function, but the
        # hook will override it.
        self.function = alloc(8, False).make_function(convention, arg_types,
            return_type)

        # A little hack to access the "self" argument
        def hook(args):
            return_value = self.callback(args)
            if return_value is not None:
                return return_value

            if return_type == DataType.VOID:
                return 0

            # We will crash now :(
            raise ValueError('Return value is not allowed to be None.')

        # Hook the function and make sure the callback doesn't go out of scope
        self._hook = self.function.add_pre_hook(hook)

    def __call__(self, callback):
        """Store the given callback."""
        assert callable(callback)
        self.callback = callback
        return self

    def _unload_instance(self):
        """Remove the hook, restore the allocated space and deallocate it."""
        # TODO: Remove the hook and restore the allocated space
        self.function.dealloc()
