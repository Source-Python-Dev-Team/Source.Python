# ../filters/players.py

# =============================================================================
# >> IMPORTS
# =============================================================================
# Site Package Imports
#   ConfigObj
from configobj import ConfigObj

# Source.Python Imports
from player_c import PlayerGenerator
from core import GameEngine
from core import GAME_NAME
from paths import SP_DATA_PATH
from public import public
#   Filters
from filters.iterator import _IterObject
from filters.manager import _BaseFilterManager
#   Players
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
# >> GLOBAL VARIABLES
# =============================================================================
# Get the team's file for the current game
_game_teams = ConfigObj(
    SP_DATA_PATH.joinpath('players', 'teams', GAME_NAME + '.ini'), unrepr=True)


# =============================================================================
# >> MAIN PLAYER ITER CLASSES
# =============================================================================
class _PlayerIterManager(_BaseFilterManager):
    '''Filter management class specifically for player iterating'''

# Get the _PlayerIterManager instance
PlayerIterManager = _PlayerIterManager()


@public
class PlayerIter(_IterObject):
    '''Player iterate class'''

    # Store the manager for the player iterator
    manager = PlayerIterManager

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
# >> FILTER FUNCTIONS
# =============================================================================
def _is_player(PlayerInfo):
    '''Always returns True, since this is a player'''
    return True


def _player_is_bot(PlayerInfo):
    '''Returns whether a player is a bot'''
    return PlayerInfo.is_fake_client()


def _player_is_human(PlayerInfo):
    '''Returns whether the player is a human'''
    return PlayerInfo.is_player()


def _player_is_alive(PlayerInfo):
    '''Returns whether the player is alive'''
    return not PlayerInfo.is_dead()


def _player_is_dead(PlayerInfo):
    '''Returns whether the player is dead'''
    return PlayerInfo.is_dead()

# Register the filter functions
PlayerIterManager.register_filter('all', _is_player)
PlayerIterManager.register_filter('bot', _player_is_bot)
PlayerIterManager.register_filter('human', _player_is_human)
PlayerIterManager.register_filter('alive', _player_is_alive)
PlayerIterManager.register_filter('dead', _player_is_dead)

# Loop through all teams in the game's team file
for team in _game_teams:

    # Add the team to the _PlayerTeamsInstance dictionary
    _PlayerTeamsInstance[team] = _game_teams[team]

    # Register the filter
    PlayerIterManager.register_filter(
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
    PlayerIterManager.register_filter(
        team, _PlayerTeamsInstance[team]._player_is_on_team)


# =============================================================================
# >> RETURN TYPE FUNCTIONS
# =============================================================================
def _return_playerinfo(PlayerInfo):
    '''Returns the player's PlayerInfo instance'''
    return PlayerInfo


def _return_player(PlayerInfo):
    '''Returns the player's PlayerEntity instance'''
    return PlayerEntity(index_from_playerinfo(PlayerInfo))


def _return_name(PlayerInfo):
    '''Returns the player's name'''
    return PlayerInfo.get_name()


def _return_steamid(PlayerInfo):
    '''Returns the player's SteamID'''
    return PlayerInfo.get_networkid_string()


def _return_location(PlayerInfo):
    '''Returns the player's location Vector'''
    return PlayerInfo.get_abs_origin()


def _return_kills(PlayerInfo):
    '''Returns the player's kill count'''
    return PlayerInfo.get_frag_count()


def _return_deaths(PlayerInfo):
    '''Returns the player's death count'''
    return PlayerInfo.get_death_count()


def _return_model(PlayerInfo):
    '''Returns the player's model'''
    return PlayerInfo.get_model_name()


def _return_health(PlayerInfo):
    '''Returns the player's health value'''
    return PlayerInfo.get_health()


def _return_armor(PlayerInfo):
    '''Returns the player's armov value'''
    return PlayerInfo.get_armor_value()


def _return_weapon(PlayerInfo):
    '''Returns the player's currently held weapon'''
    return PlayerInfo.get_weapon_name()


def _return_language(PlayerInfo):
    '''Returns the player's language'''
    return GameEngine.get_client_convar_value(
        index_from_playerinfo(PlayerInfo), 'cl_language')


def _return_team(PlayerInfo):
    '''Returns the player's team'''
    return PlayerInfo.get_team_index()

# Register the return type functions
PlayerIterManager.register_return_type('index', index_from_playerinfo)
PlayerIterManager.register_return_type('edict', edict_from_playerinfo)
PlayerIterManager.register_return_type(
    'basehandle', basehandle_from_playerinfo)
PlayerIterManager.register_return_type('inthandle', inthandle_from_playerinfo)
PlayerIterManager.register_return_type('pointer', pointer_from_playerinfo)
PlayerIterManager.register_return_type('userid', userid_from_playerinfo)
PlayerIterManager.register_return_type('uniqueid', uniqueid_from_playerinfo)
PlayerIterManager.register_return_type('address', address_from_playerinfo)
PlayerIterManager.register_return_type('info', _return_playerinfo)
PlayerIterManager.register_return_type('player', _return_player)
PlayerIterManager.register_return_type('name', _return_name)
PlayerIterManager.register_return_type('steamid', _return_steamid)
PlayerIterManager.register_return_type('location', _return_location)
PlayerIterManager.register_return_type('kills', _return_kills)
PlayerIterManager.register_return_type('deaths', _return_deaths)
PlayerIterManager.register_return_type('model', _return_model)
PlayerIterManager.register_return_type('health', _return_health)
PlayerIterManager.register_return_type('armor', _return_armor)
PlayerIterManager.register_return_type('weapon', _return_weapon)
PlayerIterManager.register_return_type('language', _return_language)
PlayerIterManager.register_return_type('team', _return_team)
