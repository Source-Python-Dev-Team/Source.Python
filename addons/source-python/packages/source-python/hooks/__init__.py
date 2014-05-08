# ../hooks/__init__.py

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
from loggers import _SPLogger
#   Translations
from translations.strings import LangStrings


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
# Set all to an empty list
__all__ = []


# =============================================================================
# >> GLOBAL VARIABLES
# =============================================================================
# Get the hooks strings
_hooks_strings = LangStrings('_core/hooks_strings')

# Get the sp.hooks logger
HooksLogger = _SPLogger.hooks
