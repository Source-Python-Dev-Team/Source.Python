# ../players/entity.py

"""Provides a class used to interact with a specific player."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
#   Engines
from engines.server import EngineServer
#   Entities
from entities.entity import BaseEntity
#   Players
from players.helpers import playerinfo_from_index
from players.helpers import address_from_playerinfo
from players.helpers import uniqueid_from_playerinfo
from players.weapons import _PlayerWeapons


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('PlayerEntity',
           )


# =============================================================================
# >> CLASSES
# =============================================================================
class PlayerEntity(BaseEntity, _PlayerWeapons):

    """Class used to interact directly with players."""

    def __new__(cls, index):
        """Set the "entities" attribute and set the PlayerInfo."""
        # Get the "self" object using the super class' __new__
        self = super(PlayerEntity, cls).__new__(cls, index)

        # Set the player's playerinfo attribute
        self._playerinfo = playerinfo_from_index(self.index)

        # Is the IPlayerInfo instance valid?
        if self.playerinfo is None:

            raise ValueError(
                'Invalid IPlayerInfo instance for index "{0}"'.format(index))

        # Set the entities attribute
        self._entities = frozenset(['entity', 'player'])

        # Return the instance
        return self

    @property
    def playerinfo(self):
        """Return the player's IPlayerInfo instance."""
        return self._playerinfo

    @property
    def instances(self):
        """Yield the player's IPlayerInfo and Edict instances."""
        yield self.playerinfo
        yield self.edict

    @property
    def userid(self):
        """Return the player's userid."""
        return self.playerinfo.get_userid()

    @property
    def steamid(self):
        """Return the player's SteamID."""
        return self.playerinfo.get_networkid_string()

    @property
    def name(self):
        """Return the player's name."""
        return self.playerinfo.get_name()

    @property
    def isdead(self):
        """Return if the player is dead or alive."""
        return self.playerinfo.is_dead()

    @property
    def language(self):
        """Return the player's language."""
        return EngineServer.get_client_convar_value(self.index, 'cl_language')

    @property
    def uniqueid(self):
        """Return the player's uniqueid."""
        return uniqueid_from_playerinfo(self.playerinfo)

    @property
    def address(self):
        """Return the player's IP address."""
        return address_from_playerinfo(self.playerinfo)

    def get_team(self):
        """Return the player's team."""
        return self.playerinfo.get_team_index()

    def set_team(self, value):
        """Set the players team."""
        self.playerinfo.change_team(value)

    # Set the "team" property methods
    team = property(get_team, set_team)
