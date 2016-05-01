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
           'CLASS_INFO',
           'Callback',
           'CallingConvention',
           'Convention',
           'DataType',
           'EXPOSED_CLASSES',
           'Function',
           'FunctionInfo',
           'NULL',
           'Pointer',
           'ProcessorRegister',
           'Register',
           'Registers',
           'StackData',
           'TYPE_SIZES',
           'alloc',
           'find_binary',
           'get_class',
           'get_class_info',
           'get_class_name',
           'get_data_type_size',
           'get_function_info',
           'get_object_pointer',
           'get_size',
           'get_virtual_function',
           'make_object'
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
    """Decorator to create a function in memory to call a Python callback."""

    def __init__(self, convention, arg_types, return_type):
        """Initialize the Callback object.

        :param Convention|CallingConvention convention: Calling convention
            that should be used for this callback.
        :param iterable arg_types: Argument types of the callback.
        :param return_type: Return type of the callback.
        """
        self.callback = None

        # Allocate enough space for a jump, so we can hook it later. Then
        # convert it to a function. Of course, this isn't a function, but the
        # hook will override it.
        super().__init__(
            alloc(8, False).address, convention, arg_types, return_type)

        self.add_pre_hook(self._hook)

    def _hook(self, args):
        """Call the callback and get the return value."""
        return_value = self.callback(args)
        if return_value is not None:
            return return_value

        if self.return_type == DataType.VOID:
            return 0

        # We will crash now :(
        raise ValueError('Return value is not allowed to be None.')

    def __call__(self, *args, **kw):
        """Store the given callback on the first call.

        All further calls will call the created callback function.
        """
        if self.callback is None:
            assert callable(args[0])
            self.callback = args[0]
            return self

        return super().__call__(*args, **kw)

    def _unload_instance(self):
        """Remove the hook, restore the allocated space and deallocate it."""
        self._delete_hook()
        self.dealloc()


# =============================================================================
# >> FUNCTIONS
# =============================================================================
def get_virtual_function(obj, function_name, function_index=0):
    """Return a :class:`Function` object.

    Create the :class:`Function` object by using
    a :class:`FunctionInfo` object.

    :param obj: An object of an exposed class.
    :param str function_name: See :func:`get_function_info`.
    :param int function_index: See :func:`get_function_info`.
    :raise ValueError: See :func:`get_class_name`.
    """
    return get_object_pointer(obj).make_virtual_function(
        get_function_info(obj, function_name, function_index))


def get_function_info(cls, function_name, function_index=0):
    """Return the :class:`FunctionInfo` object of a member function.

    :param str cls: See :func:`get_class_info`.
    :param str function_name: The name of the member function on the C++ side.
    :param int function_index: The index of the member function in the
        function info list. This is only required if the function is
        overloaded and you want to get a different FunctionInfo object than
        the first one.
    :raise ValueError: See :func:`get_class_name`.
    """
    return get_class_info(cls)[function_name][function_index]


def get_class_info(cls):
    """Return the class info dictionary of a class.

    :param str cls:  A string that defines the name of the class on the C++
        side or an exposed class or an object of an exposed class.
    :raise ValueError: See :func:`get_class_name`.
    """
    if isinstance(cls, str):
        return CLASS_INFO[cls]

    if not inspect.isclass(cls):
        cls = cls.__class__

    return get_class_info(get_class_name(cls))


def get_class_name(cls):
    """Return the name of a class or class object on the C++ side.

    :param cls: A class or class object.
    :raise ValueError: Raised if the class was not exposed by Source.Python.
    """
    if not inspect.isclass(cls):
        cls = cls.__class__

    for name, possible_cls in EXPOSED_CLASSES.items():
        if cls is possible_cls:
            return name

    for base_class in cls.__bases__:
        try:
            class_name = get_class_name(base_class)
        except ValueError:
            continue
        else:
            return class_name

    raise ValueError('Given class was not exposed.')


def get_class(classname):
    """Return the class of an exposed class by its C++ class name.

    :param str classname: The name of the exposed class on the C++ side.
    :raise KeyError: Raised if the `classname` is not the name of an exposed
        class.
    """
    return EXPOSED_CLASSES[classname]
