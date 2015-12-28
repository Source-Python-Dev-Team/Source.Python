# ../mathlib.py

"""Provides access to mathematical objects."""

# =============================================================================
# >> FORWARD IMPORTS
# =============================================================================
# Source.Python Imports
#   MathLib
from _mathlib import Vector
from _mathlib import QAngle
from _mathlib import Quaternion
from _mathlib import Plane
from _mathlib import RadianEuler


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('NULL_QANGLE',
           'NULL_QUATERNION',
           'NULL_RADIANEULER',
           'NULL_VECTOR',
           'Plane',
           'QAngle',
           'Quaternion',
           'RadianEuler',
           'Vector',
           )


# =============================================================================
# >> GLOBAL VARIABLES
# =============================================================================
# Set the Null objects
NULL_QANGLE = QAngle()  #:
NULL_QUATERNION = Quaternion()  #:
NULL_RADIANEULER = RadianEuler()  #:
NULL_VECTOR = Vector()  #:
