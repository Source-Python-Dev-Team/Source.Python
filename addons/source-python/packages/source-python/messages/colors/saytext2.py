# ../messages/colors/saytext2.py

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
#   Color
from colors import Color
#   Core
from core import GAME_NAME


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('BLUE',
           'BRIGHT_GREEN',
           'DARK_BLUE',
           'DULL_RED',
           'GRAY',
           'GREEN',
           'LIGHT_BLUE',
           'ORANGE',
           'PALE_GREEN',
           'PALE_RED',
           'PINK',
           'RED',
           # TEAM
           'WHITE',
           'YELLOW',
           )


# =============================================================================
# >> GLOBAL VARIABLES
# =============================================================================
if GAME_NAME == 'csgo':
    WHITE = '\x01'
    RED = '\x02'
    # Add TEAM color...
    BRIGHT_GREEN = '\x04'
    PALE_GREEN = '\x05'
    GREEN = '\x06'
    PALE_RED = '\x07'
    GRAY = '\x08'
    YELLOW = '\x09'
    LIGHT_BLUE = '\x0A'
    BLUE = '\x0B'
    DARK_BLUE = '\x0C'
    PINK = '\x0E'
    DULL_RED = '\x0F'
    ORANGE = '\x10'

else:
    from colors import BLUE
    from colors import GRAY
    from colors import GREEN
    from colors import LIGHT_BLUE
    from colors import LIGHT_GREEN as BRIGHT_GREEN
    from colors import LIGHT_RED as PALE_RED
    from colors import ORANGE
    from colors import RED
    from colors import WHITE
    from colors import YELLOW

    DARK_BLUE = Color(0, 0, 180)  # colors.DARK_BLUE constant is *too dark*
    DULL_RED = Color(178, 34, 34)  # http://html-color.org/B22222
    PALE_GREEN = Color(208, 253, 208)  # http://html-color.org/D0FDD0
    PINK = Color(252, 116, 253)  # http://html-color.org/FC74FD
