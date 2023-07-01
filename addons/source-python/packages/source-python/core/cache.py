# ../core/cache.py

"""Provides caching functionality."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Python Imports
#   FuncTools
from functools import wraps
#   Inspect
from inspect import isdatadescriptor
#   Types
from types import MethodType


# =============================================================================
# >> FORWARD IMPORTS
# =============================================================================
# Source.Python Imports
#   Core
from _core._cache import CachedProperty


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = [
    'CachedProperty',
    'cached_property',
    'cached_result'
]


# =============================================================================
# >> FUNCTIONS
# =============================================================================
def cached_property(fget=None, *args, **kwargs):
    """Decorator used to register or wrap a cached property."""
    return (CachedProperty
        if not isdatadescriptor(fget)
        else CachedProperty.wrap_descriptor
    )(fget, *args, **kwargs)


def cached_result(fget, *args, **kwargs):
    """Decorator used to register a cached method."""
    return CachedProperty(
        lambda self: MethodType(fget, self),
        *args, **kwargs
    )
