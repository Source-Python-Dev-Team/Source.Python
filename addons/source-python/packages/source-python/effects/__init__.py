# ../effects/__init__.py

"""Provides access to all effects within the engine."""

# ============================================================================
# >> IMPORTS
# ============================================================================
# Source.Python Imports
#   Constants
from constants import SOURCE_ENGINE
#   Memory
from memory.manager import manager
#   Paths
from paths import SP_DATA_PATH
#   Tools
from tools import server_tools


# =============================================================================
# >> FORWARD IMPORTS
# =============================================================================
# Source.Python Imports
#   Effects
from _effects import DispatchEffectData
from _effects import effects
from _effects import PredictionSystem


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('DispatchEffectData',
           'effects',
           'PredictionSystem',
           'temp_entities',
           )


# ============================================================================
# >> GLOBAL VARIABLES
# ============================================================================
# Get the temp_entities instance...
try:
    temp_entities = server_tools.get_temp_entities()
except NotImplementedError:
    manager.create_global_pointers_from_file(
        SP_DATA_PATH.joinpath('effects', SOURCE_ENGINE, 'pointers.ini'))

    temp_entities = manager.get_global_pointer('_TempEntities')
