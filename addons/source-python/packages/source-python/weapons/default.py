# ../weapons/default.py

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
#   Core
from core import GAME_NAME


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
# Set all to an empty list
__all__ = []


# =============================================================================
# >> CLASSES
# =============================================================================
class NoWeaponManager(object):
    '''Default class to use if no game specific weapon ini file is found'''

    def __getattribute__(self, attr):
        '''Raise an error when trying to get an attribute'''
        raise NotImplementedError(
            'No support for game "{0}"'.format(GAME_NAME))

    def __setattr__(self, attr, value):
        '''Raise an error when trying to set an attribute'''
        raise NotImplementedError(
            'No support for game "{0}"'.format(GAME_NAME))
