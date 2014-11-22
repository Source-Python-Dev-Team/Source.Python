# ../players/classes/language/__init__.py

""""""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
#   Engines
from engines.server import engine_server


# =============================================================================
# >> CLASSES
# =============================================================================
class _LanguagePropertyBase(object):

    """"""

    @property
    def language(self):
        """Return the player's language."""
        return engine_server.get_client_convar_value(self.index, 'cl_language')
