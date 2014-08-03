# ../players/games/cstrike.py

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
#   Players
from players.weapons.projectiles import _HEGrenade
from players.weapons.projectiles import _Flashbang
from players.weapons.projectiles import _SmokeGrenade
from players.weapons.types import _C4


# =============================================================================
# >> CLASSES
# =============================================================================
class _GameWeapons(_HEGrenade, _Flashbang, _SmokeGrenade, _C4):
    """Game-specific class inherited by _PlayerWeapons to
        handle CS:S specific weapon functionality for players"""
