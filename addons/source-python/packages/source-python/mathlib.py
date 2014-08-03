# ../mathlib.py

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
NULL_QANGLE = QAngle(.0, .0, .0)
NULL_QUATERNION = Quaternion(.0, .0, .0, .0)
NULL_RADIANEULER = RadianEuler(.0, .0, .0)
NULL_VECTOR = Vector()
