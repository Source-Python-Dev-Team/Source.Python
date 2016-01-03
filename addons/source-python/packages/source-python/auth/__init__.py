# ../auth/__init__.py

"""Provides Authorization specific functionality."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
#   Loggers
from loggers import _sp_logger
#   Translations
from translations.strings import LangStrings
#   Paths
from paths import AUTH_CFG_PATH


# =============================================================================
# >> GLOBAL VARIABLES
# =============================================================================
# Get the sp.auth logger
auth_logger = _sp_logger.auth

# Get the auth language strings
_auth_strings = LangStrings('_core/auth_strings')

if not AUTH_CFG_PATH.exists():
    AUTH_CFG_PATH.mkdir()
