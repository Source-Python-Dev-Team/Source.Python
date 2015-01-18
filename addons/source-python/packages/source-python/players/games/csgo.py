# ../players/games/csgo.py

"""Provides csgo specific player functionality."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
#   Players
from players.classes.language.cache import get_client_language
from players.weapons.projectiles import _HEGrenade
from players.weapons.projectiles import _Flashbang
from players.weapons.projectiles import _SmokeGrenade
from players.weapons.projectiles import _Decoy
from players.weapons.projectiles import _Incendiary
from players.weapons.types import _C4


# =============================================================================
# >> CLASSES
# =============================================================================
class _GameWeapons(
        _HEGrenade, _Flashbang, _SmokeGrenade, _Decoy, _Incendiary, _C4):

    """CS:GO specific player weapon functionality."""
