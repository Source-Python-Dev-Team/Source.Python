# ../weapons/manager.py

"""Provides weapon information for the current game."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Site-Package Imports
#   Configobj
from configobj import ConfigObj

# Source.Python Imports
#   Core
from core import GAME_NAME
#   Paths
from paths import SP_DATA_PATH
#   Weapons
from weapons.default import NoWeaponManager
from weapons.instance import WeaponClass


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('_WeaponManager',
           'weapon_manager',
           )


# =============================================================================
# >> GLOBAL VARIABLES
# =============================================================================
# Get the game's ini path
_gamepath = SP_DATA_PATH / 'weapons' / GAME_NAME + '.ini'


# =============================================================================
# >> CLASSES
# =============================================================================
class _WeaponManager(dict):
    """Dictionary class to store basic weapon information."""

    def __init__(self):
        """Load the ini file into the dictionary."""
        # Initialize the dictionary
        super().__init__()

        # Get the ConfigObj instance of the file
        self.ini = ConfigObj(_gamepath, unrepr=True)

        # Get the "properties"
        properties = self.ini['properties']

        # Get the game's weapon prefix
        self._prefix = properties['prefix']

        # Get the game's m_iAmmo property
        self._ammoprop = properties['ammoprop']

        # Get the game's m_hMyWeapons property
        self._myweapons = properties['myweapons']

        # Store any special names
        self._special_names = self.ini.get('special names', {})

        # Store projectile names
        self._projectiles = self.ini.get('projectiles', {})

        # Store tags as a set
        self._tags = set()

        # Loop through all weapons
        for basename in self.ini['weapons']:

            # Get the weapon's full name
            name = self._format_name(basename)

            # Add the weapon to the dictionary
            self[name] = WeaponClass(
                name, basename, self.ini['weapons'][basename]
            )

            # Add the weapon's tags to the set of tags
            self._tags.update(self[name].tags)

    def __getitem__(self, item):
        """Return the :class:`weapons.instance.WeaponClass` for the weapon.

        :param str item: The weapon to retrieve the instance of.
        :rtype: WeaponClass
        """
        name = self._format_name(item)
        return super().__getitem__(name)

    def __contains__(self, item):
        """Return whether the weapon is in the manager.

        :param str item: The weapon to retrieve the instance of.
        :rtype: WeaponClass
        """
        name = self._format_name(item)
        return super().__contains__(name)

    def get(self, item, default=None):
        """Return the :class:`weapons.instance.WeaponClass` for the weapon.

        :param str item: The weapon to retrieve the instance of.
        :param default: The value to return if the item is not found.
        :rtype: WeaponClass
        """
        name = self._format_name(item)
        return super().get(name, default)

    def _format_name(self, item):
        """Format the name to include the game's weapon prefix."""
        # Set the weapon to lower-case
        name = item.lower()

        # Is the item a member of the special names?
        if name in self.special_names:

            # Get the value of the special name
            name = self.special_names[name]

        # Is the item a member of the projectiles?
        if name in self.projectiles:

            # Get the value of the projectile name
            name = self.projectiles[name]

        # Does the weapon start with the prefix?
        if not name.startswith(self.prefix):

            # Add the prefix to the name
            name = self.prefix + name

        # Return the name
        return name

    @property
    def prefix(self):
        """Return the weapon prefix value for the server."""
        return self._prefix

    @property
    def ammoprop(self):
        """Return the ammoprop property for the server."""
        return self._ammoprop

    @property
    def myweapons(self):
        """Return the myweapons property for the server."""
        return self._myweapons

    @property
    def special_names(self):
        """Return the special_names for the server."""
        return self._special_names

    @property
    def projectiles(self):
        """Return the projectiles for the server."""
        return self._projectiles

    @property
    def tags(self):
        """Return the weapon tags for the server."""
        return self._tags

# Does the current game have an ini file?
if _gamepath.isfile():

    # Get the _WeaponManager instance
    weapon_manager = _WeaponManager()

# Is there no ini file for the current game?
else:

    # Store weapon_manager as a NoWeaponManager instance
    # to raise an error anytime the manager is utilized
    weapon_manager = NoWeaponManager()
