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
from players.games import get_client_language


# =============================================================================
# >> FORWARD IMPORTS
# =============================================================================
# Source.Python Imports
#   Players
from _players._helpers import INVALID_PLAYER_USERID
from _players._helpers import basehandle_from_playerinfo
from _players._helpers import basehandle_from_userid
from _players._helpers import edict_from_playerinfo
from _players._helpers import edict_from_userid
from _players._helpers import index_from_playerinfo
from _players._helpers import index_from_userid
from _players._helpers import inthandle_from_playerinfo
from _players._helpers import inthandle_from_userid
from _players._helpers import playerinfo_from_basehandle
from _players._helpers import playerinfo_from_edict
from _players._helpers import playerinfo_from_index
from _players._helpers import playerinfo_from_inthandle
from _players._helpers import playerinfo_from_pointer
from _players._helpers import playerinfo_from_userid
from _players._helpers import pointer_from_playerinfo
from _players._helpers import pointer_from_userid
from _players._helpers import userid_from_basehandle
from _players._helpers import userid_from_edict
from _players._helpers import userid_from_index
from _players._helpers import userid_from_inthandle
from _players._helpers import userid_from_playerinfo
from _players._helpers import userid_from_pointer


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('INVALID_PLAYER_USERID',
           'address_from_playerinfo',
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
           'playerinfo_from_basehandle',
           'playerinfo_from_edict',
           'playerinfo_from_index',
           'playerinfo_from_inthandle',
           'playerinfo_from_pointer',
           'playerinfo_from_userid',
           'pointer_from_playerinfo',
           'pointer_from_userid',
           'uniqueid_from_playerinfo',
           'userid_from_basehandle',
           'userid_from_edict',
           'userid_from_index',
           'userid_from_inthandle',
           'userid_from_playerinfo',
           'userid_from_pointer',
           )


# =============================================================================
# >> OTHER HELPER FUNCTIONS
# =============================================================================
def index_from_steamid(steamid):
    """Return an index from the given SteamID."""
    # Loop through all players on the server
    for edict in PlayerGenerator():

        # Get the PlayerInfo instance of the player...
        playerinfo = playerinfo_from_edict(edict)

        # Is the current player's SteamID the same as the one given?
        if playerinfo.get_networkid_string() == steamid:

            # Return the index of the current player
            return index_from_playerinfo(playerinfo)

    # If no player found with a matching SteamID, raise an error
    raise ValueError('Invalid SteamID "{0}"'.format(steamid))


def index_from_uniqueid(uniqueid):
    """Return an index from the given UniqueID."""
    # Loop through all players on the server
    for edict in PlayerGenerator():

        # Get the PlayerInfo instance of the player...
        playerinfo = playerinfo_from_edict(edict)

        # Is the current player's UniqueID the same as the one given?
        if uniqueid_from_playerinfo(playerinfo) == uniqueid:

            # Return the index of the current player
            return index_from_playerinfo(playerinfo)

    # If no player found with a matching UniqueID, raise an error
    raise ValueError('Invalid UniqueID "{0}"'.format(uniqueid))


def index_from_name(name):
    """Return an index from the given player name."""
    # Loop through all players on the server
    for edict in PlayerGenerator():

        # Get the PlayerInfo instance of the player...
        playerinfo = playerinfo_from_edict(edict)

        # Is the current player's name the same as the one given?
        if playerinfo.get_name() == name:

            # Return the index of the current player
            return index_from_playerinfo(playerinfo)

    # If no player found with a matching name, raise an error
    raise ValueError('Invalid name "{0}"'.format(name))


def uniqueid_from_playerinfo(playerinfo):
    """Return the UniqueID for the given player."""
    # Is the player a Bot?
    if playerinfo.is_fake_client():

        # Return the bot's UniqueID
        return 'BOT_{0}'.format(playerinfo.get_name())

    # Get the player's SteamID
    steamid = playerinfo.get_networkid_string()

    # Is this a Lan SteamID?
    if 'LAN' in steamid:

        # Get the player's IP address
        address = address_from_playerinfo(playerinfo)

        # Return the Lan player's ID
        return 'LAN_{0}'.format('_'.join(address.split(':')[0].split('.')))

    # Return the player's SteamID
    return steamid


def address_from_playerinfo(playerinfo):
    """Return the IP address for the given player."""
    # Is the player a bot?
    if playerinfo.is_fake_client():

        # Return a base value, since using
        # <netinfo>.get_address() crashes with bots
        return '0'

    # Get the player's index
    index = index_from_playerinfo(playerinfo)

    # Get the player's NetInfo instance
    netinfo = engine_server.get_player_net_info(index)

    # Return the player's IP Address
    return netinfo.get_address()
