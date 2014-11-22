# ../players/classes/language/cache.py

""""""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
#   Engines
from engines.server import QueryCvarStatus
from engines.server import engine_server
#   Listeners
from listeners import client_fully_connect_listener_manager
from listeners import on_query_cvar_value_finished_listener_manager
#   Players
from players.helpers import edict_from_index
from players.helpers import userid_from_index


# =============================================================================
# >> CLASSES
# =============================================================================
class _LanguageCache(dict):

    """"""

    def query_cvar_value_finished(cookie, index, status, cvarname, cvarvalue):
        """"""
        # 
        if cvarname != 'cl_language':
            return

        # 
        if status is not QueryCvarStatus.SUCCESS:
            return

        # 
        userid = userid_from_index(index)

        # 
        if userid in self:
            return

        # 
        self[userid] = cvarvalue

    def client_fully_connect(index):
        """"""
        engine_server.start_query_cvar_value(
            edict_from_index(index), 'cl_language')

_language_cache = _LanguageCache()
on_query_cvar_value_finished_listener_manager.register_listener(
    _language_cache.query_cvar_value_finished)
client_fully_connect_listener_manager.register_listener(
    _language_cache.client_fully_connect)


class _LanguagePropertyCache(object):

    """"""

    @property
    def language(self):
        """Return the player's cached language."""
        return _language_cache[self.userid]
