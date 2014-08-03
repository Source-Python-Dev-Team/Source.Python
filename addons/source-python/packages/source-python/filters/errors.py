# ../filters/errors.py

"""Contains custom errors that may be raised within the filters package."""

# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('FilterError',
           'ReturnTypeError',
           )


# =============================================================================
# >> CLASSES
# =============================================================================
class FilterError(TypeError):

    """Exception raised when an invalid filter is requested."""


class ReturnTypeError(TypeError):

    """Exception raised when an invalid return type is requested."""
