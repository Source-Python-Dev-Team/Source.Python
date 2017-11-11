# ../cvars/flags.py

"""Provides access to all ConVar flags in the engine."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Python Imports
#   Enum
from enum import IntFlag


# =============================================================================
# >> FORWARD IMPORTS
# =============================================================================
# Source.Python Imports
#   Cvars
from _cvars import FCVAR_NONE
from _cvars import FCVAR_UNREGISTERED
from _cvars import FCVAR_DEVELOPMENTONLY
from _cvars import FCVAR_GAMEDLL
from _cvars import FCVAR_CLIENTDLL
from _cvars import FCVAR_HIDDEN
from _cvars import FCVAR_PROTECTED
from _cvars import FCVAR_SPONLY
from _cvars import FCVAR_ARCHIVE
from _cvars import FCVAR_NOTIFY
from _cvars import FCVAR_USERINFO
from _cvars import FCVAR_PRINTABLEONLY
from _cvars import FCVAR_UNLOGGED
from _cvars import FCVAR_NEVER_AS_STRING
from _cvars import FCVAR_REPLICATED
from _cvars import FCVAR_CHEAT
from _cvars import FCVAR_DEMO
from _cvars import FCVAR_DONTRECORD
from _cvars import FCVAR_NOT_CONNECTED
from _cvars import FCVAR_ARCHIVE_XBOX
from _cvars import FCVAR_SERVER_CAN_EXECUTE
from _cvars import FCVAR_SERVER_CANNOT_QUERY
from _cvars import FCVAR_CLIENTCMD_CAN_EXECUTE


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('ConVarFlags',
           )


# =============================================================================
# >> ENUMERATORS
# =============================================================================
class ConVarFlags(IntFlag):
    """ConVar's flags wrapper enumerator."""

    NONE = FCVAR_NONE
    UNREGISTERED = FCVAR_UNREGISTERED
    DEVELOPMENTONLY = FCVAR_DEVELOPMENTONLY
    GAMEDLL = FCVAR_GAMEDLL
    CLIENTDLL = FCVAR_CLIENTDLL
    HIDDEN = FCVAR_HIDDEN
    PROTECTED = FCVAR_PROTECTED
    SPONLY = FCVAR_SPONLY
    ARCHIVE = FCVAR_ARCHIVE
    NOTIFY = FCVAR_NOTIFY
    USERINFO = FCVAR_USERINFO
    PRINTABLEONLY = FCVAR_PRINTABLEONLY
    UNLOGGED = FCVAR_UNLOGGED
    NEVER_AS_STRING = FCVAR_NEVER_AS_STRING
    REPLICATED = FCVAR_REPLICATED
    CHEAT = FCVAR_CHEAT
    DEMO = FCVAR_DEMO
    DONTRECORD = FCVAR_DONTRECORD
    NOT_CONNECTED = FCVAR_NOT_CONNECTED
    ARCHIVE_XBOX = FCVAR_ARCHIVE_XBOX
    SERVER_CAN_EXECUTE = FCVAR_SERVER_CAN_EXECUTE
    SERVER_CANNOT_QUERY = FCVAR_SERVER_CANNOT_QUERY
    CLIENTCMD_CAN_EXECUTE = FCVAR_CLIENTCMD_CAN_EXECUTE
