# ../weapons/scripts.py

"""Provides weapon scripts functionalities."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
#   Core
from core import GameConfigObj
#   Paths
from paths import SP_DATA_PATH
#   Memory
from memory import make_object
from memory.manager import manager


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('WeaponInfo',
           'weapon_scripts',
           )


# =============================================================================
# >> GLOBAL VARIABLES
# =============================================================================
# Get the WeaponInfo class...
WeaponInfo = manager.create_type_from_dict('WeaponInfo', GameConfigObj(
    SP_DATA_PATH / 'weapons' / 'scripts' / 'WeaponInfo.ini'))

# Get the _WeaponDatabase class...
try:
    from _weapons._scripts import _WeaponDatabase

    # Make sure the manager is able to find us...
    manager['_WeaponDatabase'] = _WeaponDatabase
except ImportError:
    _WeaponDatabase = manager.create_type_from_dict(
        '_WeaponDatabase', GameConfigObj(
            SP_DATA_PATH / 'weapons' / 'scripts' / 'WeaponDatabase.ini'))

# Get the global _WeaponDatabase pointer...
manager.create_global_pointers_from_file(GameConfigObj(
    SP_DATA_PATH / 'weapons' / 'scripts' / 'global_pointers.ini'))
_weapon_scripts = manager.get_global_pointer('_WeaponDatabase')


# =============================================================================
# >> CLASSES
# =============================================================================
class WeaponDatabase(object):
    """WeaponDatabase accessor class."""

    def __len__(self):
        """Return the length of the database."""
        return _weapon_scripts._length

    def __getitem__(self, item):
        """Return the WeaponInfo instance matching the given class name."""
        # Loop through all WeaponInfo instances...
        for info in self:

            # Are the class names not matching?
            if info.class_name != item:

                # If so, skip it...
                continue

            # Return the current WeaponInfo instance...
            return info

        # No match, raise an error...
        raise NameError('"{0}" is not a valid weapon name.'.format(item))

    def __iter__(self):
        """Iterate over all WeaponInfo instances."""
        # Loop through all indexes...
        for index in range(_weapon_scripts._length):

            # Yield the WeaponInfo instance at the current slot...
            yield make_object(WeaponInfo, _weapon_scripts._find(index))

# Global database...
weapon_scripts = WeaponDatabase()
