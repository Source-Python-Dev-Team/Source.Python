# ../plugins/__init__.py

"""Provides an interface to load/unload plugins."""

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
# Get the plugin strings
_plugin_strings = LangStrings('_core/plugin_strings')

# Get the sp.plugins logger
plugins_logger = _sp_logger.plugins
