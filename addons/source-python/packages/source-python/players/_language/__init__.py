# ../players/_language/__init__.py

"""Provides a base way to get a player's language."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
#   Engines
from engines.server import engine_server


# =============================================================================
# >> FUNCTIONS
# =============================================================================
def get_client_language(index):
    """Return the language of the given client.
    
    :param int index: Index of the client.
    """
    from players.helpers import playerinfo_from_index
    playerinfo = playerinfo_from_index(index)
    if playerinfo.is_fake_client() or 'BOT' in playerinfo.steamid:
        return ''

    return engine_server.get_client_convar_value(index, 'cl_language')
