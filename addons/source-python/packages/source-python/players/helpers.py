# ../players/helpers.py

"""Provides helper functions specific to players."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
#   Engines
from engines.server import engine_server
#   Players
from players import PlayerGenerator
from players._language import get_client_language


# =============================================================================
# >> FORWARD IMPORTS
# =============================================================================
# Source.Python Imports
#   Players
from _players._helpers import address_from_playerinfo
from _players._helpers import baseentity_from_playerinfo
from _players._helpers import baseentity_from_userid
from _players._helpers import basehandle_from_playerinfo
from _players._helpers import basehandle_from_userid
from _players._helpers import edict_from_playerinfo
from _players._helpers import edict_from_userid
from _players._helpers import index_from_playerinfo
from _players._helpers import index_from_userid
from _players._helpers import index_from_name
from _players._helpers import index_from_steamid
from _players._helpers import index_from_uniqueid
from _players._helpers import inthandle_from_playerinfo
from _players._helpers import inthandle_from_userid
from _players._helpers import playerinfo_from_baseentity
from _players._helpers import playerinfo_from_basehandle
from _players._helpers import playerinfo_from_edict
from _players._helpers import playerinfo_from_index
from _players._helpers import playerinfo_from_inthandle
from _players._helpers import playerinfo_from_pointer
from _players._helpers import playerinfo_from_userid
from _players._helpers import pointer_from_playerinfo
from _players._helpers import pointer_from_userid
from _players._helpers import userid_from_baseentity
from _players._helpers import userid_from_basehandle
from _players._helpers import userid_from_edict
from _players._helpers import userid_from_index
from _players._helpers import userid_from_inthandle
from _players._helpers import userid_from_playerinfo
from _players._helpers import userid_from_pointer
from _players._helpers import uniqueid_from_index
from _players._helpers import uniqueid_from_playerinfo


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('address_from_playerinfo',
           'baseentity_from_playerinfo',
           'baseentity_from_userid',
           'basehandle_from_playerinfo',
           'basehandle_from_userid',
           'edict_from_playerinfo',
           'edict_from_userid',
           'get_client_language',
           'index_from_name',
           'index_from_playerinfo',
           'index_from_steamid',
           'index_from_uniqueid',
           'index_from_userid',
           'inthandle_from_playerinfo',
           'inthandle_from_userid',
           'playerinfo_from_baseentity',
           'playerinfo_from_basehandle',
           'playerinfo_from_edict',
           'playerinfo_from_index',
           'playerinfo_from_inthandle',
           'playerinfo_from_pointer',
           'playerinfo_from_userid',
           'pointer_from_playerinfo',
           'pointer_from_userid',
           'uniqueid_from_index',
           'uniqueid_from_playerinfo',
           'userid_from_baseentity',
           'userid_from_basehandle',
           'userid_from_edict',
           'userid_from_index',
           'userid_from_inthandle',
           'userid_from_playerinfo',
           'userid_from_pointer',
           )
