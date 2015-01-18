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
# >> ALL DECLARATION
# =============================================================================
__all__ = ('DATA_DESC_MAP_OFFSET',
           'damage_types',
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

# Get the damage_types for the current engine
damage_types = IntEnum('damage_types', _entity_values.get('damage_types', {}))
