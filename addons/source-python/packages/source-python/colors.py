# ../colors.py

"""Provides Color objects with RGBA values."""

# =============================================================================
# >> FORWARD IMPORTS
# =============================================================================
# Source.Python Imports
#   Colors
from _colors import Color


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('BLACK',
           'BLUE',
           'BROWN',
           'CYAN',
           'Color',
           'DARK_BLUE',
           'DARK_GRAY',
           'DARK_GREEN',
           'DARK_RED',
           'GRAY',
           'GREEN',
           'LIGHT_BLUE',
           'LIGHT_GRAY',
           'LIGHT_GREEN',
           'LIGHT_RED',
           'OLIVE',
           'ORANGE',
           'PURPLE',
           'RED',
           'WHITE',
           'YELLOW',
           )


# =============================================================================
# >> GLOBAL VARIABLES
# =============================================================================
RED = Color(255, 0, 0)  #:
LIGHT_RED = Color(255, 80, 80)  #:
DARK_RED = Color(128, 0, 0)  #:
ORANGE = Color(255, 165, 0)  #:
YELLOW = Color(255, 255, 0)  #:
OLIVE = Color(128, 128, 0)  #:
GREEN = Color(0, 255, 0)  #:
LIGHT_GREEN = Color(128, 255, 128)  #:
DARK_GREEN = Color(0, 128, 0)  #:
CYAN = Color(0, 255, 255)  #:
BLUE = Color(0, 0, 255)  #:
LIGHT_BLUE = Color(128, 128, 255)  #:
DARK_BLUE = Color(0, 0, 128)  #:
PURPLE = Color(255, 0, 255)  #:
BROWN = Color(139, 69, 19)  #:
WHITE = Color(255, 255, 255)  #:
GRAY = Color(128, 128, 128)  #:
LIGHT_GRAY = Color(192, 192, 192)  #:
DARK_GRAY = Color(64, 64, 64)  #:
BLACK = Color()  #:
