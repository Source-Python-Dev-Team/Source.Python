# ../filters/errors.py


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
# Set all to an empty list
__all__ = []


# =============================================================================
# >> CLASSES
# =============================================================================
class FilterError(Exception):
    '''Exception raised when an invalid filter is requested'''


class ReturnTypeError(Exception):
    '''Exception raised when an invalid return type is requested'''
