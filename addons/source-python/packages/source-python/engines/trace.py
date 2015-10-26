# ../engines/trace.py

"""Provides ray tracing functionality."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Python Imports
#   Enum
from enum import IntEnum

# Source.Python Imports
#   Entities
from entities.helpers import inthandle_from_index


# =============================================================================
# >> FORWARD IMPORTS
# =============================================================================
# Source.Python Imports
#   Engines
from _engines import Ray
from _engines import engine_trace
from _engines import BaseTrace
from _engines import DISPSURF_FLAG_SURFACE
from _engines import DISPSURF_FLAG_WALKABLE
from _engines import DISPSURF_FLAG_BUILDABLE
from _engines import DISPSURF_FLAG_SURFPROP1
from _engines import DISPSURF_FLAG_SURFPROP2
from _engines import GameTrace
from _engines import Surface
from _engines import TraceFilter
from _engines import EntityEnumerator
from _engines import TraceType
from _engines import CONTENTS_EMPTY
from _engines import CONTENTS_SOLID
from _engines import CONTENTS_WINDOW
from _engines import CONTENTS_AUX
from _engines import CONTENTS_GRATE
from _engines import CONTENTS_SLIME
from _engines import CONTENTS_WATER
from _engines import CONTENTS_BLOCKLOS
from _engines import CONTENTS_OPAQUE
from _engines import LAST_VISIBLE_CONTENTS
from _engines import ALL_VISIBLE_CONTENTS
from _engines import CONTENTS_TESTFOGVOLUME
from _engines import CONTENTS_UNUSED
from _engines import CONTENTS_TEAM1
from _engines import CONTENTS_TEAM2
from _engines import CONTENTS_IGNORE_NODRAW_OPAQUE
from _engines import CONTENTS_MOVEABLE
from _engines import CONTENTS_AREAPORTAL
from _engines import CONTENTS_PLAYERCLIP
from _engines import CONTENTS_MONSTERCLIP
from _engines import CONTENTS_CURRENT_0
from _engines import CONTENTS_CURRENT_90
from _engines import CONTENTS_CURRENT_180
from _engines import CONTENTS_CURRENT_270
from _engines import CONTENTS_CURRENT_UP
from _engines import CONTENTS_CURRENT_DOWN
from _engines import CONTENTS_ORIGIN
from _engines import CONTENTS_MONSTER
from _engines import CONTENTS_DEBRIS
from _engines import CONTENTS_DETAIL
from _engines import CONTENTS_TRANSLUCENT
from _engines import CONTENTS_LADDER
from _engines import CONTENTS_HITBOX
from _engines import COORD_EXTENT
from _engines import MASK_ALL
from _engines import MASK_SOLID
from _engines import MASK_PLAYERSOLID
from _engines import MASK_NPCSOLID
from _engines import MASK_WATER
from _engines import MASK_OPAQUE
from _engines import MASK_OPAQUE_AND_NPCS
from _engines import MASK_BLOCKLOS
from _engines import MASK_BLOCKLOS_AND_NPCS
from _engines import MASK_VISIBLE
from _engines import MASK_VISIBLE_AND_NPCS
from _engines import MASK_SHOT
from _engines import MASK_SHOT_HULL
from _engines import MASK_SHOT_PORTAL
from _engines import MASK_SOLID_BRUSHONLY
from _engines import MASK_PLAYERSOLID_BRUSHONLY
from _engines import MASK_NPCWORLDSTATIC
from _engines import MASK_SPLITAREAPORTAL
from _engines import MASK_CURRENT
from _engines import MASK_DEADSOLID
from _engines import MAX_COORD_INTEGER
from _engines import MAX_COORD_FRACTION
from _engines import MAX_COORD_FLOAT
from _engines import MAX_TRACE_LENGTH
from _engines import MAX_COORD_RANGE
from _engines import MIN_COORD_INTEGER
from _engines import MIN_COORD_FRACTION
from _engines import MIN_COORD_FLOAT
from _engines import SURF_LIGHT
from _engines import SURF_SKY2D
from _engines import SURF_SKY
from _engines import SURF_WARP
from _engines import SURF_TRANS
from _engines import SURF_NOPORTAL
from _engines import SURF_TRIGGER
from _engines import SURF_NODRAW
from _engines import SURF_HINT
from _engines import SURF_SKIP
from _engines import SURF_NOLIGHT
from _engines import SURF_BUMPLIGHT
from _engines import SURF_NOSHADOWS
from _engines import SURF_NODECALS
from _engines import SURF_NOCHOP
from _engines import SURF_HITBOX


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
class DisplacementFlags(IntEnum):
    """Displacement flags wrapper enumerator."""

    SURFACE = DISPSURF_FLAG_SURFACE
    WALKABLE = DISPSURF_FLAG_WALKABLE
    BUILDABLE = DISPSURF_FLAG_BUILDABLE
    SURFPROP1 = DISPSURF_FLAG_SURFPROP1
    SURFPROP2 = DISPSURF_FLAG_SURFPROP2


class ContentFlags(IntEnum):
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


class ContentMasks(IntEnum):
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


class SurfaceFlags(IntEnum):
    """Surface flags wrapper enumerator."""

    LIGHT = SURF_LIGHT
    SKY2D = SURF_SKY2D
    SKY = SURF_SKY
    WARP = SURF_WARP
    TRANS = SURF_TRANS
    NOPORTAL = SURF_NOPORTAL
    TRIGGER = SURF_TRIGGER
    NODRAW = SURF_NODRAW
    HINT = SURF_HINT
    SKIP = SURF_SKIP
    NOLIGHT = SURF_NOLIGHT
    BUMPLIGHT = SURF_BUMPLIGHT
    NOSHADOWS = SURF_NOSHADOWS
    NODECALS = SURF_NODECALS
    NOCHOP = SURF_NOCHOP
    HITBOX = SURF_HITBOX


# =============================================================================
# >> CLASSES
# =============================================================================
class TraceFilterSimple(TraceFilter):
    """A simple trace filter."""

    def __init__(self, ignore=(), trace_type=TraceType.EVERYTHING):
        """Initialize the filter.

        :param iterable ignore: An iterable of entity indexes to ignore. The
            trace will not hit these entities.
        :param TraceType trace_type: The trace type that should be used.
        """
        super().__init__()
        self.trace_type = trace_type
        self.ignore = tuple(map(inthandle_from_index, ignore))

    def should_hit_entity(self, entity, mask):
        """Called when a trace is about to hit an entity.

        :param HandleEntity entity: The entity that should be hit.
        :param int mask: The mask that was used to intialize the trace.
        """
        entity_inthandle = entity.get_ref_ehandle().to_int()

        # Check for entities to ignore
        for ignore_inthandle in self.ignore:
            if ignore_inthandle == entity_inthandle:
                return False

        return True

    def get_trace_type(self):
        """Return the trace type."""
        return self.trace_type
