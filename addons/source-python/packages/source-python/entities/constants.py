# ../entities/constants.py

"""Provides constant values that are entity based."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Python Imports
#   Enum
from enum import IntEnum

# Site-Package Imports
#   ConfigObj
from configobj import ConfigObj

# Source.Python Imports
#   Core
from core import PLATFORM
from core import SOURCE_ENGINE
#   Paths
from paths import SP_DATA_PATH


# =============================================================================
# >> FORWARD IMPORTS
# =============================================================================
# Source.Python Imports
#   Entities
from _entities._constants import BASEDAMAGE_NOT_SPECIFIED
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

# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('BASEDAMAGE_NOT_SPECIFIED',
           'DamageTypes',
           'DATA_DESC_MAP_OFFSET',
           'EntityFlags',
           )


# =============================================================================
# >> GLOBAL VARIABLES
# =============================================================================
# Get the path to the constant values
_entity_values = ConfigObj(SP_DATA_PATH.joinpath(
    'entities', 'constants', SOURCE_ENGINE + '.ini'), unrepr=True)


# =============================================================================
# >> CONSTANT VALUES
# =============================================================================
# Get the GetDataDescMap offset
DATA_DESC_MAP_OFFSET = _entity_values.get(
    'DATA_DESC_OFFSET', {}).get(PLATFORM, None)


# =============================================================================
# >> ENUMERATORS
# =============================================================================
class DamageTypes(IntEnum):

    """Damage types wrapper enumerator."""

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


class EntityFlags(IntEnum):

    """Entity flags wrapper enumerator."""

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