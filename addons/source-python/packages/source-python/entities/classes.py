# ../entities/classes.py

"""Provides access to many objects for entities."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Python Imports
#   Warnings
from warnings import warn

# Site-Packages Imports
#   Configobj
from configobj import ConfigObj

# Source.Python Imports
#   Basetypes
from basetypes import SendPropTypes
#   Core
from core import GAME_NAME
from core import SOURCE_ENGINE
#   Engines
from engines.server import server_game_dll
#   Entities
from entities.constants import DATA_DESC_MAP_OFFSET
from entities.datamaps import _supported_input_types
from entities.datamaps import EntityProperty
from entities.datamaps import FieldTypes
from entities.datamaps import InputFunction
from entities.datamaps import TypeDescriptionFlags
from entities.helpers import edict_from_pointer
#   Memory
from memory import Argument
from memory import Convention
from memory import Return
from memory import get_object_pointer
from memory import make_object
from memory.helpers import Type
from memory.manager import CustomType
from memory.manager import TypeManager
#   Paths
from paths import SP_DATA_PATH


# =============================================================================
# >> GLOBAL VARIABLES
# =============================================================================
# Get all of the necessary paths
_managers_engine_path = SP_DATA_PATH.joinpath(
    'entities', 'managers', 'engines', SOURCE_ENGINE)
_managers_game_path = SP_DATA_PATH.joinpath(
    'entities', 'managers', 'games', GAME_NAME)

# Store all supported types
_supported_descriptor_types = {
    FieldTypes.BOOLEAN: 'bool',
    FieldTypes.CHARACTER: 'uchar',
    FieldTypes.CLASSPTR: 'pointer',
    FieldTypes.COLOR32: 'Color',
    FieldTypes.EDICT: 'Edict',
    FieldTypes.EHANDLE: 'int',
    FieldTypes.FLOAT: 'float',
    FieldTypes.FUNCTION: 'pointer',
    FieldTypes.INTEGER: 'int',
    FieldTypes.INTERVAL: 'Interval',
    FieldTypes.MODELINDEX: 'uint',
    FieldTypes.MODELNAME: 'string_pointer',
    FieldTypes.POSITION_VECTOR: 'Vector',
    FieldTypes.QUATERNION: 'Quaternion',
    FieldTypes.SHORT: 'short',
    FieldTypes.SOUNDNAME: 'string_pointer',
    FieldTypes.STRING: 'string_pointer',
    FieldTypes.TICK: 'int',
    FieldTypes.TIME: 'float',
    FieldTypes.VECTOR: 'Vector',
}

# Store all supported keyvalue types
_supported_keyvalue_types = {
    FieldTypes.BOOLEAN: 'bool',
    FieldTypes.CHARACTER: 'int',
    FieldTypes.COLOR32: 'color',
    FieldTypes.EHANDLE: 'int',
    FieldTypes.FLOAT: 'float',
    FieldTypes.INTEGER: 'int',
    FieldTypes.MODELNAME: 'string',
    FieldTypes.SHORT: 'int',
    FieldTypes.SOUNDNAME: 'string',
    FieldTypes.STRING: 'string',
    FieldTypes.TICK: 'int',
    FieldTypes.VECTOR: 'vector',
}

# Store all supported property types
_supported_property_types = {
    SendPropTypes.FLOAT: 'float',
    SendPropTypes.INT: 'int',
    SendPropTypes.STRING: 'string',
    SendPropTypes.VECTOR: 'Vector',
}

# Get a tuple with the supported inputs (including VOID)
_supported_inputs = tuple(_supported_input_types) + (FieldTypes.VOID, )

# Create a dictionary to store all server classes
_server_classes = dict()

# Loop through all server classes and add them to the dictionary
_current_class = server_game_dll.get_all_server_classes()
while _current_class:
    _server_classes[_current_class.name] = _current_class.table
    _current_class = _current_class.next


# =============================================================================
# >> CLASSES
# =============================================================================
class _ServerClasses(TypeManager):

    """Class used to retrieve objects dynamically for a server class."""

    def get_start_server_class(self, server_class, pointer):
        """Retrieve the first server class."""
        # If the given server_class is already in the dictionary,
        #   simply return the given server_class
        if server_class.class_name in self:
            return server_class.class_name

        # Is the engine not supported?
        if DATA_DESC_MAP_OFFSET is None:
            return None

        # Get all server classes
        self._get_all_server_classes(server_class)

        # Return the main server class
        return server_class.class_name

    def _get_all_server_classes(self, datamap):
        """Retrieve values for all server classes and return the first."""
        # Loop through all datamaps to add their objects to the dictionary.
        # Once one is found to be in the dictionary, stop iterating.
        while datamap:

            # If the current classname is in the dictionary, stop iterating
            if datamap.class_name in self:
                break

            # Get the engine specific data for the current datamap
            manager_contents = ConfigObj(
                _managers_engine_path.joinpath(datamap.class_name + '.ini'))

            # Merge the game specific values for the current datamap
            manager_contents.merge(ConfigObj(
                _managers_game_path.joinpath(datamap.class_name + '.ini')))

            # Are there any values for the manager?
            if manager_contents:

                # Add the binary path to the manager dictionary
                manager_contents['binary'] = '{0}/bin/server'.format(GAME_NAME)

            # Get a TypeManager instance for the current datamap
            instance = self.create_type_from_file(
                datamap.class_name, manager_contents)

            # Store the class name to more easily look it up
            instance.__name__ = datamap.class_name

            # Get the specific types of values to use
            input_contents = manager_contents.get('input', {})
            keyvalue_contents = manager_contents.get('keyvalue', {})
            descriptor_contents = manager_contents.get('descriptor', {})
            property_contents = manager_contents.get('property', {})

            # Create dictionaries to store all values for the instance
            instance.inputs = dict()
            instance.functiontables = dict()
            instance.keyvalues = dict()
            instance.outputs = dict()
            instance.properties = dict()

            # Loop through all possible properties for the server class
            for name, prop, offset in self._find_properties(
                    _server_classes.get(datamap.class_name, {})):

                # Add the property to the instance
                self._add_property(
                    instance, name, prop, offset, property_contents,
                    _supported_property_types, True)

            # Loop through all possible descriptors for the server class
            for name, desc, offset in self._find_descriptors(datamap):

                # Is the current descriptor an Output?
                if desc.flags & TypeDescriptionFlags.OUTPUT:

                    # Store the descriptor in the outputs dictionary
                    instance.outputs[name] = desc

                # Is the current descriptor a FunctionTable?
                elif desc.flags & TypeDescriptionFlags.FUNCTIONTABLE:

                    # Store the descriptor in the functiontables dictionary
                    instance.functiontables[name] = desc

                # Is the current descriptor a KeyValue?
                elif desc.flags & TypeDescriptionFlags.KEY:

                    # Add the key value to the instance
                    self._add_keyvalue(instance, name, desc, keyvalue_contents)

                # Is the current descriptor an Input?
                elif desc.flags & TypeDescriptionFlags.INPUT:

                    # Add the input to the instance
                    self._add_input(instance, name, desc, input_contents)

                # Is the current descriptor of a supported type?
                elif desc.type in _supported_descriptor_types:

                    # Add the descriptor to the instance
                    self._add_property(
                        instance, name, desc, offset,
                        descriptor_contents, _supported_descriptor_types)

            # Get a list of all properties for the current server class
            properties = list(instance.properties)

            # Store all of the properties as
            # attributes of an instance of the class
            instance._properties = self(datamap.class_name + 'Properties', (
                CustomType, ), dict(zip(properties, [
                    instance.properties[x].instance for x in properties])))

            # Store all of the inputs as attributes
            # of an instance of the class
            instance._inputs = self(
                datamap.class_name + 'Inputs', (CustomType, ), instance.inputs)

            # Move to the next datamap
            instance._base_class = datamap = datamap.base

    def _find_properties(self, table, base_name='', base_offset=0):
        """Find send props and yield their values."""
        # Loop through all properties of the given table
        for prop in table:

            # Is the property a base class?
            if prop.name == 'baseclass':
                continue

            # Get the name of the property using the given base name
            # Base names are used in case of embedded datatables
            name = base_name + prop.name

            # Get the updated offset of the property
            offset = base_offset + prop.offset

            # Is the current property a datatable?
            if prop.type == SendPropTypes.DATATABLE:

                # Find all properties for the table
                self._find_properties(prop.data_table, name, offset)

            # Is the current property not a datatable?
            else:

                # Yield the current property
                yield (name, prop, offset)

    def _find_descriptors(self, datamap, base_name='', base_offset=0):
        """Find descriptors and yield their values."""
        # Loop through all descriptors in the datamap
        for desc in datamap:

            # Get the name of the descriptor
            name = (desc.name if desc.external_name
                    is None else desc.external_name)

            # Is the name valid?
            if name is None:
                continue

            # Get the name of the descriptor using the given base name
            # Base names are used in case of embedded datamap tables
            name = base_name + name

            # Get the updated offset of the descriptor
            offset = base_offset + desc.offset

            # Is the current descriptor an embedded datamap table?
            if desc.type == FieldTypes.EMBEDDED:

                # Loop through all descriptors for the embedded datamap table
                for name, desc, offset in self._find_descriptors(
                        desc.embedded_datamap, name + '.', offset):

                    # Yield their values
                    yield (name, desc, offset)

            # Is the current descriptor not an embedded datamap table?
            else:

                # Yield the current descriptor
                yield (name, desc, offset)

    def _add_keyvalue(self, instance, name, desc, contents):
        """Add the keyvalue to the instance's keyvalues dictionary."""
        # Is the KeyValue already in the keyvalues dictionary?
        if name in instance.keyvalues:
            return

        # Add the KeyValue to the keyvalues dictionary
        instance.keyvalues[name] = desc

        # Is the KeyValue not a named KeyValue?
        if name not in contents:
            return

        # Is the KeyValue not of a supported type?
        if desc.type not in _supported_keyvalue_types:
            warn('Unsupported KeyValue type "{0}".'.format(desc.type))
            return

        # Add the KeyValue to the instance
        setattr(instance, contents[name], self.keyvalue(
            name, _supported_keyvalue_types[desc.type]))

    def _add_input(self, instance, name, desc, contents):
        """Add the input to the given instance's inputs dictionary."""
        # Is the input already in the inputs dictionary?
        if name in instance.inputs:
            return

        # Add the input to the inputs dictionary
        instance.inputs[name] = self.input(name, desc)

        # Is the input a named input?
        if name in contents:

            # Add the input to the instance
            setattr(instance, contents[name], instance.inputs[name])

    def _add_property(
            self, instance, name, prop, offset,
            contents, types, networked=False):
        """Add the property to the given instance's properties dictionary."""
        # Is the property already in the properties dictionary?
        if name in instance.properties:
            return

        # Get the instance to use to get/set the property
        value = self.instance_attribute(types[prop.type], offset)

        # Add the property to the properties dictionary
        instance.properties[name] = EntityProperty(
            value, types[prop.type], networked)

        # Is the property not a named property?
        if name not in contents:
            return

        # Does the property contain True/False values?
        if not isinstance(contents[name], str):

            # Loop through all attribute names for the property
            for attribute, values in contents[name].items():

                # Get the True/False values
                true_value = int(values['True'])
                false_value = int(values['False'])

                # Add the current attribute name to the instance
                setattr(instance, attribute, self.entity_property(
                    types[prop.type], offset,
                    networked, true_value, false_value))

        # Does the property not contain True/False values?
        else:

            # Add the property to the instance
            setattr(instance, contents[name], self.entity_property(
                types[prop.type], offset, networked))

    @staticmethod
    def keyvalue(name, type_name):
        """Entity keyvalue."""
        def fget(pointer):
            """Retrieve the keyvalue for the entity."""
            return getattr(edict_from_pointer(
                pointer), 'get_key_value_' + type_name)(name)

        def fset(pointer, value):
            """Set the keyvalue for the entity to the given value."""
            getattr(edict_from_pointer(
                pointer), 'set_key_value_' + type_name)(name, value)

        return property(fget, fset)

    @staticmethod
    def input(name, desc):
        """Input type DataMap object."""
        argument_type = desc.type

        def fget(pointer):
            """Retrieve the InputFunction instance."""
            function = desc.input.make_function(
                Convention.THISCALL,
                (Argument.POINTER, Argument.POINTER),
                Return.VOID)

            return InputFunction(name, argument_type, function, pointer)

        return property(fget)

    def entity_property(
            self, type_name, offset, networked,
            true_value=None, false_value=None):
        """Entity property."""
        native_type = Type.is_native(type_name)

        def fget(ptr):
            """Return the propery value."""
            # Is the property a native type?
            if native_type:

                # Retrieve the value
                value = getattr(ptr, 'get_' + type_name)(offset)

            # Is the property not a native type?
            else:

                # Retrieve the value
                value = self.convert(type_name, ptr + offset)

            # Does the property not have True/False values?
            if true_value is None:

                # Return the value
                return value

            # Return whether the value equals the True value
            return value == true_value

        def fset(ptr, value):
            """Set the property value and notify if networked."""
            # Does the property have True/False values?
            if true_value is not None:

                # Get the proper value to set
                value = true_value if value else false_value

            # Is the property a native type?
            if native_type:

                # Set the property
                getattr(ptr, 'set_' + type_name)(value, offset)

            # Is the property not a native type?
            else:

                # Get the class to set the property as
                cls = self.get_class(type_name)

                # Set the property
                get_object_pointer(value).copy(
                    ptr + offset, cls._size)

            # Is the property networked?
            if networked:

                # Notify the change of state
                edict_from_pointer(ptr).state_changed()

        return property(fget, fset)

server_classes = _ServerClasses()
