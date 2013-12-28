# ../plugins/__init__.py

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
# Get the plugin strings
_plugin_strings = LangStrings('_core/plugin_strings')

# Get the sp.plugins logger
PluginsLogger = _SPLogger.plugins
