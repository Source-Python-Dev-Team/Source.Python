# ../filters/manager.py

"""Contains base filter management functionality for the filters package."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
#   Filters
from filters.registry import _FilterRegistry
from filters.registry import _ReturnTypeRegistry


# =============================================================================
# >> CLASSES
# =============================================================================
class _BaseFilterManager(object):

    """Filter registration manager class."""

    def __init__(self):
        """Called on class initialization."""
        # Store filter/return type dictionaries
        self._filters = _FilterRegistry(self.__class__.__name__)
        self._return_types = _ReturnTypeRegistry(self.__class__.__name__)

    def register_filter(self, filter_name, function):
        """Register the given filter to the class."""
        # Register the filter
        self._filters.register(filter_name, function)

    def unregister_filter(self, filter_name):
        """Unregister the given filter from the class."""
        # Unregister the filter
        self._filters.unregister(filter_name)

    def register_return_type(self, return_type, function):
        """Register the given return type to the class."""
        # Register the return type
        self._return_types.register(return_type, function)

    def unregister_return_type(self, return_type):
        """Unregister the given return type from the class."""
        # Unregister the return type
        self._return_types.unregister(return_type)
