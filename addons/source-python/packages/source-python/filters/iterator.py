# ../filters/iterator.py

"""Contains base iteration functionality for the filter package."""


# =============================================================================
# >> CLASSES
# =============================================================================
class _IterObjectMeta(type):
    """Meta class used to create a _filters dictionary for each class."""

    def __new__(mcs, name, bases, odict):
        """Create a new _filters attribute on each class creation."""
        # Create the class
        cls = super().__new__(mcs, name, bases, odict)

        # Create the _filters dictionary
        cls._filters = dict()

        # Create the filters property
        cls.__class__.filters = property(
            lambda cls: cls._filters,
            doc='Return the class filters dictionary.')

        # Return the class
        return cls


class _IterObject(metaclass=_IterObjectMeta):
    """Base iterator class used to yield filtered items."""

    def __init__(self, is_filters=None, not_filters=None):
        """Store filters for the instance."""
        # Was only one "is" filter given?
        if isinstance(is_filters, str):

            # Store the "is" filters as a list
            is_filters = [is_filters]

        # Was only one "not" filter given?
        if isinstance(not_filters, str):

            # Store the "not" filters as a list
            not_filters = [not_filters]

        # Store the filters
        self.is_filters = list() if is_filters is None else is_filters
        self.not_filters = list() if not_filters is None else not_filters

    def __iter__(self):
        """Iterate through the objects and filter out any unneeded items."""
        # Loop through the items in classes iterator
        for item in self.iterator():

            # Is the current item yieldable?
            if self._is_valid(item):

                # Yield the current item
                yield item

    def __len__(self):
        """Return the length of the generator at this current time."""
        return len([x for x in self])

    @property
    def iterator(self):
        """Raise an error if the inheriting class does not have their own."""
        raise NotImplementedError('No iterator attribute defined for class.')

    def _is_valid(self, item):
        """Return whether the given item is valid for the instances filters."""
        # Loop through all "is" filters
        for filter_name in self.is_filters:

            # Does the item pass this filter?
            if (filter_name not in self._filters or not
                    self._filters[filter_name](item)):

                # If not, return False
                return False

        # Loop through all "not" filters
        for filter_name in self.not_filters:

            # Does the item pass this filter?
            if self._filters[filter_name](item):

                # If it does, return False since these are "not" filters
                return False

        # If all checks pass, return True
        return True

    @classmethod
    def register_filter(cls, filter_name, function):
        """Register the given filter to the class."""
        # Has the item already been added to the filters?
        if filter_name in cls._filters:

            # Raise an error, since the same filter cannot be added twice
            raise NameError(
                '{0} cannot register filter "{1}".  Filter is '
                'already registered.'.format(cls.__name__, filter_name))

        # Is the given function callable?
        if not callable(function):

            # Raise an error, since the function needs to be callable
            raise TypeError(
                '{0} cannot register filter "{1}".  Given function '
                'is not callable.'.format(cls.__name__, filter_name))

        # Add the filter to the dictionary
        cls._filters[filter_name] = function

    @classmethod
    def unregister_filter(cls, filter_name):
        """Unregister the given filter from the class."""
        # Is the item registered?
        if filter_name not in cls._filters:

            # If not, raise an error
            raise NameError(
                '{0} cannot unregister filter "{1}".  Filter is '
                'not registered.'.format(cls.__name__, filter_name))

        # Remove the filter from the dictionary
        del cls._filters[filter_name]
