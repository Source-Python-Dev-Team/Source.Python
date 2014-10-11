# ../entities/helpers.py

"""Provides helper functions to convert from one type to another."""

# =============================================================================
# >> FORWARD IMPORTS
# =============================================================================
# Source.Python Imports
#   Conversions
from _conversions import basehandle_from_edict
from _conversions import basehandle_from_index
from _conversions import basehandle_from_inthandle
from _conversions import basehandle_from_pointer
from _conversions import edict_from_basehandle
from _conversions import edict_from_index
from _conversions import edict_from_inthandle
from _conversions import edict_from_pointer
from _conversions import index_from_basehandle
from _conversions import index_from_edict
from _conversions import index_from_inthandle
from _conversions import index_from_pointer
from _conversions import inthandle_from_basehandle
from _conversions import inthandle_from_edict
from _conversions import inthandle_from_index
from _conversions import inthandle_from_pointer
from _conversions import pointer_from_basehandle
from _conversions import pointer_from_edict
from _conversions import pointer_from_index
from _conversions import pointer_from_inthandle
#   Entities
from _entities import create_entity
from _entities import spawn_entity


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('basehandle_from_edict',
           'basehandle_from_index',
           'basehandle_from_inthandle',
           'basehandle_from_pointer',
           'create_entity',
           'edict_from_basehandle',
           'edict_from_index',
           'edict_from_inthandle',
           'edict_from_pointer',
           'index_from_basehandle',
           'index_from_edict',
           'index_from_inthandle',
           'index_from_pointer',
           'inthandle_from_basehandle',
           'inthandle_from_edict',
           'inthandle_from_index',
           'inthandle_from_pointer',
           'pointer_from_basehandle',
           'pointer_from_edict',
           'pointer_from_index',
           'pointer_from_inthandle',
           'spawn_entity',
           )
