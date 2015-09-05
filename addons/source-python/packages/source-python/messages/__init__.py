# ../messages/__init__.py

"""Provides user message based functionality."""

# ============================================================================
# >> IMPORTS
# ============================================================================
# Source.Python Imports
#   Loggers
from loggers import _sp_logger


# ============================================================================
# >> FORWARD IMPORTS
# ============================================================================
#   Messages
from _messages import DialogType
from _messages import send_saytext2


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('DialogType',
           'send_saytext2',
           )


# =============================================================================
# >> GLOBAL VARIABLES
# =============================================================================
# Get the sp.messages logger
messages_logger = _sp_logger.messages

# TODO: Temp "fix" to be able to load the plugin
ShowMenu = None