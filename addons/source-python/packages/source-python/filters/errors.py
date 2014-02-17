# ../filters/errors.py

# =============================================================================
# >> ALL DECLARATION
# =============================================================================
# Add all the global variables to __all__
__all__ = [
    'FilterError',
    'ReturnTypeError',
]


# =============================================================================
# >> CLASSES
# =============================================================================
class FilterError(TypeError):
    '''Exception raised when an invalid filter is requested'''


class ReturnTypeError(TypeError):
    '''Exception raised when an invalid return type is requested'''
