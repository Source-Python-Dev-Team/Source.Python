# ../entities/constants.py

# =============================================================================
# >> IMPORTS
# =============================================================================
# Site Package Imports
#   ConfigObj
from configobj import ConfigObj

# Source.Python Imports
from core import GAME_NAME
from paths import SP_DATA_PATH
from public import public


# =============================================================================
# >> GLOBAL VARIABLES
# =============================================================================
_entity_values = ConfigObj(
    SP_DATA_PATH.joinpath('entities', GAME_NAME + '.ini'), unrepr=True)


# =============================================================================
# >> CLASSES
# =============================================================================
class _ConstantBase(dict):
    '''Base constant class used to not allow setting values'''

    def __contains__(self, item):
        '''Override __contains__ to return whether the constant is valid'''
        return hasattr(self, item)

    def __getitem__(self, item):
        '''Override __getitem__ to return the attribute's value'''
        return self.__getattr__(item)

    def __setitem__(self, item, value):
        '''Override __setitem__ to not allow setting any values'''
        return

    def __setattr__(self, item, value):
        '''Override __setattr__ to not allow setting any values'''
        return


@public
class _DamageTypes(_ConstantBase):
    '''Class used to easily get damage type values by name'''

    def __getattr__(self, attr):
        '''Override __getattr__ to retrieve the value of the constant'''

        # Is the attribute a proper constant?
        if ('damage_types' in _entity_values
                and attr in _entity_values['damage_types']):

            # Return the constant's value
            return _entity_values['damage_types'][attr]

        # Return 0 if the constant is not found
        return 0

# Get the _DamageTypes instance
DamageTypes = _DamageTypes()


@public
class _DamageOffsets(_ConstantBase):
    '''Class used to retrieve offset values for CBaseEntity::TakeDamage'''

    def __getattr__(self, attr):
        '''Override __getattr__ to retrieve the value of the offset'''

        # Is the attribute supported?
        if 'damage' in _entity_values and attr in _entity_values['damage']:

            # Return the offset value
            return _entity_values['damage'][attr]

        # Raise an AttributeError so that hasattr returns False
        raise AttributeError('Offset "{0}" not found'.format(attr))

# Get the _DamageOffsets instance
DamageOffsets = _DamageOffsets()
