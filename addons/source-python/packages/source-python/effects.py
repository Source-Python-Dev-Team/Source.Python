# ../effects.py

"""Provides access to all effects within the engine."""

# ============================================================================
# >> IMPORTS
# ============================================================================
# Source.Python Imports
#   Core
from core import SOURCE_ENGINE
#   Memory
from memory.manager import manager
#   Paths
from paths import SP_DATA_PATH


# =============================================================================
# >> FORWARD IMPORTS
# =============================================================================
# Source.Python Imports
#   Effects
from _effects import DispatchEffectData
from _effects import effects
from _effects import PredictionSystem
from _effects import ShatterSurface


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('DispatchEffectData',
           'PredictionSystem',
           'ShatterSurface'
           'effects',
           'temp_entities',
           )


# ============================================================================
# >> GLOBAL VARIABLES
# ============================================================================
# Get the temp_entities instance...
try:
    from _effects import temp_entities
except ImportError:
    manager.create_global_pointers_from_file(
        SP_DATA_PATH.joinpath('effects', SOURCE_ENGINE, 'pointers.ini'))

    temp_entities = manager.get_global_pointer('_TempEntities')
