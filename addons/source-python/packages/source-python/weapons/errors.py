# ../weapons/errors.py

"""Contains custom exceptions used by the weapons package."""

# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('WeaponIndexError',
           )


# =============================================================================
# >> CLASSES
# =============================================================================
class WeaponIndexError(Exception):

    """Used when trying to get an invalid weapon index."""
