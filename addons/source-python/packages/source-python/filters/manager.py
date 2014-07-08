# ../filters/manager.py

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
#   Filters
from filters.registry import _FilterRegistry
from filters.registry import _ReturnTypeRegistry


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
# Set all to an empty list
__all__ = []


# =============================================================================
# >> CLASSES
# =============================================================================
class _BaseFilterManager(object):
    '''Class designed to be used for object
        filtering and returning specific types'''

    def __init__(self):
        '''Called on class initialization'''

        # Store filter/return type dictionaries
        self._filters = _FilterRegistry(self.__qualname__)
        self._return_types = _ReturnTypeRegistry(self.__qualname__)

    def register_filter(self, filter_name, function):
        '''Registers the given filter to the class'''

        # Register the filter
        self._filters.register(filter_name, function)

    def unregister_filter(self, filter_name):
        '''Unregisters the given filter from the class'''

        # Unregister the filter
        self._filters.unregister(filter_name)

    def register_return_type(self, return_type, function):
        '''Registers the given return type to the class'''

        # Register the return type
        self._return_types.register(return_type, function)

    def unregister_return_type(self, return_type):
        '''Unregisters the given return type from the class'''

        # Unregister the return type
        self._return_types.unregister(return_type)
