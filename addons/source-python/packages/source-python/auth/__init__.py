# ../auth/__init__.py

"""Provides Authorization specific functionality."""

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
# Get the sp.auth logger
AuthLogger = _SPLogger.auth

# Get the auth language strings
_auth_strings = LangStrings('_core/auth_strings')
