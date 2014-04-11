# ../effects/__init__.py

# ============================================================================
# >> IMPORTS
# ============================================================================
# Source.Python Imports
from tools_c import ServerTools
from paths import SP_DATA_PATH
#   Constants
from constants_c import SOURCE_ENGINE
#   Effects
from effects_c import *
#   Memory
from memory.manager import manager


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = [
    'PredictionSystem',
    'Effects',
    'DispatchEffectData',
    'TempEntities'
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
