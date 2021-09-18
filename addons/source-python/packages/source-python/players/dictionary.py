# ../players/dictionary.py

"""Provides helper class to store player instances."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
#   Entities
from entities.dictionary import EntityDictionary
#   Players
from players.entity import Player
from players.helpers import index_from_userid


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('PlayerDictionary',
           )


# ============================================================================
# >> CLASSES
# ============================================================================
class PlayerDictionary(EntityDictionary):
    """Helper class used to store player instances."""

    def __init__(self, factory=Player, *args, **kwargs):
        """Initializes the dictionary.

        :param callable factory:
            Factory class or function used to create missing instances. Set to
            `None` to disable this feature.

            Factory signature: index, *args, **kwargs
        :param tuple args:
            Arguments passed to the factory class or function.
        :param dict kwargs:
            Keyword arguments passed to the factory class or function.
        """
        super().__init__(factory, *args, **kwargs)

    def from_userid(self, userid):
        """Returns a player instance from a userid.
        
        :param int userid: The userid.
        :rtype: Player
        """
        return self[index_from_userid(userid)]
