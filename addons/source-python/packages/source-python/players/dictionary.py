# ../entities/dictionary.py

"""Provides helper class to store player instances."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
#   Entities
from entities.dictionary import EntityDictionary
#   Players
from players.entity import PlayerEntity


# ============================================================================
# >> CLASSES
# ============================================================================
class PlayerDictionary(EntityDictionary):

    """Helper class used to store player instances."""

    def __init__(self, player_class=PlayerEntity):
        """Initialize the dictionary."""
        super(PlayerDictionary, self).__init__(player_class)
