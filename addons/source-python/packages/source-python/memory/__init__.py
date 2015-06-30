# ../memory/__init__.py

"""Provides binary/memory based functionality."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Python
import inspect

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
from _memory import CallingConvention
from _memory import CLASS_INFO
from _memory import Convention
from _memory import DataType
from _memory import EXPOSED_CLASSES
from _memory import Function
from _memory import FunctionInfo
from _memory import NULL
from _memory import Pointer
from _memory import ProcessorRegister
from _memory import Register
from _memory import Registers
from _memory import StackData
from _memory import TYPE_SIZES
from _memory import alloc
from _memory import find_binary
from _memory import get_data_type_size
from _memory import get_object_pointer
from _memory import get_size
from _memory import make_object


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('BinaryFile',
           'Callback',
           'CallingConvention',
           'CLASS_INFO',
           'Convention',
           'DataType',
           'EXPOSED_CLASSES',
           'Function',
           'FunctionInfo',
           'get_class',
           'get_class_info',
           'get_class_name',
           'NULL',
           'Pointer',
           'ProcessorRegister',
           'Register',
           'Registers',
           'StackData',
           'TYPE_SIZES',
           'alloc',
           'find_binary',
           'get_data_type_size',
           'get_function_info',
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
class Callback(AutoUnload, Function):

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
        super(Callback, self).__init__(
            alloc(8, False).address, convention, arg_types, return_type)

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
        self._hook = self.add_pre_hook(hook)

    def __call__(self, *args, **kw):
        """Store the given callback."""
        # The first call of this function will fully initialize the callback.
        # All further calls should call the memory function.
        if self.callback is None:
            assert callable(args[0])
            self.callback = args[0]
            return self

        return super(Callback, self).__call__(*args, **kw)

    def _unload_instance(self):
        """Remove the hook, restore the allocated space and deallocate it."""
        self._delete_hook()
        self.dealloc()

def get_function_info(classname, function_name, function_index=0):
    """Return the FunctionInfo object of a member function.

    @param <classname>:
    The name of the class on the C++ side.

    @param <function_name>:
    The name of the member function on the C++ side.

    @param <function_index>:
    The index of the member function in the function info list. This is only
    required if the function is overloaded and you want to get a different
    FunctionInfo object than the first one.
    """
    return get_class_info(classname)[function_name][function_index]

def get_class_info(cls):
    """Return the class info dictionary of a class.

    @param <cls>:
    A string that defines the name of the class on the C++ side or an exposed
    class or an object of an exposed class.
    """
    if isinstance(cls, str):
        return CLASS_INFO[cls]

    if not inspect.isclass(cls):
        cls = cls.__class__

    return get_class_info(get_class_name(cls))

def get_class_name(cls):
    """Return the name of the class on the C++ side.

    A ValueError is raised if the class was not exposed by Source.Python.
    """
    for name, possible_cls in EXPOSED_CLASSES.items():
        if cls is possible_cls:
            return name

    raise ValueError('Given class was not exposed.')

def get_class(classname):
    """Return the class of an exposed class.

    @param <classname>:
    The name of the exposed class on the C++ side.
    """
    return EXPOSED_CLASSES[classname]
