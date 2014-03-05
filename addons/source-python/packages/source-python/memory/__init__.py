# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python
from memory_c import *


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
# Add all the global variables to __all__
__all__ = [
]


# =============================================================================
# >> Type
# =============================================================================
class Type:
    BOOL         = 'bool'
    CHAR         = 'char'
    UCHAR        = 'uchar'
    SHORT        = 'short'
    USHORT       = 'ushort'
    INT          = 'int'
    UINT         = 'uint'
    LONG         = 'long'
    ULONG        = 'ulong'
    LONG_LONG    = 'long_long'
    ULONG_LONG   = 'ulong_long'
    FLOAT        = 'float'
    DOUBLE       = 'double'
    POINTER      = 'ptr'
    STRING_PTR   = 'string_ptr'
    STRING_ARRAY = 'string_array'

    @staticmethod
    def is_native(attr_type):
        '''
        Returns True if the given type name is a native type.
        '''

        return hasattr(Type, attr_type.upper())


# =============================================================================
# >> BasePointer
# =============================================================================
class BasePointer(Pointer):
    # These four operator functions are required. Otherwise we would downcast
    # the instance to the Pointer class if we add or subtract bytes.
    def __add__(self, other):
        return self.__class__(int(self) + int(other))

    def __radd__(self, other):
        return self + other

    def __sub__(self, other):
        return self.__class__(int(self) - int(other))

    def __rsub__(self, other):
        return self - other

    def get(self, type_name, offset=0):
        '''
        Returns the value at the given memory location.
        '''

        return getattr(self, 'get_' + type_name)(offset)

    def set(self, type_name, value, offset=0):
        '''
        Sets the value at the given memory location.
        '''

        getattr(self, 'set_' + type_name)(value, offset)


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
    __manager__ = None

    # This is required if TypeManager.function() is used. It should contain
    # the path to the binary that defines this type
    __binary__ = None

    # Optional -- this gets passed to BinaryFile.find_binary()
    __srv_check__ = True

    # Optional -- specifies the size of the type
    __size__ = None

    # Optional -- will be called when an instance of the type is created
    __constructor__ = None

    # TODO: Implement this!
    # Optional -- will be called when an instance of the type is deleted
    __destructor__  = None

    def __init__(self, *args, wrap=True, auto_dealloc=False):
        # __manager__ must be an instance of TypeManager. Otherwise the type
        # wasn't registered by a TypeManager.
        if not isinstance(self.__manager__, TypeManager):
            raise ValueError('Attribute __manager__ must be an instance of ' \
                '"TypeManager".')

        # Do we want to wrap a pointer?
        if wrap:
            # Check if only the this pointer was passed
            if len(args) != 1:
                raise ValueError('If <wrap> is true only one argument is ac' \
                    'cepted.')

            super(CustomType, self).__init__(args[0])

        # Obviously, we want to create a new instance
        else:
            # Was a size specified?
            if self.__size__ == None:
                raise ValueError('In order to create an instance __size__ i' \
                    's required.')

            # Allocate some space
            super(CustomType, self).__init__(
                alloc(self.__size__, auto_dealloc)
            )

            # Optionally, call a constructor
            if self.__constructor__ is not None:
                self.__constructor__(*args)

            # No constructor, but arguments? Hmm, the user is doing something
            # wrong
            elif args:
                raise ValueError('No constructor was specified, but argumen' \
                    'ts were passed.')


# =============================================================================
# >> Array
# =============================================================================
class Array(BasePointer):
    # Optional -- specifies the length of the array
    __length__ = None

    # Contains the type name of the array
    __type_name__ = None

    # Set to True if the array contains pointers, else False
    __is_ptr__ = None

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

        self.__manager__   = manager
        self.__is_ptr__    = is_ptr
        self.__type_name__ = type_name
        self.__length__    = length

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
        Returns a generator can iterate over the array.
        '''

        # This prevents users from iterating over the array without having
        # __length__ specified. Otherwise the server would hang or crash.
        if self.__length__ is None:
            raise ValueError('Cannot iterate over the array without __lengt' \
                'h__ being specified.')

        for index in range(self.__length__):
            yield self[index]

    def __make_attribute(self, index):
        '''
        Validates the index and returns a new property object.
        '''

        # Validate the index, so we don't access invalid memory addresses
        if self.__length__ is not None and index >= self.__length__:
            raise IndexError('Index out of range')

        # Construct the proper function name
        name = ('pointer' if self.__is_ptr__ else 'instance') + '_attribute'

        # Get the function and call it
        return getattr(self.__manager__, name)(
            self.__type_name__,
            self.get_offset(index)
        )

    def get_offset(self, index):
        '''
        Returns the offset of the given index.
        '''

        # Pointer arrays always have every 4 bytes a new pointer
        if self.__is_ptr__:
            return index * TYPE_SIZES[Type.POINTER]

        # Every 1, 2, 4 or 8 bytes is a new value
        if Type.is_native(self.__type_name__):
            return index * TYPE_SIZES[self.__type_name__]

        # Get the class of the custom type
        cls = self.__manager__[self.__type_name__]

        # To access a value, we require the proper size of a custom type
        if cls.__size__ == None:
            raise ValueError('Array requires a size to access its values.')

        # Every x bytes is a new instance
        return index * cls.__size__

    # Arrays have another constructor and we don't want to downcast. So, we
    # have to implement these operators here again.
    def __add__(self, other):
        return self.__class__(
            self.__manager__,
            self.__is_ptr__,
            self.__type_name__,
            int(self) + int(other),
            self.__length__
        )

    def __sub__(self, other):
        return self.__class__(
            self.__manager__,
            self.__is_ptr__,
            self.__type_name__,
            int(self) - int(other),
            self.__length__
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
    __manager__ = None

    # Holds the this pointer
    __this__ = None

    # Holds the return type name
    __type_name__ = None

    def __init__(self, manager, return_type, func, this):
        super(MemberFunction, self).__init__(func)

        self.__manager__   = manager
        self.__this__      = this
        self.__type_name__ = return_type

    def __call__(self, *args):
        '''
        Calls the function dynamically.
        '''

        result = super(MemberFunction, self).__call__(self.__this__, *args)

        # Wrap the result if it's a custom type
        if self.__type_name__ not in Return.values:
            return self.__manager__[self.__type_name__](result)

        return result

    def call_trampoline(self, *args):
        '''
        Calls the trampoline dynamically.
        '''

        result = super(MemberFunction, self).call_trampoline(
            self.__this__,
            *args
        )

        # Wrap the result if it's a custom type
        if self.__type_name__ not in Return.values:
            return self.__manager__[self.__type_name__](result)

        return result


# =============================================================================
# >> TypeManager
# =============================================================================
class TypeManager(dict):
    def __call__(self, name, bases, cls_dict):
        '''
        Creates and registers a new class.
        '''

        # Set the manager attribute. This is required, so CustomType.__init__
        # can verify that we have registered the class
        cls_dict['__manager__'] = self

        # Create the class object
        cls = type(name, bases, cls_dict)

        # Check if it's a subclass of CustomType
        if not issubclass(cls, CustomType):
            raise ValueError('Custom type "%s" has to be a subclass of "Cus' \
                'tomType".'% name)

        self[name] = cls
        return cls

    def instance_attribute(self, attr_type, offset, doc=None):
        '''
        Creates a wrapper for an instance attribute.

        Examples:
            Vector vecVal;
            bool bVal;
        '''

        native_type = Type.is_native(attr_type)

        def fget(ptr):
            # Handle custom type
            if not native_type:
                return self[attr_type](ptr + offset)

            # Handle native type
            return ptr.get(attr_type, offset)

        def fset(ptr, value):
            # Handle custom type
            if not native_type:
                if not isinstance(value, Pointer):
                    raise ValueError('The value must be an instance of the ' \
                        'Pointer class')

                value.copy(ptr + offset, self[attr_type].__size__)

            # Handle native type
            else:
                ptr.set(attr_type, value, offset)

        return property(fget, fset, None, doc)

    def pointer_attribute(self, attr_type, offset, doc=None):
        '''
        Creates a wrapper for a pointer attribute.

        Examples:
            Vector* pVec;
            bool* pBool;
        '''

        native_type = Type.is_native(attr_type)

        def fget(ptr):
            # Get the base address of the pointer. We are now on
            # "instance level"
            ptr = ptr.get_ptr(offset)

            # Handle custom type
            if not native_type:
                return self[attr_type](ptr)

            # Handle native type
            return ptr.get(attr_type)

        def fset(ptr, value):
            # Get the base address of the pointer. We are now on
            # "instance level"
            ptr = ptr.get_ptr(offset)

            # Handle custom type
            if not native_type:
                if not isinstance(value, Pointer):
                    raise ValueError('The value must be an instance of the ' \
                        'Pointer class')

                # Q: Why do we use copy instead of set_ptr?
                # A: Maybe it's a shared pointer which means that other
                #    classes might also have the address of it. So, changing
                #    address in this particular class wouldn't affect the
                #    other classes.
                # TODO: Is that what we want? Maybe we could add a "copy"
                #       parameter to let the user decide.
                value.copy(ptr, self[attr_type].__size__)

            # Handle native type
            else:
                ptr.set(attr_type, value)

        return property(fget, fset, None, doc)

    def static_instance_array(self, attr_type, offset, length=None, doc=None):
        '''
        Creates a wrapper for a static instance array.

        Examples:
            Vector vecArray[10];
            bool boolArray[10];
        '''

        def fget(ptr):
            return Array(self, False, attr_type, ptr + offset, length)

        def fset(ptr, value):
            array = fget(ptr)
            for index, val in enumerate(value):
                array[index] = val

        return property(fget, fset, None, doc)

    def dynamic_instance_array(self, attr_type, offset, length=None, doc=None):
        '''
        Creates a wrapper for a dynamic instance array.

        Examples:
            Vector* pVecArray;
            bool* pBoolArray;

        Those arrrays are mostly created by the "new" statement.
        '''

        def fget(ptr):
            return Array(self, False, attr_type, ptr.get_ptr(offset), length)

        def fset(ptr, value):
            array = fget(ptr)
            for index, val in enumerate(value):
                array[index] = val

        return property(fget, fset, None, doc)

    def static_pointer_array(self, attr_type, offset, length=None, doc=None):
        '''
        Creates a wrapper for a static pointer array.

        Examples:
            Vector* pVecArray[10];
            bool* pBoolArray[10];
        '''

        def fget(ptr):
            return Array(self, True, attr_type, ptr + offset, length)

        def fset(ptr, value):
            array = fget(ptr)
            for index, val in enumerate(value):
                array[index] = val

        return property(fget, fset, None, doc)

    def dynamic_pointer_array(self, attr_type, offset, length=None, doc=None):
        '''
        Creates a wrapper for a dynamic pointer array.

        Examples:
            Vector** pVecArray;
            bool** pBoolArray;

        Those arrays are mostly created by the "new" statement.
        '''

        def fget(ptr):
            return Array(self, True, attr_type, ptr.get_ptr(offset), length)

        def fset(ptr, value):
            array = fget(ptr)
            for index, val in enumerate(value):
                array[index] = val

        return property(fget, fset, None, doc)

    def virtual_function(self, index, args=(), return_type=Return.VOID, doc=None,
            convention=Convention.THISCALL):
        '''
        Creates a wrapper for a virtual function.
        '''

        native_type = return_type in Return.values

        # Automatically add the this pointer argument
        args = (Argument.POINTER,) + args

        def fget(ptr):
            # Create the virtual function
            func = ptr.make_virtual_function(
                index,
                convention,
                args,
                return_type if native_type else Return.POINTER
            )

            # Wrap it using MemberFunction, so we don't have to pass the this
            # pointer anymore
            return MemberFunction(self, return_type, func, ptr)

        return property(fget, None, None, doc)

    def function(self, identifier, args=(), return_type=Return.VOID, doc=None,
            convention=Convention.THISCALL):
        '''
        Creates a wrapper for a function.

        TODO:
        Make functions returned by this method accessable without having a
        this pointer. E.g. for hooking.

        WORKAROUND:
        Wrap a NULL pointer.
        '''

        native_type = return_type in Return.values

        # Automatically add the this pointer argument
        args = (Argument.POINTER,) + args

        def fget(ptr):
            if ptr.__binary__ is None:
                raise ValueError('__binary__ was not specified.')

            # Create a binary object
            binary = find_binary(ptr.__binary__, ptr.__srv_check__)

            # Create the function object
            func = binary[identifier].make_function(
                convention,
                args,
                return_type if native_type else Return.POINTER
            )

            # Wrap it using MemberFunction, so we don't have to pass the this
            # pointer anymore
            return MemberFunction(self, return_type, func, ptr)


# =============================================================================
# >> FUNCTIONS
# =============================================================================
def callback(convention, args=(), return_type=Return.VOID):
    '''
    Use this decorator to create a C++ callback that calls back to the
    decorated function.

    EXAMPLE:

    @callback(Convention.CDECL, (Argument.INT, Argument.INT), Return.INT)
    def add(x, y, ebp):
        return x + y

    <add> is now a Callback instance, but you can still call it like a normal
    Python function:

    assert add(4, 6) == 10
    '''

    def wait_for_func(func):
        return Callback(func, convention, args, return_type)

    return wait_for_func