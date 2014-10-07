# ../entities/constants.py

"""Provides constant values that are entity based."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Python Imports
#   Enum
from enum import Enum

# Site-Package Imports
#   ConfigObj
from configobj import ConfigObj

# Source.Python Imports
#   Core
from core import PLATFORM
from core import SOURCE_ENGINE
#   Memory
from memory.manager import manager
#   Paths
from paths import SP_DATA_PATH


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('CTakeDamageInfo',
           'DATA_DESC_MAP_OFFSET',
           'damage_types',
           )


# =============================================================================
# >> GLOBAL VARIABLES
# =============================================================================
# Get the path to the constant values
_entity_values = ConfigObj(SP_DATA_PATH.joinpath(
    'entities', 'constants', SOURCE_ENGINE + '.ini'), unrepr=True)

# Get the path to the game's ctakedamageinfo.ini file
_damage_info_path = SP_DATA_PATH.joinpath(
    'entities', 'types', SOURCE_ENGINE, 'ctakedamageinfo.ini')


# =============================================================================
# >> CONSTANT VALUES
# =============================================================================
# Get the GetDataDescMap offset
DATA_DESC_MAP_OFFSET = _entity_values.get(
    'DATA_DESC_OFFSET', {}).get(PLATFORM, None)

# Does the ctakedamageinfo.ini file exist?
if _damage_info_path.isfile():

    # Create the CTakeDamageInfo object
    CTakeDamageInfo = manager.create_type_from_file(
        'CTakeDamageInfo', _damage_info_path)

# Does the file not exist?
else:
    CTakeDamageInfo = None

# Get the damage_types for the current engine
damage_types = Enum('damage_types', _entity_values.get('damage_types', {}))
