# ../players/games/cstrike.py

"""Provides cstrike specific player functionality."""

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
class _GamePlayer(_HEGrenade, _Flashbang, _SmokeGrenade, _C4):
    """CS:S specific player functionality."""
