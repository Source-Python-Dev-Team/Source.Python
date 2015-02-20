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
    from menus.esc import PagedESCOption as PagedOption

    from menus.esc import SimpleESCMenu as SimpleMenu
    from menus.esc import SimpleESCOption as SimpleOption
else:
    from menus.radio import PagedRadioMenu as PagedMenu
    from menus.radio import PagedRadioOption as PagedOption

    from menus.radio import SimpleRadioMenu as SimpleMenu
    from menus.radio import SimpleRadioOption as SimpleOption


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('PagedMenu',
           'PagedOption',
           'SimpleMenu',
           'SimpleOption',
           'Text',
           )
