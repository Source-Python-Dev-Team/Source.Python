# ../players/classes/language/__init__.py

"""Provides a base way to get a player's language."""

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

    """Provides a property to get the player's language."""

    @property
    def language(self):
        """Return the player's language."""
        return engine_server.get_client_convar_value(self.index, 'cl_language')
