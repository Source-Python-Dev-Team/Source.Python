# ../effects/base.py

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
#   Memory
from memory.manager import manager


# =============================================================================
# >> FORWARD IMPORTS
# =============================================================================
# Source.Python Imports
#   Effects
from _effects._base import BaseTempEntity


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('BaseTempEntity',
           )


# =============================================================================
# >> GLOBAL VARIABLES
# =============================================================================
# Get the first temp entity in the chain...
try:
    from _effects._base import _first_temp_entity
except ImportError:
    _first_temp_entity = manager.get_global_pointer('BaseTempEntity')
