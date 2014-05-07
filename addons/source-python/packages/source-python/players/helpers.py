# ../players/helpers.py

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
from _conversions import *
from engines.server import EngineServer


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = [
    'address_from_playerinfo',
    'basehandle_from_edict',
    'basehandle_from_index',
    'basehandle_from_inthandle',
    'basehandle_from_playerinfo',
    'basehandle_from_pointer',
    'basehandle_from_userid',
    'edict_from_basehandle',
    'edict_from_index',
    'edict_from_inthandle',
    'edict_from_playerinfo',
    'edict_from_pointer',
    'edict_from_userid',
    'index_from_basehandle',
    'index_from_edict',
    'index_from_inthandle',
    'index_from_name',
    'index_from_playerinfo',
    'index_from_pointer',
    'index_from_steamid',
    'index_from_uniqueid',
    'index_from_userid',
    'inthandle_from_basehandle',
    'inthandle_from_edict',
    'inthandle_from_index',
    'inthandle_from_playerinfo',
    'inthandle_from_pointer',
    'inthandle_from_userid',
    'playerinfo_from_basehandle',
    'playerinfo_from_edict',
    'playerinfo_from_index',
    'playerinfo_from_inthandle',
    'playerinfo_from_pointer',
    'playerinfo_from_userid',
    'pointer_from_basehandle',
    'pointer_from_edict',
    'pointer_from_index',
    'pointer_from_inthandle',
    'pointer_from_playerinfo',
    'pointer_from_userid',
    'uniqueid_from_playerinfo',
    'userid_from_basehandle',
    'userid_from_edict',
    'userid_from_index',
    'userid_from_inthandle',
    'userid_from_playerinfo',
    'userid_from_pointer',
]


# =============================================================================
# >> OTHER HELPER FUNCTIONS
# =============================================================================
def index_from_steamid(steamid):
    '''Returns an index from the given SteamID'''

    # Loop through all players on the server
    for player in CPlayerGenerator():

        # Is the current player's SteamID the same as the one given?
        if player.get_networkid_string() == steamid:

            # Return the index of the current player
            return index_from_playerinfo(player)

    # If no player found with a matching SteamID, raise an error
    raise ValueError('Invalid SteamID "{0}"'.format(steamid))


def index_from_uniqueid(uniqueid):
    '''Returns an index from the given UniqueID'''

    # Loop through all players on the server
    for player in CPlayerGenerator():

        # Is the current player's UniqueID the same as the one given?
        if uniqueid_from_playerinfo(player) == uniqueid:

            # Return the index of the current player
            return index_from_playerinfo(player)

    # If no player found with a matching UniqueID, raise an error
    raise ValueError('Invalid UniqueID "{0}"'.format(uniqueid))


def index_from_name(name):
    '''Returns an index from the given player name'''

    # Loop through all players on the server
    for player in CPlayerGenerator():

        # Is the current player's name the same as the one given?
        if player.get_name() == name:

            # Return the index of the current player
            return index_from_playerinfo(player)

    # If no player found with a matching name, raise an error
    raise ValueError('Invalid name "{0}"'.format(name))


def uniqueid_from_playerinfo(player):
    '''Returns the UniqueID for the given player'''

    # Is the player a Bot?
    if player.is_fake_client():

        # Return the bot's UniqueID
        return 'BOT_{0}'.format(player.get_name())

    # Get the player's SteamID
    steamid = player.get_networkid_string()

    # Is this a Lan SteamID?
    if 'LAN' in steamid:

        # Get the player's IP address
        address = address_from_player(player)

        # Return the Lan player's ID
        return 'LAN_{0}'.format('_'.join(address.split(':')[0].split('.')))

    # Return the player's SteamID
    return steamid


def address_from_playerinfo(player):
    '''Returns the IP address for the given player'''

    # Get the player's index
    index = index_from_playerinfo(player)

    # Get the player's NetInfo instance
    netinfo = EngineServer.get_player_net_info(index)

    # Return the player's IP Address
    return netinfo.get_address()
