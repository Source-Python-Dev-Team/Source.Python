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
if GAME_NAME in ('hl2mp', ):
    from menus.esc import PagedESCMenu as PagedMenu
    from menus.esc import ESCOption as Option
    SimpleMenu = PagedMenu
else:
    from menus.radio import PagedRadioMenu as PagedMenu
    from menus.radio import SimpleRadioMenu as SimpleMenu
    from menus.radio import RadioOption as Option


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('Text',
           'Option',
           'SimpleMenu',
           'PagedMenu',
           )
