# ../colors.py

"""Provides Color objects with RGBA values."""

# =============================================================================
# >> FORWARD IMPORTS
# =============================================================================
# Source.Python Imports
#   BaseTypes
from basetypes import Color


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('Black',
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
           )


# =============================================================================
# >> GLOBAL VARIABLES
# =============================================================================
Red = Color(255, 0, 0)
LightRed = Color(255, 80, 80)
DarkRed = Color(128, 0, 0)
Orange = Color(255, 165, 0)
Yellow = Color(255, 255, 0)
Olive = Color(128, 128, 0)
Green = Color(0, 255, 0)
LightGreen = Color(128, 255, 128)
DarkGreen = Color(0, 128, 0)
Cyan = Color(0, 255, 255)
Blue = Color(0, 0, 255)
LightBlue = Color(128, 128, 255)
DarkBlue = Color(0, 0, 128)
Purple = Color(255, 0, 255)
Brown = Color(139, 69, 19)
White = Color(255, 255, 255)
Gray = Color(128, 128, 128)
LightGray = Color(192, 192, 192)
DarkGray = Color(64, 64, 64)
Black = Color()
