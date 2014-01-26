# ../players/helpers.py

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
from conversions_c import *
from public import public


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = [x for x in globals() if not x.startswith('__')]


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
