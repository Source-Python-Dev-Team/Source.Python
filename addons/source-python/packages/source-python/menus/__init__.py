# ../menus/__init__.py

"""Provides player menu based functionality."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
#   Core
from core import GAME_NAME
#   Menus
from menus.base import Text


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('Text',
           'Option',
           'SimpleMenu',
           'PagedMenu',
           )


# =============================================================================
# >> GLOBAL VARIABLES
# =============================================================================
SimpleMenu = None
PagedMenu = None
Option = None


# =============================================================================
# >> CLASSES
# =============================================================================
class _MenuStyle(object):

    """This class holds menu style values."""

    # Menus are displayed as radio menus
    RADIO = 0

    # Menus are displayed as ESC menus
    ESC = 1


# =============================================================================
# >> HELPER FUNCTIONS
# =============================================================================
def _set_default_menu_style(style):
    """Set the default menu style.

    @param <style>:
    A value of the _MenuStyle class.
    """
    from menus.radio import SimpleRadioMenu
    from menus.radio import PagedRadioMenu
    from menus.radio import RadioOption

    from menus.esc import PagedESCMenu
    from menus.esc import ESCOption

    global SimpleMenu
    global PagedMenu
    global Option

    if style == _MenuStyle.RADIO:
        SimpleMenu = SimpleRadioMenu
        PagedMenu = PagedRadioMenu
        Option = RadioOption
    elif style == _MenuStyle.ESC:
        # PagedMenu and SimpleMenu are the same for ESC menus
        PagedMenu = SimpleMenu = PagedESCMenu
        Option = ESCOption
    else:
        raise ValueError('Unknown menu style "{0}"'.format(style))

# HL2:MP will use ESC menus
if GAME_NAME == 'hl2mp':
    _set_default_menu_style(_MenuStyle.ESC)

# Default style is RADIO
else:
    _set_default_menu_style(_MenuStyle.RADIO)
