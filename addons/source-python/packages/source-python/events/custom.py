# ../events/custom.py

"""Provides a way to create custom events."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Python Imports
#   Collections
from collections import OrderedDict

# Source.Python Imports
#   Events
from events import events_logger
from events.manager import game_event_manager
#   Hooks
from hooks.exceptions import except_hooks


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('CustomEvent',
           )


# =============================================================================
# >> GLOBAL VARIABLES
# =============================================================================
# Get the sp.events.custom logger
events_custom_logger = events_logger.custom


# =============================================================================
# >> CLASSES
# =============================================================================
class _EventMeta(type):
    """Metaclass used to store class attributes in an ordered dictionary."""

    @classmethod
    def __prepare__(mcs, *args, **kwargs):
        """Return an ordered dictionary."""
        return OrderedDict()

    def __new__(mcs, name, bases, odict):
        """Called when the class is being created."""
        # Create the instance
        cls = super().__new__(mcs, name, bases, dict(odict))

        # Is this CustomEvent itself?
        if not bases:

            return cls

        # Store an ordered dictionary
        cls._odict = OrderedDict()

        # Loop through all items in the given ordered dictionary
        for item in odict:

            # Is the item not a magic method?
            if not item.startswith('__'):

                # Add the item to the dictionary
                cls._odict[item] = odict[item]

        # Loop through all base classes
        for base in bases:

            # Skip CustomEvent itself
            if base is CustomEvent:
                continue

            # Skip any non CustomEvent base classes
            if not issubclass(base, CustomEvent):
                continue

            # Loop through all items in the base class
            for item in base._odict:

                # Add the item if it doesn't already exist for the class
                if item not in cls._odict:
                    cls._odict[item] = base._odict[item]

        # Return the class
        return cls


class CustomEvent(metaclass=_EventMeta):
    """Class inherited to create custom events."""

    def __init__(self, **kwargs):
        """Called on instantiation."""
        # Loop through the given keyword arguments
        for kwarg in kwargs:

            # Is the current keyword in the event's variables?
            if kwarg not in self._odict:

                # If not, raise an error
                raise KeyError(
                    'Given keyword "{0}" is not a variable for '
                    'the event "{1}"'.format(kwarg, self.name))

            # Store the variable's value as a private attribute
            super().__setattr__('_' + kwarg, kwargs[kwarg])

    def __setattr__(self, attr, value):
        """Store variable values as private attributes."""
        # Is the given attribute for a variable?
        if not attr.startswith('_') and attr in self._odict:

            # Set the attribute as the private value
            super().__setattr__('_' + attr, value)

            # No need to go further
            return

        # Set the attribute
        super().__setattr__(attr, value)

    def __enter__(self):
        """Context management that automatically fires on exit."""
        return self

    def __exit__(self, exctype, value, trace_back):
        """Fire the event on exit, unless an exception was encountered."""
        # Was an exception raised?
        if trace_back:

            # Print the exception
            except_hooks.print_exception(exctype, value, trace_back)

            # Return
            return False

        # Fire the event
        self.fire()

        # Return
        return True

    def fire(self):
        """Fire the event with the stored variable values."""
        # Get the event's instance
        event = game_event_manager.create_event(self.name, True)

        # Loop through the event's variables
        for variable in self._odict:

            # Set the event variable values
            getattr(event, 'set_{0}'.format(self._odict[variable].method))(
                variable, getattr(self, '_' + variable))

        # Fire the event
        game_event_manager.fire_event(event)

    def reset(self):
        """Reset all event variable values to their default values."""
        # Loop through the event's variables
        for variable in self._odict:

            # Set the variable to its default value
            super().__setattr__(
                '_' + variable, getattr(self, variable)._default)

    @property
    def name(self):
        """Return the name of the event."""
        return self.__class__.__name__.lower()
