# ../players/entity.py

"""Provides a class used to interact with a specific player."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
#   Entities
from entities.entity import BaseEntity
#   Players
from players.helpers import playerinfo_from_index
from players.helpers import address_from_playerinfo
from players.helpers import uniqueid_from_playerinfo
from players.games import _GameClasses
from players.games import _GameWeapons


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('PlayerEntity',
           )


# =============================================================================
# >> CLASSES
# =============================================================================
class PlayerEntity(BaseEntity, _GameClasses, _GameWeapons):

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
        yield self.pointer

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
    team = property(
        get_team, set_team,
        doc="""Property to get/set the player's team.""")
