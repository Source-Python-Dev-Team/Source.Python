# =============================================================================
# >> IMPORTS
# =============================================================================
# Python
from configobj import ConfigObj

# Source.Python
from memory_c import *
from memory.helpers import *


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = [
    'Type',
    'Array',
    'MemberFunction',
    'CustomType',
    'TypeManager',
    'manager'
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
    POINTER = 'ptr'
    STRING_PTR = 'string_ptr'
    STRING_ARRAY = 'string_array'

    @staticmethod
    def is_native(type_name):
        '''
        Returns True if the given type name is a native type.
        '''

        return hasattr(Type, type_name.upper())


# =============================================================================
# >> Array
# =============================================================================
class Array(BasePointer):
    # Optional -- specifies the length of the array
    _length = None

    # Contains the type name of the array
    _type_name = None

    # Set to True if the array contains pointers, else False
    _is_ptr = None

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
        self._is_ptr = is_ptr
        self._type_name = type_name
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
                'Cannot iterate over the array without ' +
                '__length__ being specified.')

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
            return index * TYPE_SIZES[self._type_name]

        # Get the class of the custom type
        cls = self._manager[self._type_name]

        # To access a value, we require the proper size of a custom type
        if cls._size is None:
            raise ValueError('Array requires a size to access its values.')

        # Every x bytes is a new instance
        return index * cls._size

    # Arrays have another constructor and we don't want to downcast. So, we
    # have to implement these operators here again.
    def __add__(self, other):
        return self.__class__(
            self._manager,
            self._is_ptr,
            self._type_name,
            int(self) + int(other),
            self._length
        )

    def __sub__(self, other):
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

    # This should always hold a TypeManager instance
    _manager = None

    # Holds the this pointer
    _this = None

    # Holds the return type name
    _type_name = None

    def __init__(self, manager, return_type, func, this):
        super(MemberFunction, self).__init__(func)

        self._manager = manager
        self._this = this
        self._type_name = return_type

    def __call__(self, *args):
        '''
        Calls the function dynamically.
        '''

        result = super(MemberFunction, self).__call__(self._this, *args)

        # Wrap the result if it's a custom type
        if self._type_name not in Return.values:
            return self._manager[self._type_name](result)

        return result

    def call_trampoline(self, *args):
        '''
        Calls the trampoline dynamically.
        '''

        result = super(MemberFunction, self).call_trampoline(
            self._this,
            *args
        )

        # Wrap the result if it's a custom type
        if self._type_name not in Return.values:
            return self._manager[self._type_name](result)

        return result


# =============================================================================
# >> CustomType
# =============================================================================
class CustomType(BasePointer):
    '''
    Subclass this class if you want to create a new type. Make sure that you
    have set the metaclass attribute to a valid TypeManager instance.
    '''

    # This should always hold a TypeManager instance. It's set automatically
    # by the metaclass.
    _manager = None

    # This is required if TypeManager.function() is used. It should contain
    # the path to the binary that defines this type
    _binary = None

    # Optional -- this gets passed to BinaryFile.find_binary()
    _srv_check = True

    # Optional -- specifies the size of the type
    _size = None

    # Optional -- will be called when an instance of the type is created
    _constructor = None

    # TODO: Implement this!
    # Optional -- will be called when an instance of the type is deleted
    _destructor = None

    def __init__(self, *args, wrap=True, auto_dealloc=False):
        # _manager must be an instance of TypeManager. Otherwise the type
        # wasn't registered by a TypeManager.
        if not isinstance(self._manager, TypeManager):
            raise ValueError(
                'Attribute _manager must be an instance of "TypeManager".')

        # Do we want to wrap a pointer?
        if wrap:
            # Check if only the this pointer was passed
            if len(args) != 1:
                raise ValueError(
                    'If <wrap> is true only one argument is accepted.')

            super(CustomType, self).__init__(args[0])

        # Obviously, we want to create a new instance
        else:
            # Was a size specified?
            if self._size is None:
                raise ValueError(
                    'In order to create an instance _size is required.')

            # Allocate some space
            super(CustomType, self).__init__(
                alloc(self._size, auto_dealloc)
            )

            # Optionally, call a constructor
            if self._constructor is not None:
                self._constructor(*args)

            # No constructor, but arguments? Hmm, the user is doing something
            # wrong
            elif args:
                raise ValueError(
                    'No constructor was specified, but arguments were passed.')


# =============================================================================
# >> TypeManager
# =============================================================================
class TypeManager(dict):
    '''
    The TypeManager is able to reconstruct almost every possible data type.
    '''

    def __call__(self, name, bases, cls_dict):
        '''
        Creates and registers a new class.
        '''

        # Set the manager attribute. This is required, so CustomType.__init__
        # can verify that we have registered the class
        cls_dict['_manager'] = self

        # Create the class object
        cls = type(name, bases, cls_dict)

        # Check if it's a subclass of CustomType
        if not issubclass(cls, CustomType):
            raise ValueError(
                'Custom type "{0}" has to '.format(name) +
                'be a subclass of "CustomType".')

        self[name] = cls
        return cls

    def create_converter(self, name):
        '''
        Creates a new converter for the given name.
        '''

        def convert(ptr):
            if name not in self:
                raise NameError('Unknown converter "{0}"'.format(name))

            return self[name](ptr)

    def create_type_from_file(self, type_name, f, base=CustomType):
        '''
        Creates and registers a new type from a file.
        '''

        # Read the data
        raw_data = ConfigObj(f)

        # Try to close the file. Maybe it was an url or a file object
        try:
            f.close()
        except AttributeError:
            pass

        # Prepare general type information
        cls_dict = {
            '_binary': raw_data.get(Key.BINARY, CustomType._binary),
            '_srv_check': Key.as_bool(
                raw_data.get(Key.SRV_CHECK, str(CustomType._srv_check))),
            '_size': raw_data.get(Key.SIZE, CustomType._size)
        }

        # Prepare pointer and instance attributes
        for method in (self.instance_attribute, self.pointer_attribute):
            attributes = parse_data(
                raw_data.get(method.__name__, {}),
                (
                    (Key.TYPE_NAME, str, NO_DEFAULT),
                    (Key.OFFSET, int, NO_DEFAULT),
                    (Key.DOC, str, None)
                )
            )

            # Create the attributes
            for name, data in attributes:
                cls_dict[name] = method(*data)

        # Prepare arrays
        for method in (
                self.static_instance_array,
                self.dynamic_instance_array,
                self.static_pointer_array,
                self.dynamic_pointer_array):
            arrays = parse_data(
                raw_data.get(method.__name__, {}),
                (
                    (Key.TYPE_NAME, str, NO_DEFAULT),
                    (Key.OFFSET, int, NO_DEFAULT),
                    (Key.LENGTH, int, None),
                    (Key.DOC, str, None)
                )
            )

            # Create the arrays
            for name, data in arrays:
                cls_dict[name] = method(*data)

        # Prepare virtual functions
        vfuncs = parse_data(
            raw_data.get('virtual_function', {}),
            (
                (Key.INDEX, int, NO_DEFAULT),
                (Key.ARGS, Key.as_args_tuple, ()),
                (Key.RETURN_TYPE, Key.as_return_type, Return.VOID),
                (Key.CONVENTION, Key.as_convention, Convention.THISCALL),
                (Key.DOC, str, None)
            )
        )

        # Create the virtual functions
        for name, data in vfuncs:
            cls_dict[name] = virtual_function(*data)

        # Prepare functions
        funcs = parse_data(
            raw_data.get('function', {}),
            (
                (Key.IDENTIFIER, Key.as_identifier, NO_DEFAULT),
                (Key.ARGS, Key.as_args_tuple, ()),
                (Key.RETURN_TYPE, Key.as_return_type, Return.VOID),
                (Key.CONVENTION, Key.as_convention, Convention.THISCALL),
                (Key.DOC, str, None)
            )
        )

        # Create the functions
        for name, data in funcs:
            cls_dict[name] = function(*data)

        # Now create and register the type
        return self(type_name, (base,), cls_dict)

    def instance_attribute(self, type_name, offset, doc=None):
        '''
        Creates a wrapper for an instance attribute.

        Examples:
            Vector vecVal;
            bool bVal;
        '''

        native_type = Type.is_native(type_name)

        def fget(ptr):
            # Handle custom type
            if not native_type:
                return self[type_name](ptr + offset)

            # Handle native type
            return getattr(ptr, 'get_' + type_name)(offset)

        def fset(ptr, value):
            # Handle custom type
            if not native_type:
                if not isinstance(value, Pointer):
                    raise ValueError(
                        'The value must be an instance of the Pointer class')

                value.copy(ptr + offset, self[type_name]._size)

            # Handle native type
            else:
                getattr(ptr, 'set_' + type_name)(value, offset)

        return property(fget, fset, None, doc)

    def pointer_attribute(self, type_name, offset, doc=None):
        '''
        Creates a wrapper for a pointer attribute.

        Examples:
            Vector* pVec;
            bool* pBool;
        '''

        native_type = Type.is_native(type_name)

        def fget(ptr):
            # Get the base address of the pointer. We are now on
            # "instance level"
            ptr = ptr.get_ptr(offset)

            # Handle custom type
            if not native_type:
                return self[type_name](ptr)

            # Handle native type
            return getattr(ptr, 'get_' + type_name)()

        def fset(ptr, value):
            # Get the base address of the pointer. We are now on
            # "instance level"
            ptr = ptr.get_ptr(offset)

            # Handle custom type
            if not native_type:
                if not isinstance(value, Pointer):
                    raise ValueError(
                        'The value must be an instance of the Pointer class')

                # Q: Why do we use copy instead of set_ptr?
                # A: Maybe it's a shared pointer which means that other
                #    classes might also have the address of it. So, changing
                #    address in this particular class wouldn't affect the
                #    other classes.
                # TODO: Is that what we want? Maybe we could add a "copy"
                #       parameter to let the user decide.
                value.copy(ptr, self[type_name]._size)

            # Handle native type
            else:
                getattr(ptr, 'set_' + type_name)(value)

        return property(fget, fset, None, doc)

    def static_instance_array(self, type_name, offset, length=None, doc=None):
        '''
        Creates a wrapper for a static instance array.

        Examples:
            Vector vecArray[10];
            bool boolArray[10];
        '''

        def fget(ptr):
            return Array(self, False, type_name, ptr + offset, length)

        def fset(ptr, value):
            array = fget(ptr)
            for index, val in enumerate(value):
                array[index] = val

        return property(fget, fset, None, doc)

    def dynamic_instance_array(self, type_name, offset, length=None, doc=None):
        '''
        Creates a wrapper for a dynamic instance array.

        Examples:
            Vector* pVecArray;
            bool* pBoolArray;

        Those arrrays are mostly created by the "new" statement.
        '''

        def fget(ptr):
            return Array(self, False, type_name, ptr.get_ptr(offset), length)

        def fset(ptr, value):
            array = fget(ptr)
            for index, val in enumerate(value):
                array[index] = val

        return property(fget, fset, None, doc)

    def static_pointer_array(self, type_name, offset, length=None, doc=None):
        '''
        Creates a wrapper for a static pointer array.

        Examples:
            Vector* pVecArray[10];
            bool* pBoolArray[10];
        '''

        def fget(ptr):
            return Array(self, True, type_name, ptr + offset, length)

        def fset(ptr, value):
            array = fget(ptr)
            for index, val in enumerate(value):
                array[index] = val

        return property(fget, fset, None, doc)

    def dynamic_pointer_array(self, type_name, offset, length=None, doc=None):
        '''
        Creates a wrapper for a dynamic pointer array.

        Examples:
            Vector** pVecArray;
            bool** pBoolArray;

        Those arrays are mostly created by the "new" statement.
        '''

        def fget(ptr):
            return Array(self, True, type_name, ptr.get_ptr(offset), length)

        def fset(ptr, value):
            array = fget(ptr)
            for index, val in enumerate(value):
                array[index] = val

        return property(fget, fset, None, doc)

    def virtual_function(
            self, index, args=(), return_type=Return.VOID,
            convention=Convention.THISCALL, doc=None):
        '''
        Creates a wrapper for a virtual function.
        '''

        # Automatically add the this pointer argument
        args = (Argument.POINTER,) + args

        # Create a converter, if it's not a native type
        return_type = (
            return_type if return_type in Return.values
            else self.create_converter(return_type))

        def fget(ptr):
            # Create the virtual function
            func = ptr.make_virtual_function(
                index,
                convention,
                args,
                return_type
            )

            # Wrap it using MemberFunction, so we don't have to pass the this
            # pointer anymore
            return MemberFunction(self, return_type, func, ptr)

        return property(fget, None, None, doc)

    def function(
            self, identifier, args=(), return_type=Return.VOID,
            convention=Convention.THISCALL, doc=None):
        '''
        Creates a wrapper for a function.

        TODO:
        Make functions returned by this method accessable without having a
        this pointer. E.g. for hooking.

        WORKAROUND:
        Wrap a NULL pointer.
        '''

        # Automatically add the this pointer argument
        args = (Argument.POINTER,) + args

        # Create a converter, if it's not a native type
        return_type = (
            return_type if return_type in Return.values
            else self.create_converter(return_type))

        def fget(ptr):
            if ptr._binary is None:
                raise ValueError('_binary was not specified.')

            # Create a binary object
            binary = find_binary(ptr._binary, ptr._srv_check)

            # Create the function object
            func = binary[identifier].make_function(
                convention,
                args,
                return_type
            )

            # Wrap it using MemberFunction, so we don't have to pass the this
            # pointer anymore
            return MemberFunction(self, return_type, func, ptr)

    def function_typedef(
            self, name, args=(), return_type=Return.VOID,
            convention=Convention.CDECL, doc=None):
        '''
        Creates a new function typedef. So, when a class has an attribute that
        contains a pointer of a function. The attribute will return a Function
        object that will be created by this method.
        '''

        # Create a converter, if it's not a native type
        return_type = (
            return_type if return_type in Return.values
            else self.create_converter(return_type))

        def make_function(ptr):
            return ptr.make_function(convention, args, return_type)

        return make_function

# Create a shared manager instance
manager = TypeManager()
