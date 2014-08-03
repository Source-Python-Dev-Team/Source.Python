# ../players/games/csgo.py

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
#   Players
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
    """Game-specific class inherited by _PlayerWeapons to
        handle CS:GO specific weapon functionality for players"""
