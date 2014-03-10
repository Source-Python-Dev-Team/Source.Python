# =============================================================================
# >> IMPORTS
# =============================================================================
# Python
import binascii
import os

# Source.Python
from memory_c import *


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = [
    'Type',
    'Key',
    'BasePointer',
    'Array',
    'MemberFunction',
    'parse_data',
    'NO_DEFAULT'
]


# =============================================================================
# >> Type
# =============================================================================
class Type:
    '''
    This class is used to specify the type of an attribute or array created by
    a TypeManager instance.
    '''

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
        '''
        Returns True if the given type name is a native type.
        '''

        return hasattr(Type, type_name.upper())


# =============================================================================
# >> Key
# =============================================================================
class Key:
    '''
    This class holds some constants and provides converters for parse_data().
    '''

    # General type information keys
    BINARY = 'binary'
    SRV_CHECK = 'srv_check'
    SIZE = 'size'

    # Attribute/array kes
    OFFSET = 'offset'
    TYPE_NAME = 'type'

    # Array keys
    LENGTH = 'length'

    # Virtual function keys
    INDEX = 'index'

    # (Virtual) function keys
    ARGS = 'arguments'
    RETURN_TYPE = 'return_type'
    CONVENTION = 'convention'
    IDENTIFIER = 'identifier'

    # Shared keys
    DOC = 'doc'

    @staticmethod
    def as_bool(value):
        '''
        Converts a string that represents a boolean value into a boolean
        value. Raises a ValueError if the string doesn't represent such a
        value.
        '''

        value = value.lower()
        if value == 'true':
            return True

        if value == 'false':
            return False

        raise ValueError(
            'Cannot convert "{0}" to a boolean value.'.format(value))

    @staticmethod
    def as_args_tuple(value):
        '''
        Converts a string into a tuple containing <Argument> elements.
        '''

        if isinstance(value, str):
            return (value,)

        return tuple(Argument.names[item] for item in value)

    @staticmethod
    def as_return_type(value):
        '''
        Converts a string into a <Return> object. If the conversion fails, the
        string itself will be returned.
        '''

        return Return.names.get(value, value)

    @staticmethod
    def as_identifier(value):
        '''
        If the given string contains spaces, this function will try to convert
        it into a byte string. Otherwise the string itself will be returned.
        '''

        if ' ' in value:
            return binascii.unhexlify(value.replace(' ', ''))

        return value

    @staticmethod
    def as_convention(value):
        '''
        Converts a string into a <Convention> object.
        '''

        return Convention.values[value]

    @staticmethod
    def as_attribute_type(value):
        '''
        Converts a string into a <Type> value.
        '''

        if Type.is_native(value):
            return getattr(Type, value)

        return value


# =============================================================================
# >> BasePointer
# =============================================================================
class BasePointer(Pointer):
    # These four operator functions are required. Otherwise we would downcast
    # the instance to the Pointer class if we add or subtract bytes.
    # TODO: Can we do that on the C++ side?
    # If yes, this class would be redundant.
    def __add__(self, other):
        return self.__class__(int(self) + int(other))

    def __radd__(self, other):
        return self + other

    def __sub__(self, other):
        return self.__class__(int(self) - int(other))

    def __rsub__(self, other):
        return self - other


# =============================================================================
# >> Array
# =============================================================================
class Array(BasePointer):
    '''
    This class is used to wrap an array.
    '''

    def __init__(self, manager, is_ptr, type_name, ptr, length=None):
        '''
        Initializes the array wrapper.

        @param <manager>
        Contains an instance of TypeManager.

        @param <is_ptr>
        Set to True if the array contains pointers, else False.

        @param <type_name>
        Contains the name of the array type. E.g. 'Vector' or 'bool'.

        @param <ptr>
        Contains the base address of the array (the very first array entry).

        @param <length>
        Contains the length of the array. Setting this value allows you to
        iterate over the array.
        '''

        self._manager = manager

        # Set to True if the array contains pointers, else False
        self._is_ptr = is_ptr

        # Contains the type name of the array
        self._type_name = type_name

        # Optional -- specifies the length of the array
        self._length = length

        super(Array, self).__init__(ptr)

    def __getitem__(self, index):
        '''
        Returns the value at the given index.
        '''

        return self.__make_attribute(index).__get__(self)

    def __setitem__(self, index, value):
        '''
        Sets the value at the given index.
        '''

        self.__make_attribute(index).__set__(self, value)

    def __iter__(self):
        '''
        Returns a generator that can iterate over the array.
        '''

        # This prevents users from iterating over the array without having
        # _length specified. Otherwise the server would hang or crash.
        if self._length is None:
            raise ValueError(
                'Cannot iterate over the array without _length being specif' +
                'ied.')

        for index in range(self._length):
            yield self[index]

    def __make_attribute(self, index):
        '''
        Validates the index and returns a new property object.
        '''

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
        '''
        Returns the offset of the given index.
        '''

        # Pointer arrays always have every 4 bytes a new pointer
        if self._is_ptr:
            return index * TYPE_SIZES[Type.POINTER]

        # Every 1, 2, 4 or 8 bytes is a new value
        if Type.is_native(self._type_name):
            return index * TYPE_SIZES[self._type_name.upper()]

        # Get the class of the custom type
        cls = self._manager.get_class(self._type_name)

        # To access a value, we require the proper size of a custom type
        if cls._size is None:
            raise ValueError('Array requires a size to access its values.')

        # Every x bytes is a new instance
        return index * cls._size

    # Arrays have another constructor and we don't want to downcast. So, we
    # have to implement these operators here again.
    def __add__(self, other):
        '''
        Adds bytes or another pointer to the base address.
        '''

        return self.__class__(
            self._manager,
            self._is_ptr,
            self._type_name,
            int(self) + int(other),
            self._length
        )

    def __sub__(self, other):
        '''
        Subtracts bytes or another pointer from the base address.
        '''

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
    '''
    Use this class to create a wrapper for member functions. It passes the
    this pointer automatically to the wrapped function.
    '''

    def __init__(self, manager, return_type, func, this):
        '''
        Initializes the instance.
        '''

        super(MemberFunction, self).__init__(func)

        # This should always hold a TypeManager instance
        self._manager = manager

        # Holds the this pointer
        self._this = this

        # Holds the return type name
        self._type_name = return_type

    def __call__(self, *args):
        '''
        Calls the function dynamically.
        '''

        return super(MemberFunction, self).__call__(self._this, *args)

    def call_trampoline(self, *args):
        '''
        Calls the trampoline dynamically.
        '''

        return super(MemberFunction, self).call_trampoline(
            self._this,
            *args
        )


# =============================================================================
# >> FUNCTIONS
# =============================================================================
def parse_data(raw_data, keys):
    '''
    Parses the data dictionary by converting the values of the given keys into
    the proper type or assigning them default values. Raises a KeyError if a
    key does not exist and if no default value is available.

    Returns a generator: (<name>, [<value of key0>, <value of key1>, ...])

    <keys> must have the following structure:
    ((<key name>, <converter>, <default value>), ...)


    Information about data that comes from a file:

    You can specialize every key by adding "_nt" (for Windows) or "_posix"
    (for Linux) to the end a key.

    For example:
    If you are using a signature on Windows, but a symbol on Linux, you have
    three possibilities to do that:

    1.
    identifier_nt    = <signature for Windows>
    identifier       = <symbol for Linux>

    2.
    identifier       = <signature for Windows>
    identifier_posix = <symbol for Linux>

    3.
    identifier_nt    = <signature for Windows>
    identifier_posix = <symbol for Linux>
    '''

    for name, data in raw_data.items():
        temp_data = []
        for key, converter, default in keys:
            # Get the OS specific key. If that fails, fall back to the shared
            # key. If that fails too, use the default value
            value = data.get(key + '_' + os.name, data.get(key, default))

            # If the value is NO_DEFAULT, the key is obviously missing
            if value is NO_DEFAULT:
                raise KeyError(
                    'Missing information for key "{0}".'.format(key))

            temp_data.append(value if value is default else converter(value))

        yield (name, temp_data)

# Use this as a default value if the key is not allowed to have a default
# value
NO_DEFAULT = object()