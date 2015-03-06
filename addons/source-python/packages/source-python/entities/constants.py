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


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('DATA_DESC_MAP_OFFSET',
           'DamageTypes',
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

    NOT_SPECIFIED = BASEDAMAGE_NOT_SPECIFIED
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
