# ../players/games/csgo.py

"""Provides csgo specific player functionality."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
#   Players
from players._language.cache import get_client_language
from players.weapons.projectiles import _HEGrenade
from players.weapons.projectiles import _Flashbang
from players.weapons.projectiles import _SmokeGrenade
from players.weapons.projectiles import _Decoy
from players.weapons.projectiles import _Incendiary
from players.weapons.types import _C4


# =============================================================================
# >> CLASSES
# =============================================================================
class _GamePlayer(
        _HEGrenade, _Flashbang, _SmokeGrenade, _Decoy, _Incendiary, _C4):
    """CS:GO specific player functionality."""

    def _get_kills(self):
        """Return the number of kills the player has."""
        return self.__getattr__('kills')

    def _set_kills(self, value):
        """Set the number of kills the player has."""
        self.increment_frag_count(value - self.kills, 0)

    kills = property(
        _get_kills, _set_kills, doc="""The number of kills a player has.""")

    def _get_deaths(self):
        """Return the number of deaths the player has."""
        return self.__getattr__('deaths')

    def _set_deaths(self, value):
        """Set the number of deaths the player has."""
        self.increment_death_count(value - self.deaths)

    deaths = property(
        _get_deaths, _set_deaths, doc="""The number of deaths a player has.""")

    def _get_assists(self):
        """Return the number of assists the player has."""
        return self.__getattr__('assists')

    def _set_assists(self, value):
        """Set the number of assists the player has."""
        self.increment_assists_count(value - self.assists)

    assists = property(
        _get_assists, _set_assists, doc="""The number of assists a player has.""")
