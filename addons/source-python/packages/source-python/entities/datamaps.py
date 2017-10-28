# ../entities/datamaps.py

"""Provides DataMap functionality for entities."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Python Imports
#   Enum
from enum import IntFlag

# Source.Python Imports
#   Memory
from memory import Function


# =============================================================================
# >> FORWARD IMPORTS
# =============================================================================
# Source.Python Imports
#   Entities
from _entities._datamaps import DataMap
from _entities._datamaps import FieldType
from _entities._datamaps import FTYPEDESC_GLOBAL
from _entities._datamaps import FTYPEDESC_SAVE
from _entities._datamaps import FTYPEDESC_KEY
from _entities._datamaps import FTYPEDESC_INPUT
from _entities._datamaps import FTYPEDESC_OUTPUT
from _entities._datamaps import FTYPEDESC_FUNCTIONTABLE
from _entities._datamaps import FTYPEDESC_PTR
from _entities._datamaps import FTYPEDESC_OVERRIDE
from _entities._datamaps import FTYPEDESC_INSENDTABLE
from _entities._datamaps import FTYPEDESC_PRIVATE
from _entities._datamaps import FTYPEDESC_NOERRORCHECK
from _entities._datamaps import FTYPEDESC_MODELINDEX
from _entities._datamaps import FTYPEDESC_INDEX
from _entities._datamaps import FTYPEDESC_VIEW_OTHER_PLAYER
from _entities._datamaps import FTYPEDESC_VIEW_OWN_TEAM
from _entities._datamaps import FTYPEDESC_VIEW_NEVER
from _entities._datamaps import InputData
from _entities._datamaps import Interval
from _entities._datamaps import TypeDescription
from _entities._datamaps import Variant


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
# Set all to an empty list
__all__ = ('DataMap',
           'FieldType',
           'InputData',
           'Interval',
           'TypeDescription',
           'TypeDescriptionFlags',
           'Variant',
           )


# =============================================================================
# >> GLOBAL VARIABLES
# =============================================================================
_supported_input_types = {
    FieldType.BOOLEAN: 'bool',
    FieldType.COLOR32: 'color',
    FieldType.FLOAT: 'float',
    FieldType.INTEGER: 'int',
    FieldType.STRING: 'string',
    FieldType.VECTOR: 'vector',
}


# =============================================================================
# >> ENUMERATORS
# =============================================================================
class TypeDescriptionFlags(IntFlag):
    """TypeDescription flags wrapper enumerator."""

    GLOBAL = FTYPEDESC_GLOBAL
    SAVE = FTYPEDESC_SAVE
    KEY = FTYPEDESC_KEY
    INPUT = FTYPEDESC_INPUT
    OUTPUT = FTYPEDESC_OUTPUT
    FUNCTIONTABLE = FTYPEDESC_FUNCTIONTABLE
    PTR = FTYPEDESC_PTR
    OVERRIDE = FTYPEDESC_OVERRIDE
    INSENDTABLE = FTYPEDESC_INSENDTABLE
    PRIVATE = FTYPEDESC_PRIVATE
    NOERRORCHECK = FTYPEDESC_NOERRORCHECK
    MODELINDEX = FTYPEDESC_MODELINDEX
    INDEX = FTYPEDESC_INDEX
    VIEW_OTHER_PLAYER = FTYPEDESC_VIEW_OTHER_PLAYER
    VIEW_OWN_TEAM = FTYPEDESC_VIEW_OWN_TEAM
    VIEW_NEVER = FTYPEDESC_VIEW_NEVER


# =============================================================================
# >> CLASSES
# =============================================================================
class EntityProperty(object):
    """Class used to store property information for verification."""

    def __init__(self, instance, prop_type, networked):
        """Store the base attributes on instantiation."""
        self._instance = instance
        self._prop_type = prop_type
        self._networked = networked

    @property
    def instance(self):
        """Return the instance_property instance for the class."""
        return self._instance

    @property
    def prop_type(self):
        """Return the type of the property."""
        return self._prop_type

    @property
    def networked(self):
        """Return whether the property is networked."""
        return self._networked


class InputFunction(Function):
    """Class used to create and call an Input type function."""

    def __init__(self, name, argument_type, function, this):
        """Instantiate the function instance and store the base attributes."""
        super().__init__(function)

        self._name = name
        self._argument_type = argument_type
        self._this = this

    def __call__(self, value=None, caller=None, activator=None):
        """Call the stored function with the values given."""
        # Is the type not VOID but no value was given?
        if value is None and self._argument_type != FieldType.VOID:
            raise ValueError(
                'Must provide a value for {0}'.format(self._name))

        # Is the type VOID but a value was given?
        if value is not None and self._argument_type == FieldType.VOID:
            raise ValueError(
                '{0} is type Void.  Do not pass a value.'.format(
                    self._name))

        # Get an InputData instance
        inputdata = InputData()

        # Does the caller need set?
        if caller is not None:
            inputdata.caller = caller

        # Does the activator need set?
        if activator is not None:
            inputdata.activator = activator

        # Does the function require a value?
        if self._argument_type != FieldType.VOID:

            # Set the value
            getattr(
                inputdata.value,
                'set_{0}'.format(_supported_input_types[
                    self._argument_type]))(value)

        # Call the function
        super().__call__(self._this, inputdata)
