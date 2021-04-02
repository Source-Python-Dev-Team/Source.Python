# ../entities/classes.py

"""Provides access to many objects for entities."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Python Imports
#   Collections
from collections import OrderedDict
from collections import defaultdict
#   Warnings
from warnings import warn

# Site Package Imports
#   Configobj
from configobj import Section

# Source.Python Imports
#   Core
from core import GameConfigObj
from core import PLATFORM
#   Entities
from _entities._entity import BaseEntity
from entities import ServerClassGenerator
from entities.datamaps import _supported_input_types
from entities.datamaps import EntityProperty
from entities.datamaps import FieldType
from entities.datamaps import InputFunction
from entities.datamaps import TypeDescriptionFlags
from entities.helpers import edict_from_pointer
from entities.helpers import baseentity_from_pointer
from entities.props import SendPropFlags
from entities.props import SendPropType
#   Memory
from memory import Convention
from memory import DataType
from memory import get_object_pointer
from memory import make_object
from memory.helpers import Key
from memory.helpers import Type
from memory.manager import CustomType
from memory.manager import TypeManager
#   Paths
from paths import SP_DATA_PATH


# =============================================================================
# >> GLOBAL VARIABLES
# =============================================================================
# Get all of the necessary paths
_managers_path = SP_DATA_PATH / 'entities'

# Store all supported types
_supported_descriptor_types = {
    FieldType.BOOLEAN: 'bool',
    FieldType.CHARACTER: 'uchar',
    FieldType.CLASSPTR: 'pointer',
    FieldType.COLOR32: 'Color',
    FieldType.EDICT: 'Edict',
    FieldType.EHANDLE: 'int',
    FieldType.FLOAT: 'float',
    FieldType.FUNCTION: 'pointer',
    FieldType.INTEGER: 'int',
    FieldType.INTERVAL: 'Interval',
    FieldType.MODELINDEX: 'uint',
    FieldType.MODELNAME: 'string_pointer',
    FieldType.POSITION_VECTOR: 'Vector',
    FieldType.QUATERNION: 'Quaternion',
    FieldType.SHORT: 'short',
    FieldType.SOUNDNAME: 'string_pointer',
    FieldType.STRING: 'string_pointer',
    FieldType.TICK: 'int',
    FieldType.TIME: 'float',
    FieldType.VECTOR: 'Vector',
}

# Store all supported keyvalue types
_supported_keyvalue_types = {
    FieldType.BOOLEAN: 'bool',
    FieldType.CHARACTER: 'int',
    FieldType.COLOR32: 'color',
    FieldType.EHANDLE: 'int',
    FieldType.FLOAT: 'float',
    FieldType.INTEGER: 'int',
    FieldType.MODELNAME: 'string',
    FieldType.POSITION_VECTOR: 'vector',
    FieldType.SHORT: 'int',
    FieldType.SOUNDNAME: 'string',
    FieldType.STRING: 'string',
    FieldType.TICK: 'int',
    FieldType.TIME: 'float',
    FieldType.VECTOR: 'vector',
}

# Store all supported property types
_supported_property_types = {
    SendPropType.FLOAT: 'float',
    SendPropType.INT: 'int',
    SendPropType.STRING: 'string_array',
    SendPropType.VECTOR: 'Vector',
}

# Get a tuple with the supported inputs (including VOID)
_supported_inputs = tuple(_supported_input_types) + (FieldType.VOID, )

# Create a dictionary to store all server classes
_server_classes = dict()

# Loop through all server classes and add them to the dictionaries
_table_names = dict()
for _server_class in ServerClassGenerator():
    _server_classes[_server_class.name] = _server_class.table
    _table_names[_server_class.table.name] = _server_class.name


# =============================================================================
# >> CLASSES
# =============================================================================
class _ServerClasses(TypeManager):
    """Class used to retrieve objects dynamically for a server class."""

    def __init__(self):
        """Store the base attributes."""
        super().__init__()
        self._entity_server_classes = defaultdict(list)

    def get_entity_server_classes(self, entity):
        """Return the entity's server classes.

        :param BaseEntity entity:
            The entity whose server classes should be retrieved.
        :return:
            A list containing :class:`memory.manager.CustomType` instances
            that have been created based on the entity files stored in
            :data:`paths.SP_DATA_PATH`.
        :rtype: list
        """
        # Is the entity type already stored?
        if entity.classname in self._entity_server_classes:

            # Return the server classes
            return self._entity_server_classes[entity.classname]

        # Create a list to store the entity's ServerClasses
        entity_server_classes = [entity.server_class.name]

        # Loop through all ServerClasses for the entity
        for name in self._get_base_server_classes(entity.server_class.table):

            # Add the ServerClass to the list
            entity_server_classes.append(name)

        # Create a dictionary to store datamaps in for the entity
        entity_datamaps = OrderedDict()

        # Get the DataMap object for the entity
        datamap = entity.datamap

        # Add all DataMaps for the entity to the dictionary
        while datamap:
            entity_datamaps[datamap.class_name] = datamap
            datamap = datamap.base

        # Find the differences
        server_no_data = set(entity_server_classes).difference(entity_datamaps)
        data_no_server = set(entity_datamaps).difference(entity_server_classes)

        # Set the order for the classes to be stored/iterated
        if not data_no_server:
            order = entity_server_classes
        elif not server_no_data:
            order = entity_datamaps
        else:
            order = []
            for class_name in list(entity_datamaps):
                if class_name in entity_server_classes:
                    index = entity_server_classes.index(class_name)
                    order.extend(entity_server_classes[:index + 1])
                    del entity_server_classes[:index + 1]
                else:
                    order.append(class_name)
            order.extend(entity_server_classes)
            last_key = 'CBaseEntity'
            if last_key in order and order[~0] != last_key:
                order.remove(last_key)
                order.append(last_key)

        for class_name in order:
            # Is the current class already known?
            if class_name in self:
                self._entity_server_classes[entity.classname].append(
                    self[class_name]
                )
                continue

            # Add the class
            self._entity_server_classes[entity.classname].append(
                self._get_server_class(
                    class_name=class_name,
                    datamap=entity_datamaps.get(class_name, {}),
                )
            )

        # Return the server classes
        return self._entity_server_classes[entity.classname]

    def _get_base_server_classes(self, table):
        """Yield all baseclasses within the table."""
        # Loop through all of the props in the table
        for prop in table:

            # Is the current prop not a baseclass?
            if prop.name != 'baseclass':
                continue

            # Yield the current baseclass
            yield _table_names[prop.data_table.name]

            # Yield all tables within the baseclass
            yield from self._get_base_server_classes(prop.data_table)

    def _get_server_class(self, class_name, datamap):
        """Retrieve values for the server class."""
        # Get the engine specific data for the current class
        manager_contents = GameConfigObj(_managers_path / class_name + '.ini')

        # Are there any values for the manager?
        if manager_contents:

            # Add the binary path to the manager dictionary
            manager_contents['binary'] = 'server'

        # Get a TypeManager instance for the current datamap
        instance = self.create_type_from_dict(class_name, manager_contents)

        # Get the specific types of values to use
        input_contents = dict(map(
            reversed, manager_contents.get('input', {}).items()))
        property_contents = {}
        for item, value in manager_contents.get('property', {}).items():
            if isinstance(value, Section):
                property_contents[value['name']] = (item, value['type'])
            else:
                property_contents[value] = item
        keyvalue_contents = {}
        hardcoded_keyvalues = {}
        for item, value in manager_contents.get('keyvalue', {}).items():
            if isinstance(value, Section):
                hardcoded_keyvalues[item] = value
                hardcoded_keyvalues[item].update({'alias': item})
            else:
                keyvalue_contents[value] = item

        # Create dictionaries to store all values for the instance
        instance.inputs = dict()
        instance.functiontables = dict()
        instance.keyvalues = dict()
        instance.outputs = dict()
        instance.properties = dict()

        # Loop through all possible properties for the server class
        for name, prop, offset in self._find_properties(
                _server_classes.get(class_name, {})):

            if prop.type not in _supported_property_types:
                continue

            prop_type = _supported_property_types[prop.type]

            if prop.type == SendPropType.INT:
                bit_count = prop.bits
                if bit_count < 1:
                    # Note: I have yet to encounter this, so I'm not
                    #   sure under what circumstances this can occur.
                    # That is why this simply continues.
                    continue
                if bit_count >= 17:
                    prop_type = 'int'
                elif bit_count >= 9:
                    prop_type = '{0}short'.format(
                        '' if prop.is_signed() else 'u')
                elif bit_count >= 2:
                    prop_type = '{0}char'.format(
                        '' if prop.is_signed() else 'u')
                else:
                    prop_type = 'bool'

            # Add the property to the instance
            self._add_property(
                instance, name, offset, property_contents, prop_type, True)

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

                # Is the key value also a valid property?
                if desc.name and desc.type in _supported_descriptor_types:

                    # Add the descriptor to the instance
                    self._add_property(
                        instance, desc.name, offset, property_contents,
                        _supported_descriptor_types[desc.type])

            # Is the current descriptor an Input?
            elif desc.flags & TypeDescriptionFlags.INPUT:

                # Add the input to the instance
                self._add_input(instance, name, desc, input_contents)

            # Is the current descriptor of a supported type?
            elif desc.type in _supported_descriptor_types:

                # Add the descriptor to the instance
                self._add_property(
                    instance, name, offset, property_contents,
                    _supported_descriptor_types[desc.type])

        # Loop through all based attributes
        for name, data in manager_contents.get('based_attribute', {}).items():

            # Resolve the method to register this attribute
            method = getattr(self, data.get('method', 'instance_attribute'))

            # Resolve the offset of this attribute
            offset = Key.as_int(
                self,
                data.get('offset_' + PLATFORM, data.get('offset', 0))
            )

            # Resolve the base offset of this attribute
            base = data.get('base_' + PLATFORM, data.get('base'))
            try:
                offset += instance.properties[base].offset
            except KeyError:
                raise NameError(
                    f'"{base}" is not a valid property ' +
                    f'for attribute "{class_name}.{name}".'
                )

            # Generate the attribute
            attribute = method(
                Key.as_attribute_type(self, data['type']),
                offset,
                data.get('doc')
            )

            # Assign the attribute to the instance
            setattr(instance, name, attribute)

        # Get a list of all properties for the current server class
        properties = list(instance.properties)

        # Store all of the properties as
        # attributes of an instance of the class
        instance._properties = self(class_name + 'Properties', (
            CustomType, ), dict(zip(properties, [
                instance.properties[x].instance for x in properties])))

        # Store all of the inputs as attributes
        # of an instance of the class
        instance._inputs = self(
            class_name + 'Inputs', (CustomType, ), dict(instance.inputs))

        # Loop through all hard coded keyvalues
        for item, value in hardcoded_keyvalues.items():

            # Get the keyvalue's name and type
            name = value['name']
            keyvalue_type = getattr(FieldType, value['type'])

            # Is the keyvalue already added to the instance?
            if name in instance.keyvalues:
                warn('KeyValue "{0}" already implemented.'.format(name))
                continue

            # Is the type unsupported?
            if keyvalue_type not in _supported_keyvalue_types:
                warn('Unsupported KeyValue type "{0}".'.format(keyvalue_type))
                continue

            # Add the keyvalue to the instance
            instance.keyvalues[name] = value
            setattr(instance, value['alias'], self.keyvalue(
                name, _supported_keyvalue_types[keyvalue_type]))

        # Return the ServerClass
        return instance

    def _find_properties(self, table, base_name='', base_offset=0):
        """Find send props and yield their values."""
        # Loop through all properties of the given table
        for prop in table:

            # Is the property a base class or excluded?
            if (prop.name == 'baseclass' or
                prop.is_exclude_prop() or
                prop.flags & SendPropFlags.COLLAPSIBLE
            ):
                continue

            # Get the name of the property using the given base name
            # Base names are used in case of embedded datatables
            name = base_name + prop.name

            # Get the updated offset of the property
            offset = base_offset + prop.offset

            # Is the current property a datatable?
            if prop.type == SendPropType.DATATABLE:
                yield from self._find_properties(
                    prop.data_table, name + '.', offset)

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
            if desc.type == FieldType.EMBEDDED:
                yield from self._find_descriptors(
                    desc.embedded_datamap, name + '.', offset)

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
        instance.inputs[name] = self.input(desc)

        # Is the input a named input?
        if name in contents:

            # Add the input to the instance
            setattr(instance, contents[name], instance.inputs[name])

    def _add_property(
            self, instance, name, offset, contents,
            prop_type, networked=False):
        """Add the property to the given instance's properties dictionary."""
        # Is the property already in the properties dictionary?
        if name in instance.properties:
            return

        # Is the offset not 0?
        if not offset:
            return

        # Get the instance to use to get/set the property
        value = self.instance_attribute(prop_type, offset)

        # Add the property to the properties dictionary
        instance.properties[name] = EntityProperty(
            value, prop_type, networked, offset)

        # Is the property not a named property?
        if name not in contents:
            return

        # Is this a property that needs transformed?
        if isinstance(contents[name], tuple):

            # Add the property to the instance
            setattr(instance, contents[name][0], self.entity_property(
                contents[name][1], offset, networked))

        # Is the property normal?
        else:

            # Add the property to the instance
            setattr(instance, contents[name], self.entity_property(
                prop_type, offset, networked))

    @staticmethod
    def keyvalue(name, type_name):
        """Entity keyvalue."""
        def fget(pointer):
            """Retrieve the keyvalue for the entity."""
            return getattr(baseentity_from_pointer(
                pointer), 'get_key_value_' + type_name)(name)

        def fset(pointer, value):
            """Set the keyvalue for the entity to the given value."""
            getattr(baseentity_from_pointer(
                pointer), 'set_key_value_' + type_name)(name, value)

        return property(fget, fset)

    @staticmethod
    def input(desc):
        """Input type DataMap object."""
        def fget(pointer):
            """Retrieve the InputFunction instance."""
            func = desc.function

            # Handle virtual inputs on Linux
            if PLATFORM == 'linux' and func.address & 1:
                func = pointer.get_virtual_func((func.address - 1) // 4)

            # TODO:
            # Don't use make_function(), but use input_function directly. It's
            # already callable.
            function = func.make_function(
                Convention.THISCALL,
                (DataType.POINTER, DataType.POINTER),
                DataType.VOID)

            return InputFunction(desc, make_object(BaseEntity, pointer))

        return property(fget)

    def entity_property(self, type_name, offset, networked):
        """Entity property."""
        native_type = Type.is_native(type_name)

        def fget(ptr):
            """Return the property value."""
            # Is the property a native type?
            if native_type:

                value = getattr(ptr, 'get_' + type_name)(offset)

                # Does the value need cast to an integer?
                if type_name == 'char':
                    return ord(value)
                return value

            # Return the value
            return self.convert(type_name, ptr + offset)

        def fset(ptr, value):
            """Set the property value and notify if networked."""
            # Is the property a native type?
            if native_type:

                # Does the value need cast to a string?
                if type_name == 'char':
                    value = chr(value)

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

# Get the _ServerClasses instance
server_classes = _ServerClasses()
