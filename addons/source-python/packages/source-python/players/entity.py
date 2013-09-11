# ../players/entity.py

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
from player_c import CPlayerInfo
from core import GameEngine
from public import public
#   Entities
from entities.entity import BaseEntity
#   Players
from players.helpers import address_from_playerinfo
from players.helpers import uniqueid_from_playerinfo
from players.weapons import _PlayerWeapons


# =============================================================================
# >> CLASSES
# =============================================================================
@public
class PlayerEntity(BaseEntity, _PlayerWeapons):
    '''Class used to interact directly with players'''

    info = None

    def __init__(self, index):
        '''Override the __init__ method to set the
            "entities" attribute and set the PlayerInfo'''

        # Set the player's info attribute
        self.info = CPlayerInfo(self.edict)

        # Is the IPlayerInfo instance valid?
        if self.info is None:

            raise ValueError(
                'Invalid IPlayerInfo instance for index "{0}"'.format(index))

        # Set the entities attribute
        self._entities = frozenset(['entity', 'player'])

    @property
    def instances(self):
        '''Yields the player's IPlayerInfo and Edict instances'''
        yield self.info
        yield self.edict

    @property
    def userid(self):
        '''Returns the player's userid'''
        return self.info.get_userid()

    @property
    def steamid(self):
        '''Returns the player's SteamID'''
        return self.info.get_networkid_string()

    @property
    def name(self):
        '''Returns the player's name'''
        return self.info.get_name()

    @property
    def isdead(self):
        '''Returns if the player is dead or alive'''
        return self.info.is_dead()

    @property
    def language(self):
        '''Returns the player's language'''
        return GameEngine.get_client_convar_value(self.index, 'cl_language')

    @property
    def uniqueid(self):
        '''Returns the player's uniqueid'''
        return uniqueid_from_playerinfo(self.info)

    @property
    def address(self):
        '''Returns the player's IP address'''
        return address_from_playerinfo(self.info)

    def get_team(self):
        '''Returns the player's team'''
        return self.info.get_team_index()

    def set_team(self, value):
        '''Sets a players team'''
        self.info.change_team(value)

    # Set the "team" property methods
    team = property(get_team, set_team)
