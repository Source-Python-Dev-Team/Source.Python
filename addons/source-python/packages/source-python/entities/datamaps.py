# ../entities/datamaps.py

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
from basetypes_c import Color
from basetypes_c import Interval
from conversions_c import pointer_from_edict
from conversions_c import edict_from_pointer
from datamap_c import DataMap
from datamap_c import FieldTypes
from datamap_c import InputData
from datamap_c import TypeDescriptionFlags
from entity_c import Edict
from mathlib_c import Quaternion
from mathlib_c import Vector
from memory_c import Argument
from memory_c import Convention
from memory_c import Function
from memory_c import Return
from memory_c import get_pointer
from memory_c import make_object
#   Entities
from entities.attributes import EntityAttributes
from entities.constants import DATA_DESC_MAP_OFFSET


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
# Set all to an empty list
__all__ = []


# =============================================================================
# >> GLOBAL VARIABLES
# =============================================================================
_SupportedOffsetTypes = {
    FieldTypes.BOOLEAN: 'bool',
    FieldTypes.CHARACTER: 'char',
    FieldTypes.CLASSPTR: 'pointer',
    FieldTypes.EHANDLE: 'int',
    FieldTypes.FLOAT: 'float',
    FieldTypes.FUNCTION: 'pointer',
    FieldTypes.INTEGER: 'int',
    FieldTypes.MODELINDEX: 'uint',
    FieldTypes.MODELNAME: 'string_pointer',
    FieldTypes.SHORT: 'short',
    FieldTypes.SOUNDNAME: 'string_pointer',
    FieldTypes.STRING: 'string_pointer',
    FieldTypes.TICK: 'int',
    FieldTypes.TIME: 'float',
}

_SupportedInputTypes = {
    FieldTypes.BOOLEAN: 'bool',
    FieldTypes.COLOR32: 'color',
    FieldTypes.FLOAT: 'float',
    FieldTypes.INTEGER: 'int',
    FieldTypes.STRING: 'string',
    FieldTypes.VECTOR: 'vector',
}

_SupportedTypeObjects = {
    FieldTypes.COLOR32: Color,
    FieldTypes.EDICT: Edict,
    FieldTypes.INTERVAL: Interval,
    FieldTypes.POSITION_VECTOR: Vector,
    FieldTypes.QUATERNION: Quaternion,
    FieldTypes.VECTOR: Vector,
}


# =============================================================================
# >> CLASSES
# =============================================================================
class _NamedDataMaps(EntityAttributes):
    '''Dictionary that stores entities with their named datamap objects'''

    type = 'datamaps'
    unrepr = False
    instance = staticmethod(lambda value: value)

# Get the _NamedDataMaps instance
NamedDataMaps = _NamedDataMaps()


class _EntityDataMaps(dict):
    '''Dictionary that stores all datamaps by server class name'''

    def get_data_maps(self, edict):
        '''Converts the given edict to its server class and returns the
            _EntityTypeDataMap dictionary for the server class and all bases'''

        # Is the GetDataDescMap offset set?
        if DATA_DESC_MAP_OFFSET is None:
            return {}

        # Get the edict's classname
        server_class = edict.get_networkable().get_server_class().name

        # Has the entity's server class already been added to the dictionary?
        if not server_class in self:

            # Get the pointer of the edict
            pointer = pointer_from_edict(edict)

            # Create the function for the entity's GetDataDescMap
            function = pointer.make_virtual_function(
                DATA_DESC_MAP_OFFSET, Convention.THISCALL,
                (Argument.POINTER, ), Return.POINTER)

            # Create a DataMap object
            datamap = make_object(DataMap, function(pointer))

            # Set the new server_class name to the datamap's name.
            # This is done to fix a KeyError that occurs when the
            # server_class is still not in the dictionary later on.
            server_class = datamap.class_name

            # Is the current server_class value not in the dictionary?
            if not server_class in self:

                # Use "while" to get all datamaps for the entity
                while datamap:

                    # Store the class name as a _DataMap dictionary object
                    self[datamap.class_name] = _DataMap(datamap)

                    # Move up a level and store the base of the current datamap
                    self[datamap.class_name].base = datamap = datamap.base

        # Create a dictionary of values to return
        values = dict()

        # Use a "while" statement to get all values for the entity
        while server_class in self:

            # Add the current server_class' values to the dictionary
            values.update(self[server_class])

            # Use try/except in-case .base returns None
            try:

                # Move up one level
                server_class = self[server_class].base.class_name

            # Was an exception encountered?
            except AttributeError:

                # Break the while loop
                break

        # Return the values
        return values

# Get the _EntityDataMaps instance
EntityDataMaps = _EntityDataMaps()


class _DataMap(dict):
    '''Dictionary class used to store datamap values by type'''

    def __init__(self, datamap):
        '''
            Adds all type descriptions to the dictionary for the given datamap
        '''

        # Loop through all type descriptions in the current datamap
        for desc in datamap:

            # Does the name exist?
            if desc.name is None:
                continue

            # Is this an embedded type description?
            if desc.type == FieldTypes.EMBEDDED:

                # Store the desc name in the dictionary as an embedded
                value = self[desc.name] = _Embedded(desc.embedded_datamap)

            # Is this an input type description?
            elif (desc.flags & TypeDescriptionFlags.INPUT
                    and not desc.flags & TypeDescriptionFlags.KEY):

                # Store the desc name in the dictionary as an input
                value = self[desc.name] = _Input(desc)

            # Is this a function table type description?
            elif desc.flags & TypeDescriptionFlags.FUNCTIONTABLE:

                # Store the desc name in the dictionary as a function table
                value = self[desc.name] = _FunctionTable(desc)

            # Is this a supported type object?
            elif desc.type in _SupportedTypeObjects:

                # Store the desc name in the dictionary as its type
                value = self[desc.name] = _TypeObject(
                    desc, _SupportedTypeObjects[desc.type])

            # Is this type description supported?
            elif desc.type in _SupportedOffsetTypes:

                # Store the desc name in the dictionary as a data desc object
                value = self[desc.name] = _DataDesc(desc)

            # Is the type not supported?
            else:

                # For now, this type is unsupported, so move on to the next
                continue

            # Does the desc have a valid external name:
            if not desc.external_name is None:

                # Add the external name to the dictionary
                self[desc.external_name] = value


class _BaseType(object):
    '''Base class used for some datadesc types'''

    def _get_value(self):
        '''Return the instance itself'''
        return self

    def _set_value(self, value):
        '''Raise an error, since setting with this type is not allowed'''
        raise TypeError('Cannot set type "{0}"'.format(type(self).__name__))


class _Embedded(_DataMap, _BaseType):
    '''Dictionary class used to store embedded datamaps'''

    def __getattr__(self, attr):
        '''
            Override __getattr__ to set the pointer and return the item's value
        '''

        # Is the attribute a member of the dictionary?
        if not attr in self:
            raise AttributeError('Unknown type description "{0}"'.format(attr))

        # Set the item's pointer to the current one for the embedded class
        self[attr].current_pointer = self.current_pointer

        # Return the value of the object
        return self[attr]._get_value()

    def __setattr__(self, attr, value):
        '''Override __setattr__ to set the current pointer and
            call the _set_value method of the wanted instance'''

        # Is the attribute a member of the dictionary?
        if attr in self:

            # Is the attribute not a _DataDesc instance?
            if not isinstance(self[attr], _DataDesc):
                raise TypeError(
                    'Cannot set type "{0}"'.format(type(self[attr].__name)))

            # Set the _DataDesc instance pointer
            self[attr].current_pointer = self.current_pointer

            # Set the value
            self[attr]._set_value(value)

        # Is the attribute just an attribute?
        else:

            # Set the attribute
            super(_Embedded, self).__setattr__(attr, value)


class _BaseFunctions(Function, _BaseType):
    '''Base class for all function type descriptions'''

    def __init__(self, desc):
        '''Store the name, type, and function'''
        self.name = desc.name
        self.type = desc.type
        function = desc.input.make_function(
                        Convention.THISCALL, self.arguments, Return.VOID)

        # Initialize the Function instance
        super(_BaseFunctions, self).__init__(function)


class _FunctionTable(_BaseFunctions):
    '''Class used to interact with FUNCTIONTABLE objects'''

    # Set the arguments for FUNCTIONTABLEs
    arguments = (Argument.POINTER, )

    def __call__(self):
        '''Calls the stored function'''
        super(_FunctionTable, self).__call__(self.current_pointer)


class _Input(_BaseFunctions):
    '''Class used to interact with INPUT objects'''

    # Set the arguments for INPUTs
    arguments = (Argument.POINTER, Argument.POINTER)

    def __call__(self, value=None, caller=None, activator=None):
        '''Calls the stored function with the values given'''

        # Is the type not VOID but no value was given?
        if value is None and self.type != FieldTypes.VOID:
            raise ValueError(
                'Must provide a value for {0}'.format(self.name))

        # Is the type VOID but a value was given?
        if not value is None and self.type == FieldTypes.VOID:
            raise ValueError(
                '{0} is type Void.  Do not pass a value.'.format(
                    self.name))

        # Get an InputData instance
        inputdata = InputData()

        # Does the caller need set?
        if not caller is None:
            inputdata.caller = caller

        # Does the activator need set?
        if not activator is None:
            inputdata.activator = activator

        # Does the function require a value?
        if self.type != FieldTypes.VOID:

            # Set the value
            getattr(inputdata.value, 'set_{0}'.format(
                _SupportedInputTypes[self.type]))(value)

        # Call the function
        super(_Input, self).__call__(self.current_pointer, inputdata)


class _DataDesc(object):
    '''Class used to get and set base type description objects'''

    def __init__(self, desc):
        '''Store the type description instance'''
        self.get_attr = 'get_{0}'.format(_SupportedOffsetTypes[desc.type])
        self.set_attr = 'set_{0}'.format(_SupportedOffsetTypes[desc.type])
        self.offset = desc.offset

    def _get_value(self):
        '''Return the current value of the type
            description for the current pointer'''
        return getattr(self.current_pointer, self.get_attr)(self.offset)

    def _set_value(self, value):
        '''Set the value of the type description to
            the given value for the current pointer'''
        getattr(self.current_pointer, self.set_attr)(value, self.offset)


class _TypeObject(object):
    '''Class used to get and set other types of objects for entities'''

    def __init__(self, desc, type_object):
        '''Store the offset and type of object'''
        self.offset = desc.offset
        self.type_object = type_object

    def _get_value(self):
        '''Returns the object type at the offset's address'''
        return make_object(
            self.type_object, self.current_pointer + self.offset)

    def _set_value(self, value):
        '''Sets the object type value at the offset's address'''

        # Is the value of the correct type?
        if not isinstance(value, self.type_object):
            raise ValueError('Invalid type given. Use "{0}" not "{1}"'.format(
                self.type_object.__name__, type(value).__name__))

        # Set the value
        self.current_pointer.set_pointer(get_pointer(value), self.offset)
