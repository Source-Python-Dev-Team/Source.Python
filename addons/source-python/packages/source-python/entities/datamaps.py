# ../entities/datamaps.py

# =============================================================================
# >> IMPORTS
# =============================================================================
# Python Imports
#   OS
from os import name as os_name

# Source.Python Imports
from conversions_c import pointer_from_edict
from datamap_c import DataMap
from datamap_c import FieldTypes
from datamap_c import InputData
from datamap_c import TypeDescriptionFlags
from memory_c import Argument
from memory_c import Convention
from memory_c import Return
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
_SupportedTypes = {
    FieldTypes.BOOLEAN: 'bool',
    FieldTypes.CHARACTER: 'char',
    FieldTypes.FLOAT: 'float',
    FieldTypes.INTEGER: 'int',
    FieldTypes.SHORT: 'short',
}

_InputSetTypes = {
    FieldTypes.BOOLEAN: 'bool',
    FieldTypes.COLOR32: 'color',
    FieldTypes.FLOAT: 'float',
    FieldTypes.INTEGER: 'int',
    FieldTypes.STRING: 'string',
    FieldTypes.VECTOR: 'vector',
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
    '''Dictionary that stores all datamaps by entity classname'''

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

            # Use "while" to get all datamaps for the entity
            while datamap:

                # Store the class name as a _DataMap dictionary object
                self[datamap.class_name] = _DataMap(datamap)

                # Move up a level and store the base of the current datamap
                self[datamap.class_name].base = datamap = datamap.base

        # Create a dictionary of values to return
        values = dict()

        # Use a "while" statement to get all values for the entity
        while not self[server_class].base is None:

            # Add the current server_class' values to the dictionary
            values.update(self[server_class])

            # Move up one level
            server_class = self[server_class].base.class_name

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
            elif desc.flags & TypeDescriptionFlags.INPUT:

                # Use try/except in case the Pointer is not found
                try:

                    # Get the Function instance for the INPUT
                    function = desc.input.make_function(
                        Convention.THISCALL,
                        (Argument.POINTER, Argument.POINTER),
                        Return.VOID)

                # Was the Pointer not found?
                except ValueError:
                    continue

                # Store the desc name in the dictionary as an input
                value = self[desc.name] = _Input(desc, function)

            # Is this type description supported?
            elif desc.type in _SupportedTypes:

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


class _Input(_BaseType):
    '''Class used to interact with INPUT objects'''

    def __init__(self, desc, function):
        '''Store the type description and function instance'''
        self.desc = desc
        self.function = function

    def __call__(self, value=None, caller=None, activator=None):
        '''Calls the stored function with the values given'''

        # Is the type not VOID but no value was given?
        if value is None and self.desc.type != FieldTypes.VOID:
            raise ValueError(
                'Must provide a value for {0}'.format(self.desc.name))

        # Is the type VOID but a value was given?
        if not value is None and self.desc.type == FieldTypes.VOID:
            raise ValueError(
                '{0} is type Void.  Do not pass a value.'.format(
                    self.desc.name))

        # Is the type supported?
        if (not self.desc.type in _InputSetTypes
                and self.desc.type != FieldTypes.VOID):
            raise TypeError(
                'FieldType "{0}" is unsupported.'.format(self.desc.type))

        # Get an InputData instance
        inputdata = InputData()

        # Does the caller need set?
        if not caller is None:
            inputdata.caller = caller

        # Does the activator need set?
        if not activator is None:
            inputdata.activator = activator

        # Does the function require a value?
        if self.desc.type != FieldTypes.VOID:

            # Set the value
            getattr(inputdata.value, 'set_{0}'.format(
                _InputSetTypes[self.desc.type]))(value)

        # Call the function
        self.function(self.current_pointer, inputdata)


class _DataDesc(object):
    '''Class used to get and set base type description objects'''

    def __init__(self, desc):
        '''Store the type description instance'''
        self.desc = desc

    def _get_value(self):
        '''Return the current value of the type
            description for the current pointer'''
        return getattr(
            self.current_pointer, 'get_{0}'.format(
            _SupportedTypes[self.desc.type]))(self.desc.offset)

    def _set_value(self, value):
        '''Set the value of the type description to
            the given value for the current pointer'''
        getattr(
            self.current_pointer, 'set_{0}'.format(
            _SupportedTypes[self.desc.type]))(value, self.desc.offset)
