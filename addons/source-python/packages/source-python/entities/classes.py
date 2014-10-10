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
#   Core
from core import GAME_NAME
from core import SOURCE_ENGINE
#   Entities
from entities.constants import DATA_DESC_MAP_OFFSET
from entities.datamaps import _supported_input_types
from entities.datamaps import DataMap
from entities.datamaps import FieldTypes
from entities.datamaps import InputFunction
from entities.datamaps import TypeDescriptionFlags
from entities.helpers import edict_from_pointer
#   Memory
from memory import Argument
from memory import Convention
from memory import Return
from memory import make_object
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
_supported_offset_types = {
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
    # FieldTypes.BOOLEAN: '',
    # FieldTypes.CHARACTER: '',
    # FieldTypes.COLOR32: '',
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

# Get a tuple with the supported inputs (including VOID)
_supported_inputs = tuple(_supported_input_types) + (FieldTypes.VOID, )


# =============================================================================
# >> CLASSES
# =============================================================================
class _ServerClasses(TypeManager):

    """Class used to retrieve objects dynamically for a server class."""

    def get_start_server_class(self, server_class, pointer):
        """Retrieve the first server class and build any classes."""
        # If the given server_class is already in the dictionary,
        #   simply return the given server_class
        if server_class in self:
            return server_class

        # Is the engine not supported?
        if DATA_DESC_MAP_OFFSET is None:
            return None

        # Create a function for the <server_class>::GetDataDescMap method
        function = pointer.make_virtual_function(
            DATA_DESC_MAP_OFFSET, Convention.THISCALL,
            (Argument.POINTER, ), Return.POINTER)
        server_class = datamap = make_object(DataMap, function(pointer))

        # If the current classname is in the dictionary, simply return it
        if datamap.class_name in self:
            return datamap.class_name

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

            # Get the specific types of values to use
            input_contents = manager_contents.get('inputs', {})
            keyvalue_contents  = manager_contents.get('keyvalues', {})
            descriptor_contents = manager_contents.get('descriptors', {})
            property_contents = manager_contents.get('properties', {})

            # Are there any values for the manager?
            if manager_contents:

                # Add the binary path to the manager dictionary
                manager_contents['binary'] = '{0}/bin/server'.format(GAME_NAME)

            # Get a TypeManager instance for the current datamap
            instance = self.create_type_from_file(
                datamap.class_name, manager_contents)

            # Create dictionaries to hold specific
            #    information based on the datamap.
            descriptors = dict()
            inputs = dict()
            instance._outputs = dict()
            instance._functiontables = dict()
            instance._keyvalues = dict()

            # Set the instance's name to the classname
            instance.__name__ = datamap.class_name

            # Loop through all descriptors in the current datamap
            for desc in datamap:

                # If the name doesn't exist, move onto the next one
                if desc.name is None:
                    continue

                # Get the name to use.
                # If the external name is set, use it, otherwise use the name.
                name = (desc.name if desc.external_name
                        is None else desc.external_name)

                # Is the current name already
                # registered for the current datamap?
                if hasattr(instance, name):
                    warn(
                        '{0} object already contains the attribute {1}'.format(
                            instance.__class__.__name__, name))
                    continue

                # Is this an embedded descriptor?
                if desc.type == FieldTypes.EMBEDDED:

                    # Loop through all embedded
                    #   descriptors within this embedded.
                    for embedded_name, embedded_type, offset in (
                            self._find_embedded_descriptors(
                                name, desc.embedded_datamap, desc.offset)):

                        # If the name is already registered, move on
                        if name in descriptors:
                            warn(
                                'Embedded descriptor "{0}" is '.format(name) +
                                'already registered for class "{0}"'.format(
                                    datamap.class_name))
                            continue

                        # Add the embedded to the descriptors dictionary
                        descriptors[embedded_name] = self.instance_attribute(
                            _supported_offset_types[embedded_type], offset)

                        # Is the embedded a named descriptor?
                        if (embedded_name in descriptor_contents
                                and embedded_type in _supported_offset_types):

                            # Add the descriptor to the instance by name
                            setattr(
                                instance,
                                descriptor_contents[embedded_name],
                                descriptors[embedded_name])

                # Is the current descriptor an input?
                elif (desc.flags & TypeDescriptionFlags.INPUT
                        and not desc.flags & TypeDescriptionFlags.KEY
                        and desc.type in _supported_inputs):

                    # If the input is already registered, move on
                    if name in inputs:
                        warn(
                            'Input "{0}" is '.format(name) +
                            'already registered for class "{0}"'.format(
                                datamap.class_name))
                        continue

                    # Add the descriptor to the inputs dictionary
                    inputs[name] = self.input(name, desc)

                    # Is this a named input?
                    if name in input_contents:

                        # Add the input to the instance by name
                        setattr(instance, input_contents[name], inputs[name])

                # Is the current descriptor an output?
                elif desc.flags & TypeDescriptionFlags.OUTPUT:

                    # Add the descriptor to the outputs dictionary
                    instance._outputs[name] = desc

                # Is the current descriptor a functiontable?
                elif desc.flags & TypeDescriptionFlags.FUNCTIONTABLE:

                    # Add the descriptor to the functiontable dictionary
                    instance._functiontables[name] = desc

                # Is the current descriptor a keyvalue?
                elif desc.flags & TypeDescriptionFlags.KEY:

                    # If the keyvalue is already registered, move on
                    if name in instance._keyvalues:
                        warn(
                            'KeyValue "{0}" is '.format(name) +
                            'already registered for class "{0}"'.format(
                                datamap.class_name))
                        continue

                    # Add the keyvalue to the keyvalues dictionary
                    instance._keyvalues[name] = desc

                    # Is this a named keyvalue?
                    if name in keyvalue_contents:

                        # Is the keyvalue's type not supported?
                        if desc.type not in _supported_keyvalue_types:
                            warn(
                                'Named KeyValue "{0}" '.format(name) +
                                'is of an unsupported type:' +
                                ' ({0}).'.format(desc.type))
                            continue

                        # Add the keyvalue to the instance by name
                        setattr(
                            instance, keyvalue_contents[name], self.keyvalue(
                                name, _supported_keyvalue_types[desc.type]))

                # Is the current descriptor supported?
                elif desc.type in _supported_offset_types:

                    # If the name is already registered, move on
                    if name in descriptors:
                        warn(
                            'Descriptor "{0}" is '.format(name) +
                            'already registered for class "{0}"'.format(
                                datamap.class_name))
                        continue

                    # Add the descriptor to the descriptors dictionary
                    descriptors[name] = self.instance_attribute(
                        _supported_offset_types[desc.type], desc.offset)

                    # Is this a named descriptor?
                    if name in descriptor_contents:

                        # Add the descriptor to the instance by name
                        setattr(
                            instance, descriptor_contents[name],
                            descriptors[name])

            # Store the descriptors and inputs with the instance
            instance._descriptors = self(
                datamap.class_name + 'Descriptors',
                (CustomType, ), descriptors)
            instance._inputs = self(
                datamap.class_name + 'Inputs', (CustomType, ), inputs)

            # Loop through all properties to add to the instance
            for property_name in property_contents:

                # If the current property name is already registered, move on
                if hasattr(instance, property_name):
                    warn(
                        'Property "{0}" is '.format(name) +
                        'already registered for class "{0}"'.format(
                            datamap.class_name))
                    continue

                # Get the properties values
                property_values = property_contents[property_name]

                # Add the property to the instance
                setattr(
                    instance, property_name,
                    self.network_property(
                        property_values['prop'],
                        property_values['type'],
                        property_values.get('True', None),
                        property_values.get('False', None)))

            # Get the next server class
            instance._base_class = datamap = datamap.base

        # Return the main server class name
        return server_class.class_name

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
    def network_property(name, type_name, true_value, false_value):
        """Networked property."""
        # If there are true/false values, typecast them to integer
        if true_value is not None:
            true_value = int(true_value)
            false_value = int(false_value)

        def fget(pointer):
            """Retrieve the network property for the entity."""
            value = getattr(
                edict_from_pointer(pointer), 'get_prop_' + type_name)(name)
            if true_value is None:

                return value

            return value == true_value

        def fset(pointer, value):
            """Set the network property for the entity to the given value."""
            if true_value is not None:

                value = true_value if value else false_value

            getattr(
                edict_from_pointer(pointer),
                'set_prop_' + type_name)(name, value)

        return property(fget, fset)

    def _find_embedded_descriptors(self, basename, datamap, embedded_offset):
        """Search for embedded descriptors to be added to the server class."""
        # Loop through each descriptor in the embedded
        for desc in datamap:

            # Get the name to use for the embedded
            name = basename + '.' + desc.name

            # Get the current offset
            offset = embedded_offset + desc.offset

            # If the current descriptor is another
            #   embedded, loop through its descriptors.
            if desc.type == FieldTypes.EMBEDDED:
                self._find_embedded_descriptors(
                    name, desc.embedded_datamap, offset)

            # If the current descriptor is supported, yield its values
            elif desc.type in _supported_offset_types:
                yield name, desc.type, offset

# Get the _ServerClasses instance
_server_classes = _ServerClasses()
