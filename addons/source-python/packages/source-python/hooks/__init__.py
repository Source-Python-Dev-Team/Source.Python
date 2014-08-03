# ../hooks/__init__.py

"""Provides warnings and exceptions hooking functionality."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
#   Loggers
from loggers import _SPLogger
#   Translations
from translations.strings import LangStrings


# =============================================================================
# >> GLOBAL VARIABLES
# =============================================================================
# Get the hooks strings
_hooks_strings = LangStrings('_core/hooks_strings')

# Get the sp.hooks logger
HooksLogger = _SPLogger.hooks
