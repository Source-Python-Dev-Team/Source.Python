# ../entities/constants.py

"""Provides constant values that are entity based."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Python Imports
#   Enum
from enum import IntEnum
from enum import IntFlag


# =============================================================================
# >> FORWARD IMPORTS
# =============================================================================
# Source.Python Imports
#   Entities
from _entities._constants import BASEDAMAGE_NOT_SPECIFIED
from _entities._constants import INVALID_ENTITY_INDEX
from _entities._constants import INVALID_ENTITY_INTHANDLE
from _entities._constants import CollisionGroup
from _entities._constants import DAMAGE_NO
from _entities._constants import DAMAGE_EVENTS_ONLY
from _entities._constants import DAMAGE_YES
from _entities._constants import DAMAGE_AIM
from _entities._constants import DMG_GENERIC
from _entities._constants import DMG_CRUSH
from _entities._constants import DMG_BULLET
from _entities._constants import DMG_SLASH
from _entities._constants import DMG_BURN
from _entities._constants import DMG_VEHICLE
from _entities._constants import DMG_FALL
from _entities._constants import DMG_BLAST
from _entities._constants import DMG_CLUB
from _entities._constants import DMG_SHOCK
from _entities._constants import DMG_SONIC
from _entities._constants import DMG_ENERGYBEAM
from _entities._constants import DMG_PREVENT_PHYSICS_FORCE
from _entities._constants import DMG_NEVERGIB
from _entities._constants import DMG_ALWAYSGIB
from _entities._constants import DMG_DROWN
from _entities._constants import DMG_PARALYZE
from _entities._constants import DMG_NERVEGAS
from _entities._constants import DMG_POISON
from _entities._constants import DMG_RADIATION
from _entities._constants import DMG_DROWNRECOVER
from _entities._constants import DMG_ACID
from _entities._constants import DMG_SLOWBURN
from _entities._constants import DMG_REMOVENORAGDOLL
from _entities._constants import DMG_PHYSGUN
from _entities._constants import DMG_PLASMA
from _entities._constants import DMG_AIRBOAT
from _entities._constants import DMG_DISSOLVE
from _entities._constants import DMG_BLAST_SURFACE
from _entities._constants import DMG_DIRECT
from _entities._constants import DMG_BUCKSHOT
from _entities._constants import DMG_LASTGENERICFLAG
from _entities._constants import DMG_HEADSHOT
from _entities._constants import EF_BONEMERGE
from _entities._constants import EF_BRIGHTLIGHT
from _entities._constants import EF_DIMLIGHT
from _entities._constants import EF_NOINTERP
from _entities._constants import EF_NOSHADOW
from _entities._constants import EF_NODRAW
from _entities._constants import EF_NORECEIVESHADOW
from _entities._constants import EF_BONEMERGE_FASTCULL
from _entities._constants import EF_ITEM_BLINK
from _entities._constants import EF_PARENT_ANIMATES
from _entities._constants import EF_MARKED_FOR_FAST_REFLECTION
from _entities._constants import EF_NOSHADOWDEPTH
from _entities._constants import EF_NOFLASHLIGHT
from _entities._constants import EFL_KILLME
from _entities._constants import EFL_DORMANT
from _entities._constants import EFL_NOCLIP_ACTIVE
from _entities._constants import EFL_SETTING_UP_BONES
from _entities._constants import EFL_KEEP_ON_RECREATE_ENTITIES
from _entities._constants import EFL_DIRTY_SHADOWUPDATE
from _entities._constants import EFL_NOTIFY
from _entities._constants import EFL_FORCE_CHECK_TRANSMIT
from _entities._constants import EFL_BOT_FROZEN
from _entities._constants import EFL_SERVER_ONLY
from _entities._constants import EFL_NO_AUTO_EDICT_ATTACH
from _entities._constants import EFL_DIRTY_ABSTRANSFORM
from _entities._constants import EFL_DIRTY_ABSVELOCITY
from _entities._constants import EFL_DIRTY_ABSANGVELOCITY
from _entities._constants import EFL_DIRTY_SURROUNDING_COLLISION_BOUNDS
from _entities._constants import EFL_DIRTY_SPATIAL_PARTITION
from _entities._constants import EFL_HAS_PLAYER_CHILD
from _entities._constants import EFL_IN_SKYBOX
from _entities._constants import EFL_USE_PARTITION_WHEN_NOT_SOLID
from _entities._constants import EFL_TOUCHING_FLUID
from _entities._constants import EFL_IS_BEING_LIFTED_BY_BARNACLE
from _entities._constants import EFL_NO_ROTORWASH_PUSH
from _entities._constants import EFL_NO_THINK_FUNCTION
from _entities._constants import EFL_NO_GAME_PHYSICS_SIMULATION
from _entities._constants import EFL_CHECK_UNTOUCH
from _entities._constants import EFL_DONTBLOCKLOS
from _entities._constants import EFL_DONTWALKON
from _entities._constants import EFL_NO_DISSOLVE
from _entities._constants import EFL_NO_MEGAPHYSCANNON_RAGDOLL
from _entities._constants import EFL_NO_WATER_VELOCITY_CHANGE
from _entities._constants import EFL_NO_PHYSCANNON_INTERACTION
from _entities._constants import EFL_NO_DAMAGE_FORCES
from _entities._constants import ENTITY_DISSOLVE_NORMAL
from _entities._constants import ENTITY_DISSOLVE_ELECTRICAL
from _entities._constants import ENTITY_DISSOLVE_ELECTRICAL_LIGHT
from _entities._constants import ENTITY_DISSOLVE_CORE
from _entities._constants import FL_FLY
from _entities._constants import FL_SWIM
from _entities._constants import FL_CONVEYOR
from _entities._constants import FL_NPC
from _entities._constants import FL_GODMODE
from _entities._constants import FL_NOTARGET
from _entities._constants import FL_AIMTARGET
from _entities._constants import FL_PARTIALGROUND
from _entities._constants import FL_STATICPROP
from _entities._constants import FL_GRAPHED
from _entities._constants import FL_GRENADE
from _entities._constants import FL_STEPMOVEMENT
from _entities._constants import FL_DONTTOUCH
from _entities._constants import FL_BASEVELOCITY
from _entities._constants import FL_WORLDBRUSH
from _entities._constants import FL_OBJECT
from _entities._constants import FL_KILLME
from _entities._constants import FL_ONFIRE
from _entities._constants import FL_DISSOLVING
from _entities._constants import FL_TRANSRAGDOLL
from _entities._constants import FL_UNBLOCKABLE_BY_PLAYER
from _entities._constants import MoveCollide
from _entities._constants import MoveType
from _entities._constants import RenderEffects
from _entities._constants import RenderMode
from _entities._constants import SolidFlags
from _entities._constants import SolidType
from _entities._constants import WORLD_ENTITY_INDEX


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('BASEDAMAGE_NOT_SPECIFIED',
           'CollisionGroup',
           'DamageTypes',
           'DissolveType',
           'EntityEffects',
           'EntityFlags',
           'EntityStates',
           'INVALID_ENTITY_INDEX',
           'INVALID_ENTITY_INTHANDLE',
           'MoveCollide',
           'MoveType',
           'RenderEffects',
           'RenderMode',
           'SolidFlags',
           'SolidType',
           'TakeDamage',
           'WORLD_ENTITY_INDEX',
           )


# =============================================================================
# >> ENUMERATORS
# =============================================================================
class DamageTypes(IntFlag):
    """Damage types wrapper enumerator."""

    GENERIC = DMG_GENERIC
    CRUSH = DMG_CRUSH
    BULLET = DMG_BULLET
    SLASH = DMG_SLASH
    BURN = DMG_BURN
    VEHICLE = DMG_VEHICLE
    FALL = DMG_FALL
    BLAST = DMG_BLAST
    CLUB = DMG_CLUB
    SHOCK = DMG_SHOCK
    SONIC = DMG_SONIC
    ENERGYBEAM = DMG_ENERGYBEAM
    PREVENT_PHYSICS_FORCE = DMG_PREVENT_PHYSICS_FORCE
    NEVERGIB = DMG_NEVERGIB
    ALWAYSGIB = DMG_ALWAYSGIB
    DROWN = DMG_DROWN
    PARALYZE = DMG_PARALYZE
    NERVEGAS = DMG_NERVEGAS
    POISON = DMG_POISON
    RADIATION = DMG_RADIATION
    DROWNRECOVER = DMG_DROWNRECOVER
    ACID = DMG_ACID
    SLOWBURN = DMG_SLOWBURN
    REMOVENORAGDOLL = DMG_REMOVENORAGDOLL
    PHYSGUN = DMG_PHYSGUN
    PLASMA = DMG_PLASMA
    AIRBOAT = DMG_AIRBOAT
    DISSOLVE = DMG_DISSOLVE
    BLAST_SURFACE = DMG_BLAST_SURFACE
    DIRECT = DMG_DIRECT
    BUCKSHOT = DMG_BUCKSHOT
    LASTGENERICFLAG = DMG_LASTGENERICFLAG
    HEADSHOT = DMG_HEADSHOT


class EntityStates(IntFlag):
    """Entity state flags wrapper enumerator."""

    FLY = FL_FLY
    SWIM = FL_SWIM
    CONVEYOR = FL_CONVEYOR
    NPC = FL_NPC
    GODMODE = FL_GODMODE
    NOTARGET = FL_NOTARGET
    AIMTARGET = FL_AIMTARGET
    PARTIALGROUND = FL_PARTIALGROUND
    STATICPROP = FL_STATICPROP
    GRAPHED = FL_GRAPHED
    GRENADE = FL_GRENADE
    STEPMOVEMENT = FL_STEPMOVEMENT
    DONTTOUCH = FL_DONTTOUCH
    BASEVELOCITY = FL_BASEVELOCITY
    WORLDBRUSH = FL_WORLDBRUSH
    OBJECT = FL_OBJECT
    KILLME = FL_KILLME
    ONFIRE = FL_ONFIRE
    DISSOLVING = FL_DISSOLVING
    TRANSRAGDOLL = FL_TRANSRAGDOLL
    UNBLOCKABLE_BY_PLAYER = FL_UNBLOCKABLE_BY_PLAYER


class EntityEffects(IntFlag):
    """Entity effects wrapper enumerator."""

    BONEMERGE = EF_BONEMERGE
    BRIGHTLIGHT = EF_BRIGHTLIGHT
    DIMLIGHT = EF_DIMLIGHT
    NOINTERP = EF_NOINTERP
    NOSHADOW = EF_NOSHADOW
    NODRAW = EF_NODRAW
    NORECEIVESHADOW = EF_NORECEIVESHADOW
    BONEMERGE_FASTCULL = EF_BONEMERGE_FASTCULL
    ITEM_BLINK = EF_ITEM_BLINK
    PARENT_ANIMATES = EF_PARENT_ANIMATES
    MARKED_FOR_FAST_REFLECTION = EF_MARKED_FOR_FAST_REFLECTION
    NOSHADOWDEPTH = EF_NOSHADOWDEPTH
    NOFLASHLIGHT = EF_NOFLASHLIGHT


class DissolveType(IntEnum):
    """Dissolve type wrapper enumerator."""

    NORMAL = ENTITY_DISSOLVE_NORMAL
    ELECTRICAL = ENTITY_DISSOLVE_ELECTRICAL
    ELECTRICAL_LIGHT = ENTITY_DISSOLVE_ELECTRICAL_LIGHT
    CORE = ENTITY_DISSOLVE_CORE


class TakeDamage(IntEnum):
    """Take damage constants wrapper enumerator."""

    NO = DAMAGE_NO
    EVENTS_ONLY = DAMAGE_EVENTS_ONLY
    YES = DAMAGE_YES
    AIM = DAMAGE_AIM


class EntityFlags(IntFlag):
    """Entity flags wrapper enumerator."""

    KILLME = EFL_KILLME
    DORMANT = EFL_DORMANT
    NOCLIP_ACTIVE = EFL_NOCLIP_ACTIVE
    SETTING_UP_BONES = EFL_SETTING_UP_BONES
    KEEP_ON_RECREATE_ENTITIES = EFL_KEEP_ON_RECREATE_ENTITIES
    DIRTY_SHADOWUPDATE = EFL_DIRTY_SHADOWUPDATE
    NOTIFY = EFL_NOTIFY
    FORCE_CHECK_TRANSMIT = EFL_FORCE_CHECK_TRANSMIT
    BOT_FROZEN = EFL_BOT_FROZEN
    SERVER_ONLY = EFL_SERVER_ONLY
    NO_AUTO_EDICT_ATTACH = EFL_NO_AUTO_EDICT_ATTACH
    DIRTY_ABSTRANSFORM = EFL_DIRTY_ABSTRANSFORM
    DIRTY_ABSVELOCITY = EFL_DIRTY_ABSVELOCITY
    DIRTY_ABSANGVELOCITY = EFL_DIRTY_ABSANGVELOCITY
    DIRTY_SURROUNDING_COLLISION_BOUNDS = EFL_DIRTY_SURROUNDING_COLLISION_BOUNDS
    DIRTY_SPATIAL_PARTITION = EFL_DIRTY_SPATIAL_PARTITION
    HAS_PLAYER_CHILD = EFL_HAS_PLAYER_CHILD
    IN_SKYBOX = EFL_IN_SKYBOX
    USE_PARTITION_WHEN_NOT_SOLID = EFL_USE_PARTITION_WHEN_NOT_SOLID
    TOUCHING_FLUID = EFL_TOUCHING_FLUID
    IS_BEING_LIFTED_BY_BARNACLE = EFL_IS_BEING_LIFTED_BY_BARNACLE
    NO_ROTORWASH_PUSH = EFL_NO_ROTORWASH_PUSH
    NO_THINK_FUNCTION = EFL_NO_THINK_FUNCTION
    NO_GAME_PHYSICS_SIMULATION = EFL_NO_GAME_PHYSICS_SIMULATION
    CHECK_UNTOUCH = EFL_CHECK_UNTOUCH
    DONTBLOCKLOS = EFL_DONTBLOCKLOS
    DONTWALKON = EFL_DONTWALKON
    NO_DISSOLVE = EFL_NO_DISSOLVE
    NO_MEGAPHYSCANNON_RAGDOLL = EFL_NO_MEGAPHYSCANNON_RAGDOLL
    NO_WATER_VELOCITY_CHANGE = EFL_NO_WATER_VELOCITY_CHANGE
    NO_PHYSCANNON_INTERACTION = EFL_NO_PHYSCANNON_INTERACTION
    NO_DAMAGE_FORCES = EFL_NO_DAMAGE_FORCES
