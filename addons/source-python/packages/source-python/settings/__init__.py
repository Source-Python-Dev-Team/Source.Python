# ../settings/__init__.py

"""Provides user settings functionality."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
#   Loggers
from loggers import _sp_logger
#   Translations
from translations.strings import LangStrings


# =============================================================================
# >> GLOBAL VARIABLES
# =============================================================================
# Get the sp.settings logger
settings_logger = _sp_logger.settings

# Get the settings strings
_settings_strings = LangStrings('_core/settings_strings')
