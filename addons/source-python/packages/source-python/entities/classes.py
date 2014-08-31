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
from basetypes import Color
from basetypes import Interval
#   Core
from core import GAME_NAME
from core import PLATFORM
#   Entities
from entities import Edict
from entities.constants import DATA_DESC_MAP_OFFSET
from entities.datamaps import _supported_input_types
from entities.datamaps import DataMap
from entities.datamaps import FieldTypes
from entities.datamaps import InputFunction
from entities.datamaps import OutputFunction
from entities.datamaps import TypeDescriptionFlags
from entities.helpers import edict_from_pointer
#   Mathlib
from mathlib import Quaternion
from mathlib import Vector
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
_managers_path = SP_DATA_PATH.joinpath('entities', 'managers', GAME_NAME)
_offsets_path = SP_DATA_PATH.joinpath('entities', 'offsets', GAME_NAME)
_virtuals_path = SP_DATA_PATH.joinpath('virtuals', GAME_NAME)
_properties_path = SP_DATA_PATH.joinpath('entities', 'properties', GAME_NAME)

_supported_offset_types = {
    FieldTypes.BOOLEAN: 'bool',
    FieldTypes.CHARACTER: 'char',
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


# =============================================================================
# >> CLASSES
# =============================================================================
class _ServerClasses(TypeManager):

    """Class used to retrieve objects dynamically for a server class."""

    def get_start_server_class(self, server_class, pointer):
        """Retrieve the first server class and build any classes."""
        if server_class in self:
            return server_class
        function = pointer.make_virtual_function(
            DATA_DESC_MAP_OFFSET, Convention.THISCALL,
            (Argument.POINTER, ), Return.POINTER)
        server_class = datamap = make_object(DataMap, function(pointer))
        if datamap.class_name in self:
            return datamap.class_name
        while datamap:
            if datamap.class_name in self:
                break
            manager_contents = ConfigObj(
                _managers_path.joinpath(datamap.class_name + '.ini'))
            virtual_contents = ConfigObj(
                _virtuals_path.joinpath(datamap.class_name + '.ini'))
            offset_contents = ConfigObj(
                _offsets_path.joinpath(datamap.class_name + '.ini'))
            property_contents = ConfigObj(
                _properties_path.joinpath(datamap.class_name + '.ini'))
            if 'virtual_function' in manager_contents:
                for item in manager_contents['virtual_function']:
                    value = manager_contents['virtual_function'][item]
                    value['offset_{0}'.format(
                        PLATFORM)] = virtual_contents[value['name']][PLATFORM]

            instance = self.create_type_from_file(
                datamap.class_name, manager_contents)

            descriptors = dict()
            inputs = dict()
            outputs = dict()
            instance._keyvalues = dict()

            for desc in datamap:

                if desc.name is None:
                    continue

                name = (desc.name if desc.external_name
                        is None else desc.external_name)

                if hasattr(instance, name):
                    warn(
                        '{0} object already contains the attribute {1}'.format(
                            instance.__class__.__name__, name))
                    continue

                if desc.type == FieldTypes.EMBEDDED:

                    if name in descriptors:
                        warn()
                        continue
                    for embedded_name, embedded_type, offset in (
                            self._find_embedded_descriptors(
                                name, desc.embedded_datamap, desc.offset)):

                        descriptors[embedded_name] = self.instance_attribute(
                            _supported_offset_types[embedded_type], offset)
                        if (embedded_name in offset_contents
                                and embedded_type in _supported_offset_types):
                            setattr(
                                instance,
                                offset_contents[embedded_name],
                                descriptors[embedded_name])

                elif (desc.flags & TypeDescriptionFlags.INPUT
                        and not desc.flags & TypeDescriptionFlags.KEY
                        and desc.type in _supported_input_types):

                    if name in inputs:
                        warn()
                        continue

                    inputs[name] = self.input(name, desc)

                # Check types and test
                elif desc.flags & TypeDescriptionFlags.OUTPUT:

                    if name in outputs:
                        warn()
                        continue

                    outputs[name] = self.output(desc.offset)

                elif desc.flags & TypeDescriptionFlags.KEY:

                    if name in instance._keyvalues:
                        warn()
                        continue

                    instance._keyvalues[name] = desc

                elif desc.type in _supported_offset_types:

                    descriptors[name] = self.instance_attribute(
                        _supported_offset_types[desc.type], desc.offset)
                    if name in offset_contents:
                        setattr(
                            instance, offset_contents[name], descriptors[name])

            instance._descriptors = self(
                datamap.class_name + 'Descriptors',
                (CustomType, ), descriptors)

            instance._inputs = self(
                datamap.class_name + 'Inputs', (CustomType, ), inputs)

            instance._outputs = self(
                datamap.class_name + 'Outputs', (CustomType, ), outputs)

            for property_name in property_contents:

                if hasattr(instance, property_name):

                    warn()
                    continue

                property_values = property_contents[property_name]

                setattr(
                    instance, property_name,
                    self.instance_property(
                        property_values['prop'],
                        property_values['type'],
                        property_values.get('True', None),
                        property_values.get('False', None)))

            instance._base_class = datamap = datamap.base

        return server_class.class_name

    def input(self, name, desc):
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

    def output(self, offset):
        """Output type DataMap object."""
        def fget(pointer):
            """Retrieve the OutputFunction instance."""
            function = pointer.make_virtual_function(
                offset, Convention.THISCALL,
                (Argument.POINTER, Argument.POINTER), Return.VOID)

            return OutputFunction(function)

        return property(fget)

    def instance_property(self, name, type_name, true_value, false_value):
        """Networked property."""
        def fget(pointer):
            """Retrive the network property for the entity."""
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
        for desc in datamap:
            # Test this as it might not be necessary
            name = basename + '.' + (
                desc.name if desc.external_name
                is None else desc.external_name)
            offset = embedded_offset + desc.offset
            if desc.type == FieldTypes.EMBEDDED:
                self._find_embedded_descriptors(
                    name, desc.embedded_datamap, offset)
            elif desc.type in _supported_offset_types:
                yield name, desc.type, offset

_server_classes = _ServerClasses()
