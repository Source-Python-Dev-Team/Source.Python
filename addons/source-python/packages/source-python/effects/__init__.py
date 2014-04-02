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
from memory.pointers import Pointers


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
    TempEntities = Pointers(SP_DATA_PATH.joinpath(
        'effects', SOURCE_ENGINE, 'pointers.ini'))['TempEntities']