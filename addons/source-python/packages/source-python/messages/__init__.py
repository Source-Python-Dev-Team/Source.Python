# ../messages/__init__.py

# ============================================================================
# >> IMPORTS
# ============================================================================
# Source.Python Imports
#   Core
from core import GAME_NAME
#   Loggers
from loggers import _SPLogger
#   Messages
from messages.base import _UserMessages
#   Paths
from paths import SP_DATA_PATH


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ()


# =============================================================================
# >> GLOBAL VARIABLES
# =============================================================================
# Get the sp.messages logger
MessagesLogger = _SPLogger.messages


# ============================================================================
# >> INITIALIZATION
# ============================================================================
# Loop trough all message classes
for message_name, message_class in _UserMessages(SP_DATA_PATH.joinpath(
    'messages', 'usermessages.ini'), SP_DATA_PATH.joinpath(
        'messages', 'games', GAME_NAME + '.ini')).items():

    # Globalize the current message class
    globals()[message_name] = message_class

    # Add the object, by name, to __all__
    __all__ = tuple(list(__all__) + [message_name])
