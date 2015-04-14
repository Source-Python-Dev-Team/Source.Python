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
#   Engines
from engines.precache import Model
#   Entities
from entities.helpers import basehandle_from_edict
from entities.helpers import index_from_edict
from entities.helpers import inthandle_from_edict
from entities.helpers import pointer_from_edict
#   Filters
from filters.iterator import _IterObject
from filters.manager import _BaseFilterManager
#   Paths
from paths import SP_DATA_PATH
#   Players
from players import PlayerGenerator
from players.entity import PlayerEntity
from players.helpers import address_from_playerinfo
from players.helpers import get_client_language
from players.helpers import playerinfo_from_edict
from players.helpers import uniqueid_from_playerinfo
from players.helpers import userid_from_edict


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('PlayerIter',
           )


# =============================================================================
# >> GLOBAL VARIABLES
# =============================================================================
# Get the team's file for the current game
_game_teams = ConfigObj(
    SP_DATA_PATH.joinpath('teams', GAME_NAME + '.ini'))


# =============================================================================
# >> MAIN PLAYER ITER CLASSES
# =============================================================================
class _PlayerIterManager(_BaseFilterManager):

    """Filter management class specifically for player iterating."""

# Get the _PlayerIterManager instance
_player_iter_manager = _PlayerIterManager()


class PlayerIter(_IterObject):

    """Player iterate class."""

    # Store the manager for the player iterator
    manager = _player_iter_manager

    # Store the base iterator
    iterator = staticmethod(PlayerGenerator)


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
        super(_PlayerTeams, self).__setitem__(item, instance)

# Get the _PlayerTeams instance
_player_teams = _PlayerTeams()


class _Team(object):

    """Class used to store a team number and compare to a given player."""

    def __init__(self, team):
        """Store the team number for future use."""
        self.team = team

    def _player_is_on_team(self, playerinfo):
        """Return whether the player is on the team."""
        return playerinfo.get_team_index() == self.team


# =============================================================================
# >> FILTERS
# =============================================================================
# Register the filter functions
_player_iter_manager.register_filter('all', lambda edict: True)
_player_iter_manager.register_filter(
    'bot', lambda edict: playerinfo_from_edict(edict).is_fake_client())
_player_iter_manager.register_filter(
    'human', lambda edict: not playerinfo_from_edict(edict).is_fake_client())
_player_iter_manager.register_filter(
    'alive', lambda edict: not PlayerEntity(index_from_edict(
        edict)).dead_flag)
_player_iter_manager.register_filter(
    'dead', lambda edict: PlayerEntity(index_from_edict(
        edict)).dead_flag)

# Loop through all teams in the game's team file
for _team in _game_teams.get('names', {}):

    # Add the team to the _player_teams dictionary
    _player_teams[_team] = int(_game_teams['names'][_team])

    # Register the filter
    _player_iter_manager.register_filter(
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
    _player_iter_manager.register_filter(
        _team, _player_teams[_team]._player_is_on_team)


# =============================================================================
# >> RETURN TYPES
# =============================================================================
# Register the return type functions
_player_iter_manager.register_return_type('index', index_from_edict)
_player_iter_manager.register_return_type('edict', lambda edict: edict)
_player_iter_manager.register_return_type(
    'basehandle', basehandle_from_edict)
_player_iter_manager.register_return_type(
    'inthandle', inthandle_from_edict)
_player_iter_manager.register_return_type(
    'pointer', pointer_from_edict)
_player_iter_manager.register_return_type(
    'userid', userid_from_edict)
_player_iter_manager.register_return_type(
    'uniqueid', lambda edict: uniqueid_from_playerinfo(
        playerinfo_from_edict(edict)))
_player_iter_manager.register_return_type(
    'address', lambda edict: address_from_playerinfo(
        playerinfo_from_edict(edict)))
_player_iter_manager.register_return_type(
    'playerinfo', playerinfo_from_edict)
_player_iter_manager.register_return_type(
    'player', lambda edict: PlayerEntity(
        index_from_edict(edict)))
_player_iter_manager.register_return_type(
    'name', lambda edict: playerinfo_from_edict(edict).get_name())
_player_iter_manager.register_return_type(
    'steamid', lambda edict: playerinfo_from_edict(
        edict).get_networkid_string())
_player_iter_manager.register_return_type(
    'location', lambda edict: playerinfo_from_edict(edict).get_abs_origin())
_player_iter_manager.register_return_type(
    'kills', lambda edict: playerinfo_from_edict(edict).get_frag_count())
_player_iter_manager.register_return_type(
    'deaths', lambda edict: playerinfo_from_edict(edict).get_death_count())
_player_iter_manager.register_return_type(
    'model', lambda edict: Model(
        playerinfo_from_edict(edict).get_model_name()))
_player_iter_manager.register_return_type(
    'health', lambda edict: playerinfo_from_edict(edict).get_health())
_player_iter_manager.register_return_type(
    'armor', lambda edict: playerinfo_from_edict(edict).get_armor_value())
_player_iter_manager.register_return_type(
    'weapon', lambda edict: playerinfo_from_edict(edict).get_weapon_name())
_player_iter_manager.register_return_type(
    'language', lambda edict: get_client_language(index_from_edict(edict)))
_player_iter_manager.register_return_type(
    'team', lambda edict: playerinfo_from_edict(edict).get_team_index())
