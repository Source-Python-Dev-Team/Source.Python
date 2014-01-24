# ../players/helpers.py

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
from player_c import CPlayerGenerator
from player_c import PlayerInfo
from core import GameEngine
from public import public
#   Entities
from entities.helpers import *
from entities.helpers import __all__ as _all


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
# Set __all__ to the entities.helpers __all__ list
__all__ = _all


# =============================================================================
# >> HELPER FUNCTIONS
# =============================================================================
@public
def index_from_userid(userid):
    '''Returns an index from the given userid'''
    return index_from_playerinfo(playerinfo_from_userid(userid))


@public
def index_from_playerinfo(playerinfo):
    '''Returns an index from the given IPlayerInfo instance'''
    return index_from_edict(edict_from_playerinfo(playerinfo))


@public
def edict_from_userid(userid):
    '''Returns an edict from the given userid'''
    return edict_from_playerinfo(playerinfo_from_userid(userid))


@public
def edict_from_playerinfo(playerinfo):
    '''Returns an edict from the given IPlayerInfo instance'''
    return playerinfo.get_edict()


@public
def basehandle_from_userid(userid):
    '''Returns a BaseHandle instance from the given userid'''
    return basehandle_from_edict(edict_from_userid(userid))


@public
def basehandle_from_playerinfo(playerinfo):
    '''Returns a BaseHandle instance from the given IPlayerInfo instance'''
    return basehandle_from_edict(edict_from_playerinfo(playerinfo))


@public
def inthandle_from_userid(userid):
    '''Returns a handle in int form from the given userid'''
    return inthandle_from_basehandle(basehandle_from_userid(userid))


@public
def inthandle_from_playerinfo(playerinfo):
    '''Returns a handle in int form from the given IPlayerInfo instance'''
    return inthandle_from_edict(edict_from_playerinfo(playerinfo))


@public
def pointer_from_userid(userid):
    '''Returns a BaseEntity pointer from the given userid'''
    return pointer_from_edict(edict_from_userid(userid))


@public
def pointer_from_playerinfo(playerinfo):
    '''Returns a BaseEntity pointer from the given IPlayerInfo instance'''
    return pointer_from_edict(edict_from_playerinfo(playerinfo))


@public
def userid_from_index(index):
    '''Returns a userid from the given index'''
    return userid_from_edict(edict_from_index(index))


@public
def userid_from_edict(edict):
    '''Returns a userid from the given edict'''
    return userid_from_playerinfo(playerinfo_from_edict(edict))


@public
def userid_from_basehandle(bhandle):
    '''Returns a userid from the given BaseHandle instance'''
    return userid_from_index(index_from_basehandle(bhandle))


@public
def userid_from_inthandle(ihandle):
    '''Returns a userid from the given handle in int form'''
    return userid_from_index(index_from_inthandle(ihandle))


@public
def userid_from_pointer(pointer):
    '''Returns a userid from the given BaseEntity pointer'''
    return userid_from_index(index_from_pointer(pointer))


@public
def userid_from_playerinfo(playerinfo):
    '''Returns a userid from the given IPlayerInfo instance'''
    return playerinfo.get_userid()


@public
def playerinfo_from_index(index):
    '''Returns an IPlayerInfo instance from the given index'''
    return playerinfo_from_edict(edict_from_index(index))


@public
def playerinfo_from_edict(edict):
    '''Returns an IPlayerInfo instance from the given edict'''
    return PlayerInfo(edict)


@public
def playerinfo_from_basehandle(bhandle):
    '''Returns an IPlayerInfo instance from the given BaseHandle instance'''
    return playerinfo_from_edict(edict_from_basehandle(bhandle))


@public
def playerinfo_from_inthandle(ihandle):
    '''Returns an IPlayerInfo instance from the given handle in int form'''
    return playerinfo_from_edict(edict_from_inthandle(ihandle))


@public
def playerinfo_from_pointer(pointer):
    '''Returns an IPlayerInfo instance from the given BaseEntity pointer'''
    return playerinfo_from_edict(edict_from_pointer(pointer))


@public
def playerinfo_from_userid(userid):
    '''Returns an IPlayerInfo instance from the given userid'''
    return PlayerInfo(userid)


# =============================================================================
# >> OTHER HELPER FUNCTIONS
# =============================================================================
@public
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


@public
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


@public
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


@public
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


@public
def address_from_playerinfo(player):
    '''Returns the IP address for the given player'''

    # Get the player's index
    index = index_from_playerinfo(player)

    # Get the player's NetInfo instance
    netinfo = GameEngine.get_player_net_info(index)

    # Return the player's IP Address
    return netinfo.get_address()
