# ../entities/props.py

"""Provides access to entity network properties."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Python Imports
#   Enum
from enum import IntFlag


# =============================================================================
# >> FORWARD IMPORTS
# =============================================================================
# Source.Python Imports
#   Entities
from _entities._props import SendProp
from _entities._props import SendPropType
from _entities._props import SendPropVariant
from _entities._props import SendTable
from _entities._props import ServerClass
from _entities._props import SendProxyRecipients
from _entities._props import SPROP_UNSIGNED
from _entities._props import SPROP_COORD
from _entities._props import SPROP_NOSCALE
from _entities._props import SPROP_ROUNDDOWN
from _entities._props import SPROP_ROUNDUP
from _entities._props import SPROP_NORMAL
from _entities._props import SPROP_EXCLUDE
from _entities._props import SPROP_XYZE
from _entities._props import SPROP_INSIDEARRAY
from _entities._props import SPROP_PROXY_ALWAYS_YES
from _entities._props import SPROP_IS_A_VECTOR_ELEM
from _entities._props import SPROP_COLLAPSIBLE
from _entities._props import SPROP_COORD_MP
from _entities._props import SPROP_COORD_MP_LOWPRECISION
from _entities._props import SPROP_COORD_MP_INTEGRAL
from _entities._props import SPROP_CHANGES_OFTEN


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('SendProp',
           'SendPropFlags',
           'SendPropType',
           'SendPropVariant',
           'SendTable',
           'ServerClass',
           'SendProxyRecipients',
           )


# =============================================================================
# >> ENUMERATORS
# =============================================================================
class SendPropFlags(IntFlag):
    """SendProp flags wrapper enumerator."""

    UNSIGNED = SPROP_UNSIGNED
    COORD = SPROP_COORD
    NOSCALE = SPROP_NOSCALE
    ROUND_DOWN = SPROP_ROUNDDOWN
    ROUND_UP = SPROP_ROUNDUP
    NORMAL = SPROP_NORMAL
    EXCLUDE = SPROP_EXCLUDE
    XYZE = SPROP_XYZE
    INSIDE_ARRAY = SPROP_INSIDEARRAY
    PROXY_ALWAYS_YES = SPROP_PROXY_ALWAYS_YES
    IS_A_VECTOR_ELEM = SPROP_IS_A_VECTOR_ELEM
    COLLAPSIBLE = SPROP_COLLAPSIBLE
    COORD_MP = SPROP_COORD_MP
    COORD_MP_LOW_PRECISION = SPROP_COORD_MP_LOWPRECISION
    COORD_MP_INTEGRAL = SPROP_COORD_MP_INTEGRAL
    CHANGES_OFTEN = SPROP_CHANGES_OFTEN
