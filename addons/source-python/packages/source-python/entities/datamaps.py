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
from _entities._datamaps import InputFunction
from _entities._datamaps import Interval
from _entities._datamaps import TypeDescription
from _entities._datamaps import Variant


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
# Set all to an empty list
__all__ = ('DataMap',
           'EntityProperty',
           'FieldType',
           'InputData',
           'InputFunction',
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

    def __init__(self, instance, prop_type, networked, offset):
        """Store the base attributes on instantiation."""
        self._instance = instance
        self._prop_type = prop_type
        self._networked = networked
        self._offset = offset

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

    @property
    def offset(self):
        """Returns the offset of the property."""
        return self._offset

