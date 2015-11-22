# ../players/_language/cache.py

"""Provides a way to get a player's language using a cache."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
#   Engines
from engines.server import QueryCvarStatus
from engines.server import engine_server
#   Listeners
from listeners import on_client_disconnect_listener_manager
from listeners import on_client_fully_connect_listener_manager
from listeners import on_query_cvar_value_finished_listener_manager


# =============================================================================
# >> CLASSES
# =============================================================================
class _LanguageCache(dict):
    """Dictionary class that stores player's with their language."""

    @staticmethod
    def __missing__(index):
        """Return an empty string when the given index is not found."""
        return ''

    def query_cvar_value_finished(
            self, cookie, index, status, cvarname, cvarvalue):
        """Store the player's if that is what the query is for."""
        # Is the query not for the player's language?
        if cvarname != 'cl_language':
            return

        # Was the query unsuccessful?
        if status is not QueryCvarStatus.SUCCESS:
            return

        # Is the player's language already cached?
        if index in self:
            return

        # Store the player's language
        self[index] = cvarvalue

    def client_disconnect(self, index):
        """Remove the player from the cache."""
        # Is the player's language cached?
        if index in self:

            # Remove the player
            del self[index]

    @staticmethod
    def client_fully_connect(index):
        """Query the player's language when they are fully connected."""
        from entities.helpers import edict_from_index
        engine_server.start_query_cvar_value(
            edict_from_index(index), 'cl_language')

# Get the _LanguageCache instance and register the listeners
_language_cache = _LanguageCache()
on_query_cvar_value_finished_listener_manager.register_listener(
    _language_cache.query_cvar_value_finished)
on_client_fully_connect_listener_manager.register_listener(
    _language_cache.client_fully_connect)
on_client_disconnect_listener_manager.register_listener(
    _language_cache.client_disconnect)


# =============================================================================
# >> FUNCTIONS
# =============================================================================
def get_client_language(index):
    """Return the language of the given client."""
    return _language_cache[index]
