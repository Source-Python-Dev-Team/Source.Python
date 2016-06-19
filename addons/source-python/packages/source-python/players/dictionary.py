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

    def __init__(self, player_class=Player, *args, **kwargs):
        """Initialize the dictionary."""
        super().__init__(player_class, *args, **kwargs)

    def from_userid(self, userid):
        """Get a player instance from a userid.
        
        :param int userid: The userid.
        :rtype: Player
        """
        return self[index_from_userid(userid)]
