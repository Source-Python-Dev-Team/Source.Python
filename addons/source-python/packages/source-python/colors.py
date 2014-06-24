# ../colors.py

# =============================================================================
# >> FORWARD IMPORTS
# =============================================================================
# Source.Python Imports
#   BaseTypes
from basetypes import Color


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = [
    'Black',
    'Blue',
    'Brown',
    'Cyan',
    'DarkBlue',
    'DarkGray',
    'DarkGreen',
    'DarkRed',
    'Gray',
    'Green',
    'LightBlue',
    'LightGray',
    'LightGreen',
    'LightRed',
    'Olive',
    'Orange',
    'Purple',
    'Red',
    'White',
    'Yellow',
]


# =============================================================================
# >> GLOBAL VARIABLES
# =============================================================================
Red = Color(r=255)
LightRed = Color(r=255, g=80, b=80)
DarkRed = Color(r=128)
Orange = Color(r=255, g=165)
Yellow = Color(r=255, g=255)
Olive = Color(r=128, g=128)
Green = Color(g=255)
LightGreen = Color(r=128, g=255, b=128)
DarkGreen = Color(g=128)
Cyan = Color(g=255, b=255)
Blue = Color(b=255)
LightBlue = Color(r=128, g=128, b=255)
DarkBlue = Color(b=128)
Purple = Color(r=255, b=255)
Brown = Color(r=139, g=69, b=19)
White = Color(r=255, g=255, b=255)
Gray = Color(r=128, g=128, b=128)
LightGray = Color(r=192, g=192, b=192)
DarkGray = Color(r=64, g=64, b=64)
Black = Color()
