# ../messages/__init__.py

# ============================================================================
# >> IMPORTS
# ============================================================================
# Source.Python Imports
from core import GAME_NAME
from loggers import _SPLogger
from paths import SP_DATA_PATH
#   Messages
from messages.base import _UserMessages


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
