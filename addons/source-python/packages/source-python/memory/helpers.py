# =============================================================================
# >> IMPORTS
# =============================================================================
# Python
import binascii
import os

# Source.Python
from memory_c import Pointer


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = [
    'BasePointer',
    'Key',
    'parse_data',
    'NO_DEFAULT'
]


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
# >> Key
# =============================================================================
class Key:
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

        return tuple(
            Argument.values[item] for item in
            value.replace(' ', '').split(','))

    @staticmethod
    def as_return_type(value):
        '''
        Converts a string into a <Return> object. If the conversion fails, the
        string itself will be returned.
        '''

        return Return.values.get(value, value)

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
