# ../effects/__init__.py

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
from tools import ServerTools


# =============================================================================
# >> FORWARD IMPORTS
# =============================================================================
# Source.Python Imports
#   Effects
from _effects import DispatchEffectData
from _effects import Effects
from _effects import PredictionSystem


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = [
    'DispatchEffectData',
    'Effects',
    'PredictionSystem',
    'TempEntities',
]


# ============================================================================
# >> GLOBAL VARIABLES
# ============================================================================
# Get the TempEntities instance...
try:
    TempEntities = ServerTools.get_temp_entities()
except NotImplementedError:
    manager.create_global_pointers_from_file(
        SP_DATA_PATH.joinpath('effects', SOURCE_ENGINE, 'pointers.ini'))

    TempEntities = manager.get_global_pointer('_TempEntities')
