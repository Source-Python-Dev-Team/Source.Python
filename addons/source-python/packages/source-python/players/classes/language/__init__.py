# ../players/classes/language/__init__.py

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
    """Return the language of the given client."""
    return engine_server.get_client_convar_value(index, 'cl_language')
