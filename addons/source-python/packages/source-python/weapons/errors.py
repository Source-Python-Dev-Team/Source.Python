# ../weapons/errors.py

# =============================================================================
# >> ALL DECLARATION
# =============================================================================
# Add all the global variables to __all__
__all__ = [
    'WeaponIndexError',
]


# =============================================================================
# >> CLASSES
# =============================================================================
class WeaponIndexError(Exception):
    '''Used when trying to get an invalid weapon index'''
