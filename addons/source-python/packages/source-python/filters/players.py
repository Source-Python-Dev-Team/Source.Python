# ../filters/players.py

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
from engines.server import EngineServer
#   Filters
from filters.iterator import _IterObject
from filters.manager import _BaseFilterManager
#   Paths
from paths import SP_DATA_PATH
#   Players
from players import PlayerGenerator
from players.entity import PlayerEntity
from players.helpers import address_from_playerinfo
from players.helpers import basehandle_from_playerinfo
from players.helpers import edict_from_playerinfo
from players.helpers import index_from_playerinfo
from players.helpers import inthandle_from_playerinfo
from players.helpers import pointer_from_playerinfo
from players.helpers import uniqueid_from_playerinfo
from players.helpers import userid_from_playerinfo


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
# Add all the global variables to __all__
__all__ = [
    'PlayerIter',
]


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
    '''Filter management class specifically for player iterating'''

# Get the _PlayerIterManager instance
_PlayerIterManagerInstance = _PlayerIterManager()


class PlayerIter(_IterObject):
    '''Player iterate class'''

    # Store the manager for the player iterator
    manager = _PlayerIterManagerInstance

    # Store the base iterator
    iterator = staticmethod(PlayerGenerator)


# =============================================================================
# PLAYER TEAM CLASSES
# =============================================================================
class _PlayerTeams(dict):
    '''Class used to store team names for the current game'''

    def __setitem__(self, item, value):
        '''Adds a _Team instance for the given team name'''

        # Get the _Team instance
        instance = _Team(value)

        # Add the _Team instance for the given team name
        super(_PlayerTeams, self).__setitem__(item, instance)

# Get the _PlayerTeams instance
_PlayerTeamsInstance = _PlayerTeams()


class _Team(object):
    '''Class used to store a team number and compare to a given player'''

    def __init__(self, team):
        '''Store the team number for future use'''

        # Store the team number
        self.team = team

    def _player_is_on_team(self, PlayerInfo):
        '''Returns whether the player is on the team'''

        # Return whether the player is on the team
        return PlayerInfo.get_team_index() == self.team


# =============================================================================
# >> FILTERS
# =============================================================================
# Register the filter functions
_PlayerIterManagerInstance.register_filter('all', lambda playerinfo: True)
_PlayerIterManagerInstance.register_filter(
    'bot', lambda playerinfo: playerinfo.is_fake_client())
_PlayerIterManagerInstance.register_filter(
    'human', lambda playerinfo: not playerinfo.is_fake_client())
_PlayerIterManagerInstance.register_filter(
    'alive', lambda playerinfo: not edict_from_playerinfo(
        playerinfo).get_prop_int('pl.deadflag'))
_PlayerIterManagerInstance.register_filter(
    'dead', lambda playerinfo: edict_from_playerinfo(
        playerinfo).get_prop_int('pl.deadflag'))

# Loop through all teams in the game's team file
for team in _game_teams['names']:

    # Add the team to the _PlayerTeamsInstance dictionary
    _PlayerTeamsInstance[team] = int(_game_teams['names'][team])

    # Register the filter
    _PlayerIterManagerInstance.register_filter(
        team, _PlayerTeamsInstance[team]._player_is_on_team)

# Loop through all base team names
for number, team in enumerate(('un', 'spec', 't', 'ct')):

    # Has the team already been added to the _PlayerTeamsInstance dictionary
    if team in _PlayerTeamsInstance:

        # If it has been added, do not re-add it
        continue

    # Add the team to the _PlayerTeamsInstance dictionary
    _PlayerTeamsInstance[team] = number

    # Register the filter
    _PlayerIterManagerInstance.register_filter(
        team, _PlayerTeamsInstance[team]._player_is_on_team)


# =============================================================================
# >> RETURN TYPES
# =============================================================================
# Register the return type functions
_PlayerIterManagerInstance.register_return_type('index', index_from_playerinfo)
_PlayerIterManagerInstance.register_return_type('edict', edict_from_playerinfo)
_PlayerIterManagerInstance.register_return_type(
    'basehandle', basehandle_from_playerinfo)
_PlayerIterManagerInstance.register_return_type(
    'inthandle', inthandle_from_playerinfo)
_PlayerIterManagerInstance.register_return_type(
    'pointer', pointer_from_playerinfo)
_PlayerIterManagerInstance.register_return_type(
    'userid', userid_from_playerinfo)
_PlayerIterManagerInstance.register_return_type(
    'uniqueid', uniqueid_from_playerinfo)
_PlayerIterManagerInstance.register_return_type(
    'address', address_from_playerinfo)
_PlayerIterManagerInstance.register_return_type(
    'info', lambda playerinfo: playerinfo)
_PlayerIterManagerInstance.register_return_type(
    'player', lambda playerinfo: PlayerEntity(
        index_from_playerinfo(playerinfo)))
_PlayerIterManagerInstance.register_return_type(
    'name', lambda playerinfo: playerinfo.get_name())
_PlayerIterManagerInstance.register_return_type(
    'steamid', lambda playerinfo: playerinfo.get_networkid_string())
_PlayerIterManagerInstance.register_return_type(
    'location', lambda playerinfo: playerinfo.get_abs_origin())
_PlayerIterManagerInstance.register_return_type(
    'kills', lambda playerinfo: playerinfo.get_frag_count())
_PlayerIterManagerInstance.register_return_type(
    'deaths', lambda playerinfo: playerinfo.get_death_count())
_PlayerIterManagerInstance.register_return_type(
    'model', lambda playerinfo: playerinfo.get_model_name())
_PlayerIterManagerInstance.register_return_type(
    'health', lambda playerinfo: playerinfo.get_health())
_PlayerIterManagerInstance.register_return_type(
    'armor', lambda playerinfo: playerinfo.get_armor_value())
_PlayerIterManagerInstance.register_return_type(
    'weapon', lambda playerinfo: playerinfo.get_weapon_name())
_PlayerIterManagerInstance.register_return_type(
    'language', lambda playerinfo: EngineServer.get_client_convar_value(
        index_from_playerinfo(playerinfo), 'cl_language'))
_PlayerIterManagerInstance.register_return_type(
    'team', lambda playerinfo: playerinfo.get_team_index())
