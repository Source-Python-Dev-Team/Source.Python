# ../players/_language/base.py

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
    from players.entity import Player
    if Player(index).is_bot():
        return ''

    return engine_server.get_client_convar_value(index, 'cl_language')
