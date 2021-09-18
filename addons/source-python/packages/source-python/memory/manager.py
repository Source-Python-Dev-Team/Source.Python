# ../memory/manager.py

"""Provides extended memory functionality."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
#   Core
from core import GameConfigObj
#   Memory
from memory import Convention
from memory import DataType
from memory import EXPOSED_CLASSES
from memory import TYPE_SIZES
from memory import alloc
from memory import find_binary
from memory import get_object_pointer
from memory import make_object
from memory import memory_logger
from memory.helpers import Array
from memory.helpers import BasePointer
from memory.helpers import Key
from memory.helpers import MemberFunction
from memory.helpers import NO_DEFAULT
from memory.helpers import Type
from memory.helpers import parse_data


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('CustomType',
           'TypeManager',
           'manager',
           )


# =============================================================================
# >> GLOBAL VARIABLES
# =============================================================================
manager_logger = memory_logger.manager


# =============================================================================
# >> CustomType
# =============================================================================
class CustomType(BasePointer):
    """
    Subclass this class if you want to create a new type.

    Make sure that you have set the metaclass
    attribute to a valid TypeManager instance.
    """

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

    # Optional -- will be called when an instance of the type is deleted
    _destructor = None

    def __init__(self, *args, wrap=False, auto_dealloc=True):
        """Initialize the custom type."""
        # _manager must be an instance of TypeManager. Otherwise the type
        # wasn't registered by a TypeManager.
        if not isinstance(self._manager, TypeManager):
            raise ValueError(
                'Attribute _manager must be an instance of "TypeManager".')

        # Make sure the _manager attribute wasn't set manually
        if self.__class__.__name__ not in self._manager:
            raise TypeError(
                'Custom type was not registered at a type manager.')

        # This set will contain internally allocated pointers.
        self._allocated_pointers = set()

        # This dict will hold pointers, so they don't deallocate if
        # auto_dealloc was set. {<offset>: <pointer>}
        self._pointer_values = {}

        # Do we want to wrap a pointer?
        if wrap:
            # Check if only the this pointer was passed
            if len(args) != 1:
                raise ValueError(
                    'If <wrap> is true only one argument is accepted.')

            super().__init__(args[0])

        # Obviously, we want to create a new instance
        else:
            # Was a size specified?
            if self._size is None:
                raise ValueError(
                    'In order to create an instance _size is required.')

            # Allocate some space
            super().__init__(alloc(self._size, False))
            self.auto_dealloc = auto_dealloc

            # Optionally, call a constructor
            if self._constructor is not None:
                self._constructor(*args)

            # No constructor, but arguments? Hmm, the user is doing something
            # wrong
            elif args:
                raise ValueError(
                    'No constructor was specified, but arguments were passed.')

    def _ptr(self):
        """Return the pointer of the object."""
        return self

    @classmethod
    def _obj(cls, ptr):
        """Wrap the given pointer."""
        return cls(ptr, wrap=True)

    def on_dealloc(self):
        """Call the destructor.

        This method is automatically called, when the pointer gets
        deallocated. It then calls the destructor if it was specified.
        """
        # Call the destructor if it was specified
        if self._destructor is not None:
            self._destructor()


# =============================================================================
# >> TypeManager
# =============================================================================
class TypeManager(dict):
    """Class able to reconstruct almost every possible data type."""

    def __init__(self):
        """Initialize the instance."""
        # Initialize the dictionary
        super().__init__()

        # This dictionary will hold global pointer instances
        self.global_pointers = {}

        # Stores converters
        self.converters = {}

        # Stores function typedefs
        self.function_typedefs = {}

        # Storage for custom calling conventions
        self.custom_conventions = {}

    def __call__(self, name, bases, cls_dict):
        """Create and registers a new class."""
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

    def custom_calling_convention(self, cls):
        """Register a custom calling convention class by its name.

        Example:

        .. code:: python

            @manager.custom_calling_convention
            class MyCustomCallingConvention(CallingConvention):
                pass

            # Equals to...
            class MyCustomCallingConvention(CallingConvention):
                pass

            manager.register_convention(
                'MyCustomCallingConvention',
                MyCustomCallingConvention)
        """
        self.register_convention(cls.__name__, cls)
        return cls

    def register_convention(self, name, convention):
        """Register a custom calling convention.

        :param str name:
            Name of the custom calling convention.
        :param CallingConvention convention:
            The custom calling convention.
        """
        self.custom_conventions[name] = convention

    def unregister_convention(self, name):
        """Unregister a custom calling convention."""
        self.custom_conventions.pop(name, None)

    def register_converter(self, name, obj):
        """Register a callable object as a converter.

        The callable object should only accept a pointer as an argument.
        """
        # Make sure we can call the object
        if not callable(obj):
            raise ValueError('Object is not callable.')

        self.converters[name] = obj

    def unregister_converter(self, name):
        """Unregister a converter."""
        self.converters.pop(name, None)

    def convert(self, name, ptr):
        """Convert the pointer.

        Tries to convert a pointer in the following order:

        Attempts to convert the pointer...
        1. to a custom type
        2. to a exposed type
        3. to a function typedef
        4. by using a converter
        """
        cls = self.get_class(name)
        if cls is not None:
            # Use the class to convert the pointer
            return make_object(cls, ptr)

        # No class was found. Maybe we have luck with a function typedef
        converter = self.function_typedefs.get(name, None)

        # Is there no function typedef?
        if converter is None:
            converter = self.converters.get(name, None)

            # Is there no converter?
            if converter is None:
                raise NameError(
                    'No class, function typedef or ' +
                    'converter found for "{0}".'.format(name))

        # Yay, we found a converter or function typedef!
        return converter(ptr)

    def create_converter(self, name):
        """Create a new converter for the given name."""
        return lambda ptr: self.convert(name, ptr)

    def get_class(self, name):
        """Return the custom type for the given name.

        Tries to return a custom type that matches the given name. If no
        custom type was found, it tries to return a class that was exposed on
        the C++ side. If that fails too, None will be returned.
        """
        return self.get(name, None) or EXPOSED_CLASSES.get(name, None)

    def create_type(self, name, cls_dict, bases=(CustomType,)):
        """Create and registers a new class."""
        # This is just a wrapper for __call__
        return self(name, bases, cls_dict)

    @staticmethod
    def create_pipe(cls_dict):
        """Create a new pipe class that acts like a collection of functions."""
        # Just create a container for all the functions
        return type('Pipe', (object,), cls_dict)

    def create_pipe_from_file(self, f):
        """Create a pipe from a file or URL."""
        return self.create_pipe_from_dict(GameConfigObj(f))

    def create_pipe_from_dict(self, raw_data):
        """Create a pipe from a dictionary."""
        # Prepare functions
        funcs = parse_data(
            self,
            raw_data,
            (
                (Key.BINARY, Key.as_str, NO_DEFAULT),
                (Key.IDENTIFIER, Key.as_identifier, NO_DEFAULT),
                (Key.ARGS, Key.as_args_tuple, ()),
                (Key.RETURN_TYPE, Key.as_return_type, DataType.VOID),
                (Key.CONVENTION, Key.as_convention, Convention.CDECL),
                (Key.SRV_CHECK, Key.as_bool, True),
                (Key.DOC, Key.as_str, None)
            )
        )

        # Create the functions
        cls_dict = {}
        for name, data in funcs:
            cls_dict[name] = self.pipe_function(*data)

        return self.create_pipe(cls_dict)

    def pipe_function(
            self, binary, identifier, args=(), return_type=DataType.VOID,
            convention=Convention.CDECL, srv_check=True, doc=None):
        """Create a simple pipe function."""
        # Create a converter, if it's not a native type
        if return_type not in DataType.values:
            return_type = self.create_converter(return_type)

        # Find the binary
        binary = find_binary(binary, srv_check)

        # Find the address and make it to a function
        func = binary[identifier].make_function(convention, args, return_type)

        # Add documentation
        func.__doc__ = doc
        return func

    def create_type_from_file(self, type_name, f, bases=(CustomType,)):
        """Create and registers a new type from a file or URL."""
        return self.create_type_from_dict(
            type_name, GameConfigObj(f), bases)

    def create_type_from_dict(self, type_name, raw_data, bases=(CustomType,)):
        """Create and registers a new type from a dictionary."""
        # Prepare general type information
        data = tuple(parse_data(
            self,
            # Discard all subkeys and add the new dict to a another dict to
            # make it work with parse_data(). Okay, this can be improved...
            {0: dict((k, v) for k, v in raw_data.items() if not isinstance(
                v, dict))},
            (
                (Key.BINARY, Key.as_str, CustomType._binary),
                (Key.SRV_CHECK, Key.as_bool, CustomType._srv_check),
                (Key.SIZE, Key.as_int, CustomType._size)
            )
        ))[0][1]

        cls_dict = dict(zip(('_binary', '_srv_check', '_size'), data))

        # Prepare pointer and instance attributes
        for method in (self.instance_attribute, self.pointer_attribute):
            attributes = parse_data(
                self,
                raw_data.get(method.__name__, {}),
                (
                    (Key.TYPE_NAME, Key.as_attribute_type, NO_DEFAULT),
                    (Key.OFFSET, Key.as_int, NO_DEFAULT),
                    (Key.DOC, Key.as_str, None)
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
                self,
                raw_data.get(method.__name__, {}),
                (
                    (Key.TYPE_NAME, Key.as_attribute_type, NO_DEFAULT),
                    (Key.OFFSET, Key.as_int, NO_DEFAULT),
                    (Key.LENGTH, Key.as_int, None),
                    (Key.DOC, Key.as_str, None)
                )
            )

            # Create the arrays
            for name, data in arrays:
                cls_dict[name] = method(*data)

        # Prepare virtual functions
        vfuncs = parse_data(
            self,
            raw_data.get('virtual_function', {}),
            (
                (Key.OFFSET, Key.as_int, NO_DEFAULT),
                (Key.ARGS, Key.as_args_tuple, ()),
                (Key.RETURN_TYPE, Key.as_return_type, DataType.VOID),
                (Key.CONVENTION, Key.as_convention, Convention.THISCALL),
                (Key.DOC, Key.as_str, None)
            )
        )

        # Create the virtual functions
        for name, data in vfuncs:
            cls_dict[name] = self.virtual_function(*data)

        # Prepare functions
        funcs = parse_data(
            self,
            raw_data.get('function', {}),
            (
                (Key.IDENTIFIER, Key.as_identifier, NO_DEFAULT),
                (Key.ARGS, Key.as_args_tuple, ()),
                (Key.RETURN_TYPE, Key.as_return_type, DataType.VOID),
                (Key.CONVENTION, Key.as_convention, Convention.THISCALL),
                (Key.DOC, Key.as_str, None)
            )
        )

        # Create the functions
        for name, data in funcs:
            cls_dict[name] = self.function(*data)

        # Now create and register the type
        return self(type_name, bases, cls_dict)

    def instance_attribute(self, type_name, offset, doc=None):
        """Create a wrapper for an instance attribute.

        Examples:
            Vector vecVal;
            bool bVal;
        """
        native_type = Type.is_native(type_name)

        def fget(ptr):
            """Return the instance attribute value."""
            # Handle custom type
            if not native_type:
                return self.convert(type_name, ptr + offset)

            # Handle native type
            return getattr(ptr, 'get_' + type_name)(offset)

        def fset(ptr, value):
            """Set the instance attribute value."""
            # Handle custom type
            if not native_type:
                cls = self.get_class(type_name)
                if cls is None:
                    raise NameError('Unknown class "{0}".'.format(type_name))

                get_object_pointer(value).copy(
                    ptr + offset,
                    cls._size
                )

            # Handle native type
            else:
                getattr(ptr, 'set_' + type_name)(value, offset)

        return property(fget, fset, None, doc)

    def pointer_attribute(self, type_name, offset, doc=None):
        """Create a wrapper for a pointer attribute.

        Examples:
            Vector* pVec;
            bool* pBool;
        """
        native_type = Type.is_native(type_name)

        def fget(ptr):
            """Get the pointer attribute value."""
            # Get the base address of the pointer. We are now on
            # "instance level"
            ptr = ptr.get_pointer(offset)

            # Handle custom type
            if not native_type:
                return self.convert(type_name, ptr)

            # Handle native type
            return getattr(ptr, 'get_' + type_name)()

        def fset(ptr, value):
            """Set the pointer attribute value."""
            # Handle custom type
            if not native_type:
                # Set the pointer
                ptr.set_pointer(value, offset)

                # Make sure the value will not deallocate as long as it is
                # part of this object
                ptr._pointer_values[offset] = value

            # Handle native type
            else:
                # Go down to "instance level"
                instance_ptr = ptr.get_pointer(offset)

                # Is there no space allocated?
                if not instance_ptr:
                    # Allocate space for the value
                    instance_ptr = alloc(TYPE_SIZES[type_name.upper()])

                    # Add the pointer to the set, so there will be a reference
                    # until the instance gets deleted
                    ptr._allocated_pointers.add(instance_ptr)

                    # Set the pointer
                    ptr.set_pointer(instance_ptr, offset)

                # Set the value
                getattr(instance_ptr, 'set_' + type_name)(value)

        return property(fget, fset, None, doc)

    def static_instance_array(self, type_name, offset, length=None, doc=None):
        """Create a wrapper for a static instance array.

        Examples:
            Vector vecArray[10];
            bool boolArray[10];
        """
        def fget(ptr):
            """Get the static instance array."""
            return Array(self, False, type_name, ptr + offset, length)

        def fset(ptr, value):
            """Set all values in the static instance array."""
            array = fget(ptr)
            for index, val in enumerate(value):
                array[index] = val

        return property(fget, fset, None, doc)

    def dynamic_instance_array(self, type_name, offset, length=None, doc=None):
        """Create a wrapper for a dynamic instance array.

        Examples:
            Vector* pVecArray;
            bool* pBoolArray;

        Those arrrays are mostly created by the "new" statement.
        """
        def fget(ptr):
            """Get the dynamic instance array."""
            return Array(
                self, False, type_name, ptr.get_pointer(offset), length)

        def fset(ptr, value):
            """Set all values for the dynamic instance array."""
            array = fget(ptr)
            for index, val in enumerate(value):
                array[index] = val

        return property(fget, fset, None, doc)

    def static_pointer_array(self, type_name, offset, length=None, doc=None):
        """Create a wrapper for a static pointer array.

        Examples:
            Vector* pVecArray[10];
            bool* pBoolArray[10];
        """
        def fget(ptr):
            """Get the static pointer array."""
            return Array(self, True, type_name, ptr + offset, length)

        def fset(ptr, value):
            """Set all values for the static pointer array."""
            array = fget(ptr)
            for index, val in enumerate(value):
                array[index] = val

        return property(fget, fset, None, doc)

    def dynamic_pointer_array(self, type_name, offset, length=None, doc=None):
        """Create a wrapper for a dynamic pointer array.

        Examples:
            Vector** pVecArray;
            bool** pBoolArray;

        Those arrays are mostly created by the "new" statement.
        """
        def fget(ptr):
            """Get the dynamic pointer array."""
            return Array(
                self, True, type_name, ptr.get_pointer(offset), length)

        def fset(ptr, value):
            """Set all values for the dynamic pointer array."""
            array = fget(ptr)
            for index, val in enumerate(value):
                array[index] = val

        return property(fget, fset, None, doc)

    def virtual_function(
            self, index, args=(), return_type=DataType.VOID,
            convention=Convention.THISCALL, doc=None):
        """Create a wrapper for a virtual function."""
        # Automatically add the this pointer argument
        args = (DataType.POINTER,) + tuple(args)

        # Create a converter, if it's not a native type
        if return_type not in DataType.values:
            return_type = self.create_converter(return_type)

        def fget(ptr):
            """Return the virtual function."""
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
            self, identifier, args=(), return_type=DataType.VOID,
            convention=Convention.THISCALL, doc=None):
        """Create a wrapper for a function."""
        # Automatically add the this pointer argument
        args = (DataType.POINTER,) + tuple(args)

        # Create a converter, if it's not a native type
        if return_type not in DataType.values:
            return_type = self.create_converter(return_type)

        class fget(object):
            def __get__(fget_self, obj, cls):
                if cls._binary is None:
                    raise ValueError('_binary was not specified.')

                # Create a binary object
                binary = find_binary(cls._binary, cls._srv_check)

                # Create the function object
                func = binary[identifier].make_function(
                    convention,
                    args,
                    return_type
                )

                # Called with a this pointer?
                if obj is not None:
                    # Wrap the function using MemberFunction, so we don't have
                    # to pass the this pointer anymore
                    func = MemberFunction(self, return_type, func, obj)

                func.__doc__ = doc
                return func

        return fget()

    def function_typedef(
            self, name, args=(), return_type=DataType.VOID,
            convention=Convention.CDECL, doc=None):
        """Create a new function typedef.

        When a class has an attribute that contains a pointer of a function,
        the attribute will return a Function object that will be created
        by this method.
        """
        # Create a converter, if it's not a native type
        if return_type not in DataType.values:
            return_type = self.create_converter(return_type)

        def make_function(ptr):
            """Return the function typedef."""
            func = ptr.make_function(convention, args, return_type)
            func.__doc__ = doc
            return func

        self.function_typedefs[name] = make_function
        return make_function

    def create_function_typedefs_from_file(self, f):
        """Create function typedefs from a file."""
        # Read the data
        raw_data = GameConfigObj(f)

        # Prepare typedefs
        typedefs = parse_data(
            self,
            raw_data,
            (
                (Key.ARGS, Key.as_args_tuple, ()),
                (Key.RETURN_TYPE, Key.as_return_type, DataType.VOID),
                (Key.CONVENTION, Key.as_convention, Convention.CDECL),
                (Key.DOC, Key.as_str, None)
            )
        )

        # Create the typedefs
        for name, data in typedefs:
            self.function_typedef(name, *data)

    def global_pointer(
            self, cls, binary, identifier, offset=0, level=0, srv_check=True):
        """Search for a global pointer and wrap the it."""
        manager_logger.log_debug(
            'Retrieving global pointer for {}...'.format(cls.__name__))

        # Get the binary
        binary = find_binary(binary, srv_check)

        # Get the global pointer
        ptr = binary.find_pointer(identifier, offset, level)

        # Raise an error if the pointer is invalid
        if not ptr:
            raise ValueError('Unable to find the global pointer.')

        # Wrap the pointer using the given class and save the instance
        ptr = self.global_pointers[cls.__name__] = make_object(cls, ptr)
        return ptr

    def create_global_pointers_from_file(self, f):
        """Create global pointers from a file."""
        # Parse pointer data
        pointers = parse_data(
            self,
            GameConfigObj(f),
            (
                (Key.BINARY, Key.as_str, NO_DEFAULT),
                (Key.IDENTIFIER, Key.as_identifier, NO_DEFAULT),
                (Key.OFFSET, Key.as_int, 0),
                (Key.LEVEL, Key.as_int, 0),
                (Key.SRV_CHECK, Key.as_bool, True),
            )
        )

        # Create the global pointer objects
        for name, data in pointers:
            cls = self.get_class(name)
            if cls is None:
                raise NameError('Unknown class "{0}".'.format(name))

            self.global_pointer(cls, *data)

    def get_global_pointer(self, name):
        """Return the global pointer for the given class."""
        # Allow passing class objects
        if not isinstance(name, str):
            name = name.__name__

        # Raise an error if no global pointer was found.
        if name not in self.global_pointers:
            raise NameError('No global pointer found for "{0}".'.format(name))

        return self.global_pointers[name]

# Create a shared manager instance
manager = TypeManager()
