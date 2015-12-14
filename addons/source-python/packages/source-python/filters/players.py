# ../filters/players.py

"""Provides player filtering functionality."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Site-Package Imports
#   ConfigObj
from configobj import ConfigObj

# Source.Python Imports
#   Core
from core import GAME_NAME
#   Entities
from entities.helpers import index_from_edict
#   Filters
from filters.iterator import _IterObject
#   Paths
from paths import SP_DATA_PATH
#   Players
from players import PlayerGenerator
from players.entity import Player


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('PlayerIter',
           )


# =============================================================================
# >> GLOBAL VARIABLES
# =============================================================================
# Get the team's file for the current game
_game_teams = ConfigObj(SP_DATA_PATH / 'teams' / GAME_NAME + '.ini')


# =============================================================================
# >> PLAYER ITERATION CLASSES
# =============================================================================
class PlayerIter(_IterObject):
    """Player iterate class."""

    @staticmethod
    def iterator():
        """Iterate over all :class:`players.entity.Player` objects."""
        # Loop through all players on the server
        for edict in PlayerGenerator():

            # Yield the Player instance for the current edict
            yield Player(index_from_edict(edict))


# =============================================================================
# PLAYER TEAM CLASSES
# =============================================================================
class _PlayerTeams(dict):
    """Class used to store team names for the current game."""

    def __setitem__(self, item, value):
        """Add a _Team instance for the given team name."""
        # Get the _Team instance
        instance = _Team(value)

        # Add the _Team instance for the given team name
        super().__setitem__(item, instance)

# Get the _PlayerTeams instance
_player_teams = _PlayerTeams()


class _Team(object):
    """Class used to store a team number and compare to a given player."""

    def __init__(self, team):
        """Store the team number for future use."""
        self.team = team

    def _player_is_on_team(self, player):
        """Return whether the player is on the team."""
        return player.team == self.team


# =============================================================================
# >> FILTER REGISTRATION
# =============================================================================
# Register the filter functions
PlayerIter.register_filter('all', lambda player: True)
PlayerIter.register_filter('bot', lambda player: player.is_fake_client())
PlayerIter.register_filter('human', lambda player: not player.is_fake_client())
PlayerIter.register_filter('alive', lambda player: not player.dead_flag)
PlayerIter.register_filter('dead', lambda player: player.dead_flag)

# Loop through all teams in the game's team file
for _team in _game_teams.get('names', {}):

    # Add the team to the _player_teams dictionary
    _player_teams[_team] = int(_game_teams['names'][_team])

    # Register the filter
    PlayerIter.register_filter(
        _team, _player_teams[_team]._player_is_on_team)

# Loop through all base team names
for _number, _team in enumerate(('un', 'spec', 't', 'ct')):

    # Has the team already been added to the _player_teams dictionary
    if _team in _player_teams:

        # If it has been added, do not re-add it
        continue

    # Add the team to the _player_teams dictionary
    _player_teams[_team] = _number

    # Register the filter
    PlayerIter.register_filter(
        _team, _player_teams[_team]._player_is_on_team)
