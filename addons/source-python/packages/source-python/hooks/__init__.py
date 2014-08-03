# ../hooks/__init__.py

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
#   Loggers
from loggers import _SPLogger
#   Translations
from translations.strings import LangStrings


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ()


# =============================================================================
# >> GLOBAL VARIABLES
# =============================================================================
# Get the hooks strings
_hooks_strings = LangStrings('_core/hooks_strings')

# Get the sp.hooks logger
HooksLogger = _SPLogger.hooks
