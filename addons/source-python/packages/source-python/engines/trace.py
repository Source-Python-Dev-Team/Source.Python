# ../engines/trace.py

"""Provides ray tracing functionality."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Python Imports
#   Enum
from enum import IntFlag

# Source.Python Imports
#   Entities
from entities.helpers import inthandle_from_baseentity


# =============================================================================
# >> FORWARD IMPORTS
# =============================================================================
# Source.Python Imports
#   Engines
from _engines._trace import Ray
from _engines._trace import engine_trace
from _engines._trace import BaseTrace
from _engines._trace import DisplacementFlags
from _engines._trace import GameTrace
from _engines._trace import Surface
from _engines._trace import SurfaceFlags
from _engines._trace import TraceFilter
from _engines._trace import EntityEnumerator
from _engines._trace import TraceType
from _engines._trace import CONTENTS_EMPTY
from _engines._trace import CONTENTS_SOLID
from _engines._trace import CONTENTS_WINDOW
from _engines._trace import CONTENTS_AUX
from _engines._trace import CONTENTS_GRATE
from _engines._trace import CONTENTS_SLIME
from _engines._trace import CONTENTS_WATER
from _engines._trace import CONTENTS_BLOCKLOS
from _engines._trace import CONTENTS_OPAQUE
from _engines._trace import LAST_VISIBLE_CONTENTS
from _engines._trace import ALL_VISIBLE_CONTENTS
from _engines._trace import CONTENTS_TESTFOGVOLUME
from _engines._trace import CONTENTS_UNUSED
from _engines._trace import CONTENTS_TEAM1
from _engines._trace import CONTENTS_TEAM2
from _engines._trace import CONTENTS_IGNORE_NODRAW_OPAQUE
from _engines._trace import CONTENTS_MOVEABLE
from _engines._trace import CONTENTS_AREAPORTAL
from _engines._trace import CONTENTS_PLAYERCLIP
from _engines._trace import CONTENTS_MONSTERCLIP
from _engines._trace import CONTENTS_CURRENT_0
from _engines._trace import CONTENTS_CURRENT_90
from _engines._trace import CONTENTS_CURRENT_180
from _engines._trace import CONTENTS_CURRENT_270
from _engines._trace import CONTENTS_CURRENT_UP
from _engines._trace import CONTENTS_CURRENT_DOWN
from _engines._trace import CONTENTS_ORIGIN
from _engines._trace import CONTENTS_MONSTER
from _engines._trace import CONTENTS_DEBRIS
from _engines._trace import CONTENTS_DETAIL
from _engines._trace import CONTENTS_TRANSLUCENT
from _engines._trace import CONTENTS_LADDER
from _engines._trace import CONTENTS_HITBOX
from _engines._trace import COORD_EXTENT
from _engines._trace import MASK_ALL
from _engines._trace import MASK_SOLID
from _engines._trace import MASK_PLAYERSOLID
from _engines._trace import MASK_NPCSOLID
from _engines._trace import MASK_WATER
from _engines._trace import MASK_OPAQUE
from _engines._trace import MASK_OPAQUE_AND_NPCS
from _engines._trace import MASK_BLOCKLOS
from _engines._trace import MASK_BLOCKLOS_AND_NPCS
from _engines._trace import MASK_VISIBLE
from _engines._trace import MASK_VISIBLE_AND_NPCS
from _engines._trace import MASK_SHOT
from _engines._trace import MASK_SHOT_HULL
from _engines._trace import MASK_SHOT_PORTAL
from _engines._trace import MASK_SOLID_BRUSHONLY
from _engines._trace import MASK_PLAYERSOLID_BRUSHONLY
from _engines._trace import MASK_NPCWORLDSTATIC
from _engines._trace import MASK_SPLITAREAPORTAL
from _engines._trace import MASK_CURRENT
from _engines._trace import MASK_DEADSOLID
from _engines._trace import MAX_COORD_INTEGER
from _engines._trace import MAX_COORD_FRACTION
from _engines._trace import MAX_COORD_FLOAT
from _engines._trace import MAX_TRACE_LENGTH
from _engines._trace import MAX_COORD_RANGE
from _engines._trace import MIN_COORD_INTEGER
from _engines._trace import MIN_COORD_FRACTION
from _engines._trace import MIN_COORD_FLOAT


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('BaseTrace',
           'COORD_EXTENT',
           'ContentFlags',
           'ContentMasks',
           'DisplacementFlags',
           'EntityEnumerator',
           'GameTrace',
           'MAX_COORD_FLOAT',
           'MAX_COORD_FRACTION',
           'MAX_COORD_INTEGER',
           'MAX_COORD_RANGE',
           'MAX_TRACE_LENGTH',
           'MIN_COORD_FLOAT',
           'MIN_COORD_FRACTION',
           'MIN_COORD_INTEGER',
           'Ray',
           'Surface',
           'SurfaceFlags',
           'TraceFilter',
           'TraceFilterSimple',
           'TraceType',
           'engine_trace',
           )


# =============================================================================
# >> ENUMERATORS
# =============================================================================
class ContentFlags(IntFlag):
    """Content flags wrapper enumerator."""

    EMPTY = CONTENTS_EMPTY
    SOLID = CONTENTS_SOLID
    WINDOW = CONTENTS_WINDOW
    AUX = CONTENTS_AUX
    GRATE = CONTENTS_GRATE
    SLIME = CONTENTS_SLIME
    WATER = CONTENTS_WATER
    BLOCKLOS = CONTENTS_BLOCKLOS
    OPAQUE = CONTENTS_OPAQUE
    LAST_VISIBLE = LAST_VISIBLE_CONTENTS
    ALL_VISIBLE = ALL_VISIBLE_CONTENTS
    TESTFOGVOLUME = CONTENTS_TESTFOGVOLUME
    UNUSED = CONTENTS_UNUSED
    TEAM1 = CONTENTS_TEAM1
    TEAM2 = CONTENTS_TEAM2
    IGNORE_NODRAW_OPAQUE = CONTENTS_IGNORE_NODRAW_OPAQUE
    MOVEABLE = CONTENTS_MOVEABLE
    AREAPORTAL = CONTENTS_AREAPORTAL
    PLAYERCLIP = CONTENTS_PLAYERCLIP
    MONSTERCLIP = CONTENTS_MONSTERCLIP
    CURRENT_0 = CONTENTS_CURRENT_0
    CURRENT_90 = CONTENTS_CURRENT_90
    CURRENT_180 = CONTENTS_CURRENT_180
    CURRENT_270 = CONTENTS_CURRENT_270
    CURRENT_UP = CONTENTS_CURRENT_UP
    CURRENT_DOWN = CONTENTS_CURRENT_DOWN
    ORIGIN = CONTENTS_ORIGIN
    MONSTER = CONTENTS_MONSTER
    DEBRIS = CONTENTS_DEBRIS
    DETAIL = CONTENTS_DETAIL
    TRANSLUCENT = CONTENTS_TRANSLUCENT
    LADDER = CONTENTS_LADDER
    HITBOX = CONTENTS_HITBOX


class ContentMasks(IntFlag):
    """Content masks wrapper enumerator."""

    ALL = MASK_ALL
    SOLID = MASK_SOLID
    PLAYER_SOLID = MASK_PLAYERSOLID
    NPC_SOLID = MASK_NPCSOLID
    WATER = MASK_WATER
    OPAQUE = MASK_OPAQUE
    OPAQUE_AND_NPCS = MASK_OPAQUE_AND_NPCS
    BLOCKLOS = MASK_BLOCKLOS
    BLOCKLOS_AND_NPCS = MASK_BLOCKLOS_AND_NPCS
    VISIBLE = MASK_VISIBLE
    VISIBLE_AND_NPCS = MASK_VISIBLE_AND_NPCS
    SHOT = MASK_SHOT
    SHOT_HULL = MASK_SHOT_HULL
    SHOT_PORTAL = MASK_SHOT_PORTAL
    SOLID_BRUSH_ONLY = MASK_SOLID_BRUSHONLY
    PLAYER_SOLID_BRUSH_ONLY = MASK_PLAYERSOLID_BRUSHONLY
    NPC_WORLD_STATIC = MASK_NPCWORLDSTATIC
    SPLIT_AREA_PORTAL = MASK_SPLITAREAPORTAL
    CURRENT = MASK_CURRENT
    DEAD_SOLID = MASK_DEADSOLID


# =============================================================================
# >> CLASSES
# =============================================================================
class TraceFilterSimple(TraceFilter):
    """A simple trace filter."""

    def __init__(self, ignore=(), trace_type=TraceType.EVERYTHING):
        """Initialize the filter.

        :param iterable ignore:
            An iterable of entity indexes to ignore. The trace will not hit
            these entities.
        :param TraceType trace_type:
            The trace type that should be used.
        """
        super().__init__()
        self.trace_type = trace_type
        self.ignore = set(map(inthandle_from_baseentity, ignore))

    def should_hit_entity(self, entity, mask):
        """Called when a trace is about to hit an entity.

        :param HandleEntity entity:
            The entity that should be hit.
        :param int mask:
            The mask that was used to intialize the trace.
        :rtype: bool
        """
        return entity.basehandle.to_int() not in self.ignore

    def get_trace_type(self):
        """Return the trace type.

        :rtype: TraceType
        """
        return self.trace_type
