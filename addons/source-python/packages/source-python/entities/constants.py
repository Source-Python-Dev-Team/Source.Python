# ../entities/constants.py

"""Provides constant values that are entity based."""

# =============================================================================
# >> IMPORTS
# =============================================================================
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


# =============================================================================
# >> CLASSES
# =============================================================================
class _ConstantBase(dict):

    """Base constant class used to not allow setting values."""

    def __contains__(self, item):
        """Override __contains__ to return whether the constant is valid."""
        return hasattr(self, item)

    def __getitem__(self, item):
        """Override __getitem__ to return the attribute's value."""
        return self.__getattr__(item)

    def __setitem__(self, item, value):
        """Override __setitem__ to not allow setting any values."""
        return

    def __setattr__(self, item, value):
        """Override __setattr__ to not allow setting any values."""
        return


class _DamageTypes(_ConstantBase):

    """Class used to easily get damage type values by name."""

    def __getattr__(self, attr):
        """Override __getattr__ to retrieve the value of the constant."""
        # Is the attribute a proper constant?
        if ('damage_types' in _entity_values
                and attr in _entity_values['damage_types']):

            # Return the constant's value
            return _entity_values['damage_types'][attr]

        # Return 0 if the constant is not found
        return 0

# Get the _DamageTypes instance
damage_types = _DamageTypes()
