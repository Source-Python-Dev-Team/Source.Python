# ../public.py

# =============================================================================
# >> IMPORTS
# =============================================================================
# Python Imports
#   Sys
import sys


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
# Add all the global variables to __all__
__all__ = [
    'public',
]


# =============================================================================
# >> FUNCTIONS
# =============================================================================
def public(public_object):
    '''Decorator function used to add objects to their module's __all__ list'''

    # Get the object's module's __all__ list
    base_all = sys.modules[
        public_object.__module__].__dict__.setdefault('__all__', [])

    # Get the object's name
    # Remove the leading underscore for "the instance" objects
    name = public_object.__name__.lstrip('_')

    # Is the object's name already in the list?
    if not name in base_all:

        # If not, add the name to the list
        base_all.append(name)

    # Return the object
    return public_object
