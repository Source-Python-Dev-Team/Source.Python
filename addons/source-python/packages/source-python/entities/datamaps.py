# ../entities/datamaps.py

"""Provides DataMap functionality for entities."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
from _datamaps import DataMap
from _datamaps import FieldTypes
from _datamaps import InputData
from _datamaps import TypeDescription
from _datamaps import TypeDescriptionFlags
from _datamaps import Variant
from memory import Function


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
# Set all to an empty list
__all__ = ('DataMap',
           'FieldTypes',
           'InputData',
           'TypeDescription',
           'TypeDescriptionFlags',
           'Variant',
           )


# =============================================================================
# >> GLOBAL VARIABLES
# =============================================================================
_supported_input_types = {
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
class InputFunction(Function):

    """Class used to create and call an Input type function."""

    def __init__(self, name, argument_type, function, this):
        """Instantiate the function instance and store the base attributes."""
        super(InputFunction, self).__init__(function)

        self._name = name
        self._argument_type = argument_type
        self._this = this

    def __call__(self, value=None, caller=None, activator=None):
        """Call the stored function with the values given."""
        # Is the type not VOID but no value was given?
        if value is None and self._argument_type != FieldTypes.VOID:
            raise ValueError(
                'Must provide a value for {0}'.format(self._name))

        # Is the type VOID but a value was given?
        if value is not None and self._argument_type == FieldTypes.VOID:
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
        if self._argument_type != FieldTypes.VOID:

            # Set the value
            getattr(
                inputdata.value,
                'set_{0}'.format(_supported_input_types[
                    self._argument_type]))(value)

        # Call the function
        super(InputFunction, self).__call__(self._this, inputdata)


class OutputFunction(Function):

    """Class used to create an Output function."""

    def __call__(self, *args):
        """Do not allow calling of the output."""
        raise TypeError('Output functions are only hookable, not callable.')
