# ../messages/__init__.py

"""Provides user message based functionality."""

# ============================================================================
# >> IMPORTS
# ============================================================================
# Source.Python Imports
#   Loggers
from loggers import _sp_logger
#   Messages
from messages.base import VGUIMenu
from messages.base import ShowMenu
from messages.base import SayText2
from messages.base import HintText
from messages.base import SayText
from messages.base import Shake
from messages.base import ResetHUD
from messages.base import TextMsg
from messages.base import KeyHintText
from messages.base import Fade
from messages.base import HudMsg
from messages.base import UserMessageCreator
from messages.base import UserMessage
from messages.dialog import DialogAskConnect
from messages.dialog import DialogEntry
from messages.dialog import DialogMenu
from messages.dialog import DialogMsg
from messages.dialog import DialogText


# ============================================================================
# >> FORWARD IMPORTS
# ============================================================================
#   Messages
from _messages import DialogType
from _messages import ShakeCommand
from _messages import HudDestination
from _messages import FadeFlags
from _messages import get_message_index
from _messages import get_message_name
from _messages import get_message_size


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('DialogAskConnect',
           'DialogEntry',
           'DialogMenu',
           'DialogMsg',
           'DialogText',
           'DialogType',
           'Fade',
           'FadeFlags',
           'get_message_index',
           'get_message_name',
           'get_message_size',
           'HintText',
           'HudDestination',
           'HudMsg',
           'KeyHintText',
           'ResetHUD',
           'SayText',
           'SayText2',
           'Shake',
           'ShakeCommand',
           'ShowMenu',
           'TextMsg',
           'UserMessage',
           'UserMessageCreator',
           'VGUIMenu',
           )


# =============================================================================
# >> GLOBAL VARIABLES
# =============================================================================
# Get the sp.messages logger
messages_logger = _sp_logger.messages
