# ../memory/helpers.py

"""Provides helper classes/functions for memory functionality."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Python
#   Binascii
import binascii

# Source.Python
#   Core
from core import PLATFORM
#   Memory
from memory import Convention
from memory import DataType
from memory import Function
from memory import Pointer
from memory import TYPE_SIZES
from memory import make_object


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('Array',
           'BasePointer',
           'Key',
           'MemberFunction',
           'NO_DEFAULT',
           'Type',
           'parse_data',
           )


# =============================================================================
# >> Type
# =============================================================================
class Type(object):
    """Stores attribute/array types."""

    BOOL = 'bool'
    CHAR = 'char'
    UCHAR = 'uchar'
    SHORT = 'short'
    USHORT = 'ushort'
    INT = 'int'
    UINT = 'uint'
    LONG = 'long'
    ULONG = 'ulong'
    LONG_LONG = 'long_long'
    ULONG_LONG = 'ulong_long'
    FLOAT = 'float'
    DOUBLE = 'double'
    POINTER = 'pointer'
    STRING_POINTER = 'string_pointer'
    STRING_ARRAY = 'string_array'

    @staticmethod
    def is_native(type_name):
        """Return True if the given type name is a native type."""
        return hasattr(Type, type_name.upper())


# =============================================================================
# >> Key
# =============================================================================
class Key(object):
    """Holds some constants and provides converters for parse_data()."""

    # General type information keys
    BINARY = 'binary'
    SRV_CHECK = 'srv_check'
    SIZE = 'size'

    OFFSET = 'offset'

    # Attribute/array keys
    TYPE_NAME = 'type'

    # Array keys
    LENGTH = 'length'

    # Pointer keys
    LEVEL = 'level'

    # (Virtual) function keys
    ARGS = 'arguments'
    RETURN_TYPE = 'return_type'
    CONVENTION = 'convention'
    IDENTIFIER = 'identifier'

    # Shared keys
    DOC = 'doc'

    @staticmethod
    def as_bool(manager, value):
        """Convert a string to a boolean.

        Raises a ValueError if the string doesn't represent such a value.
        """
        value = value.lower()
        if value == 'true':
            return True

        if value == 'false':
            return False

        raise ValueError(
            'Cannot convert "{0}" to a boolean value.'.format(value))

    @staticmethod
    def as_args_tuple(manager, value):
        """Convert a string into a tuple containing <DataType> elements."""
        if isinstance(value, str):
            return (DataType.names[value], )

        return tuple(DataType.names[item] for item in value)

    @staticmethod
    def as_return_type(manager, value):
        """Convert a string into a <Return> object.

        If the conversion fails, the string itself will be returned.
        """
        return DataType.names.get(value, value)

    @staticmethod
    def as_identifier(manager, value):
        """Convert a string into a byte string.

        If no spaces in the string, the string itself will be returned.
        """
        if ' ' in value:
            return binascii.unhexlify(value.replace(' ', ''))

        return value

    @staticmethod
    def as_convention(manager, value):
        """Convert a string into a <Convention> object."""
        try:
            return Convention.names[value]
        except KeyError:
            return manager.custom_conventions[value]

    @staticmethod
    def as_attribute_type(manager, value):
        """Convert a string into a <Type> value."""
        if Type.is_native(value):
            return getattr(Type, value)

        return value

    @staticmethod
    def as_str(manager, value):
        """Convert the value to a string."""
        return str(value)

    @staticmethod
    def as_int(manager, value):
        """Convert the value to an integer."""
        return int(value)


# =============================================================================
# >> BasePointer
# =============================================================================
class BasePointer(Pointer):
    """Pointer extension class."""

    # These four operator functions are required. Otherwise we would downcast
    # the instance to the Pointer class if we add or subtract bytes.
    # TODO: Can we do that on the C++ side?
    # If yes, this class would be redundant.
    def __add__(self, other):
        """Return self+value."""
        return make_object(self.__class__, Pointer(int(self) + int(other)))

    def __radd__(self, other):
        """Return value+self."""
        return self + other

    def __sub__(self, other):
        """Return self-value."""
        return make_object(self.__class__, Pointer(int(self) - int(other)))

    def __rsub__(self, other):
        """Return value-self."""
        return self - other


# =============================================================================
# >> Array
# =============================================================================
class Array(BasePointer):
    """Wrap an array."""

    def __init__(self, manager, is_ptr, type_name, ptr, length=None):
        """Initialize the array wrapper.

        :param TypeManager manager: The manager that should be used to
            retrieve classes.
        :param bool is_ptr: Set to True if the array contains pointers.
        :param str type_name: The name of the array type. E.g. 'Vector' or
            'bool'.
        :param Pointer ptr: The base address of the array (the very first
            array entry).
        :param int|None length: Length of the array. Setting this value allows
            you to iterate over the array.
        """
        self._manager = manager

        # Set to True if the array contains pointers, else False
        self._is_ptr = is_ptr

        # Contains the type name of the array
        self._type_name = type_name

        # Optional -- specifies the length of the array
        self._length = length

        super().__init__(ptr)

    def __getitem__(self, index):
        """Return the value at the given index."""
        return self._make_attribute(index).__get__(self)

    def __setitem__(self, index, value):
        """Set the value at the given index."""
        self._make_attribute(index).__set__(self, value)

    def __iter__(self):
        """Return a generator that can iterate over the array."""
        # This prevents users from iterating over the array without having
        # _length specified. Otherwise the server would hang or crash.
        if self._length is None:
            raise ValueError(
                'Cannot iterate over the array without _length being specif' +
                'ied.')

        for index in range(self._length):
            yield self[index]

    def _make_attribute(self, index):
        """Validate the index and returns a new property object."""
        # Validate the index, so we don't access invalid memory addresses
        if self._length is not None and index >= self._length:
            raise IndexError('Index out of range')

        # Construct the proper function name
        name = ('pointer' if self._is_ptr else 'instance') + '_attribute'

        # Get the function and call it
        return getattr(self._manager, name)(
            self._type_name,
            self.get_offset(index)
        )

    def get_offset(self, index):
        """Return the offset of the given index."""
        # Pointer arrays always have every 4 bytes a new pointer
        if self._is_ptr:
            return index * TYPE_SIZES['POINTER']

        # Every 1, 2, 4 or 8 bytes is a new value
        if Type.is_native(self._type_name):
            return index * TYPE_SIZES[self._type_name.upper()]

        # Get the class of the custom type
        cls = self._manager.get_class(self._type_name)

        if cls is None:
            raise NameError('Unknown class "{0}".'.format(self._type_name))

        # To access a value, we require the proper size of a custom type
        if cls._size is None:
            raise ValueError('Array requires a size to access its values.')

        # Every x bytes is a new instance
        return index * cls._size

    # Arrays have another constructor and we don't want to downcast. So, we
    # have to implement these operators here again.
    def __add__(self, other):
        """Add bytes or another pointer to the base address."""
        return self.__class__(
            self._manager,
            self._is_ptr,
            self._type_name,
            int(self) + int(other),
            self._length
        )

    def __sub__(self, other):
        """Subtract bytes or another pointer from the base address."""
        return self.__class__(
            self._manager,
            self._is_ptr,
            self._type_name,
            int(self) - int(other),
            self._length
        )


# =============================================================================
# >> MemberFunction
# =============================================================================
class MemberFunction(Function):
    """Use this class to create a wrapper for member functions.

    It passes the this pointer automatically to the wrapped function.
    """

    def __init__(self, manager, return_type, func, this):
        """Initialize the instance."""
        self._function = func
        super().__init__(
            func.address, func.convention, func.arguments, func.return_type
        )

        # This should always hold a TypeManager instance
        self._manager = manager

        # Holds the this pointer
        self._this = this

        # Holds the return type name
        self._type_name = return_type

    def __call__(self, *args):
        """Call the function dynamically."""
        return super().__call__(self._this, *args)

    def call_trampoline(self, *args):
        """Call the trampoline dynamically."""
        return super().call_trampoline(self._this, *args)

    def skip_hooks(self, *args):
        """Call the function, but skip hooks if there are any."""
        return super().skip_hooks(self._this, *args)


# =============================================================================
# >> FUNCTIONS
# =============================================================================
def parse_data(manager, raw_data, keys):
    """Parse the data dictionary.

    Parses by converting the values of the given keys into
    the proper type or assigning them default values. Raises a KeyError if a
    key does not exist and if no default value is available.

    Returns a generator: (<name>, [<value of key0>, <value of key1>, ...])

    <keys> must have the following structure:
    ((<key name>, <converter>, <default value>), ...)
    
    The convert function must accept 2 arguments:
    
    1. An instance of the TypeManager class
    2. The value to convert


    Information about data that comes from a file:

    You can specialize every key by adding ''_windows'' (for Windows) or
    ''_linux'' (for Linux) to the end a key.

    For example:
    If you are using a signature on Windows, but a symbol on Linux, you have
    three possibilities to do that:

    1.
    identifier_windows = <signature for Windows>
    identifier         = <symbol for Linux>

    2.
    identifier         = <signature for Windows>
    identifier_linux   = <symbol for Linux>

    3.
    identifier_windows = <signature for Windows>
    identifier_linux   = <symbol for Linux>
    """
    for name, data in raw_data.items():
        temp_data = []
        for key, converter, default in keys:
            # Get the OS specific key. If that fails, fall back to the shared
            # key. If that fails too, use the default value
            value = data.get(key + '_' + PLATFORM, data.get(key, default))

            # If the value is NO_DEFAULT, the key is obviously missing
            if value is NO_DEFAULT:
                raise KeyError(
                    'Missing information for key "{0}".'.format(key))

            temp_data.append(
                value if value is default else converter(manager, value))

        yield (name, temp_data)

# Use this as a default value if the key is not allowed to have a default
# value
NO_DEFAULT = object()
