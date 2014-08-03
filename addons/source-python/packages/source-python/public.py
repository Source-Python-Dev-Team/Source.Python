# ../public.py

"""Provides a dynamic way to add objects to their module's __all__ variable."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Python Imports
#   Sys
import sys


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('public',
           )


# =============================================================================
# >> FUNCTIONS
# =============================================================================
def public(public_object):
    """Automatically add objects to their module's __all__ value."""
    # Get the object's module's __all__ list
    base_all = sys.modules[
        public_object.__module__].__dict__.setdefault('__all__', [])

    # Get the object's name
    # Remove the leading underscore for "the instance" objects
    name = public_object.__name__.lstrip('_')

    # Is the object's name already in the list?
    if name not in base_all:

        # If not, add the name to the list
        base_all.append(name)

    # Return the object
    return public_object
