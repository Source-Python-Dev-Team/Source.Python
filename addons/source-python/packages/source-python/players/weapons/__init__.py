# ../players/weapons/__init__.py

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
from entity_c import Edict
from core import GAME_NAME
#   Entities
from entities.entity import BaseEntity
from entities.helpers import index_from_inthandle
#   Weapons
from weapons.manager import WeaponManager


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
# Set all to an empty list
__all__ = []


# =============================================================================
# >> GLOBAL VARIALBES
# =============================================================================
# Use try/except to import the _GameWeapons class
try:

    # Get the game's file's instance
    _game_instance = __import__(
        'players.games.{0}'.format(GAME_NAME), fromlist=[''])

# Was an ImportError encountered?
except ImportError:

    # Set the game's _GameWeapons class to the basic "object" type
    _GameWeapons = object

# Was no ImportError encountered?
else:

    # Get the game's _GameWeapons class
    _GameWeapons = _game_instance._GameWeapons

    # If the rest was successful, import WeaponClassIter
    from filters.weapons import WeaponClassIter


# =============================================================================
# >> CLASSES
# =============================================================================
class _PlayerWeapons(_GameWeapons):
    '''Base Weapon class inherited by PlayerEntity to
        perform basic weapon functionality for the player'''

    # =========================================================================
    # >> GET AMMO
    # =========================================================================
    def get_ammo(self, weapon):
        '''Returns the amount of ammo the player has for the given weapon'''

        # Get the proper classname for the weapon
        classname = WeaponManager[weapon].name

        # Return the amount of ammo the player has for the weapon
        return self._get_weapon_ammo(classname)

    def get_primary_ammo(self):
        '''
            Returns the amount of ammo the player has for their primary weapon
        '''
        return self._get_weapon_ammo(is_filters='primary')

    def get_secondary_ammo(self):
        '''Returns the amount of ammo the
            player has for their secondary weapon'''
        return self._get_weapon_ammo(is_filters='secondary')

    def _get_weapon_ammo(self, classname=None, is_filters=[], not_filters=[]):
        '''Returns the amount of ammo for the
            first weapon for the given arguments'''

        # Get the index for the weapon with the given arguments
        index = self.get_weapon_index(classname, is_filters, not_filters)

        # Was no index found for the given arguments?
        if index is None:

            # Return 0 as the amount
            return 0

        # Get the BaseEntity instance for the index
        weapon = BaseEntity(index, 'weapon')

        # Return the amount of ammo the player has for the weapon
        return self.GetPropInt(
            WeaponManager.ammoprop + '%03d' % weapon.ammoprop)

    # =========================================================================
    # >> GET CLIP
    # =========================================================================
    def get_clip(self, weapon):
        '''Returns the amount of ammo in the
            weapon's clip for the given weapon'''

        # Get the proper classname for the weapon
        classname = WeaponManager[weapon].name

        # Return the amount of ammo in the weapon's clip
        return self._get_weapon_clip(classname)

    def get_primary_clip(self):
        '''Returns the amount of ammo in the player's primary weapon's clip'''
        return self._get_weapon_clip(is_filters='primary')

    def get_secondary_clip(self):
        '''
            Returns the amount of ammo in the player's secondary weapon's clip
        '''
        return self._get_weapon_clip(is_filters='secondary')

    def _get_weapon_clip(self, classname=None, is_filters=[], not_filters=[]):
        '''Returns the amount of ammo in the clip for
            the first weapon for the given arguments'''

        # Get the index for the weapon with the given arguments
        index = self.get_weapon_index(classname, is_filters, not_filters)

        # Was no index found for the given arguments?
        if index is None:

            # Return 0 as the amount
            return 0

        # Get the BaseEntity instance for the index
        weapon = BaseEntity(index, 'weapon')

        # Return the amount of ammo in the weapon's clip
        return weapon.clip

    # =========================================================================
    # >> SET AMMO
    # =========================================================================
    def set_ammo(self, weapon, value):
        '''Sets the player's ammo property for the given weapon'''

        # Get the proper classname for the weapon
        classname = WeaponManager[weapon].name

        # Set the player's ammo value
        self._set_weapon_ammo(value, classname)

    def set_primary_ammo(self, value):
        '''Sets the player's primary weapon's ammo value'''
        self._set_weapon_ammo(value, is_filters='primary')

    def set_secondary_ammo(self, value):
        '''Sets the player's secondary weapon's ammo value'''
        self._set_weapon_ammo(value, is_filters='secondary')

    def _set_weapon_ammo(
            self, value, classname=None, is_filters=[], not_filters=[]):
        '''Sets the player's ammo value for first
            weapon found with the given arguments'''

        # Get the index for the given arguments
        index = self.get_weapon_index(classname, is_filters, not_filters)

        # Was no index found?
        if index is None:

            # Raise an error
            raise LookupError(
                'No index found for given arguments '
                '"{0}, {1}, {2}" for player "{3}"'.format(
                    classname, is_filters, not_filters, self.userid))

        # Get the entity's BaseEntity instance
        weapon = BaseEntity(index, 'weapon')

        # Set the player's ammo value
        self.SetPropInt(
            WeaponManager.ammoprop + '%03d' % weapon.ammoprop, value)

    # =========================================================================
    # >> SET CLIP
    # =========================================================================
    def set_clip(self, weapon, value):
        '''Sets the player's clip value for the given weapon'''

        # Get the proper classname for the weapon
        classname = WeaponManager[weapon].name

        # Set the clip value for the given weapon
        self._set_weapon_clip(value, classname)

    def set_primary_clip(self, value):
        '''Sets the player's primary weapon's clip value'''
        self._set_weapon_clip(value, is_filters='primary')

    def set_secondary_clip(self, value):
        '''Sets the player's secondary weapon's clip value'''
        self._set_weapon_clip(value, is_filters='secondary')

    def _set_weapon_clip(
            self, value, classname=None, is_filters=[], not_filters=[]):
        '''Sets the player's clip value for first
            weapon found with the given arguments'''

        # Get the index for the given arguments
        index = self.get_weapon_index(classname, is_filters, not_filters)

        # Was no index found?
        if index is None:

            # Raise an error
            raise LookupError(
                'No index found for given arguments '
                '"{0}, {1}, {2}" for player "{3}"'.format(
                    classname, is_filters, not_filters, self.userid))

        # Get the entity's BaseEntity instance
        weapon = BaseEntity(index, 'weapon')

        # Set the weapon's clip value
        weapon.clip = value

    # =========================================================================
    # >> ADD AMMO
    # =========================================================================
    def add_ammo(self, weapon, value):
        '''Adds ammo to the given weapon'''

        # Get the proper classname for the weapon
        classname = WeaponManager[weapon].name

        # Add ammo for the given weapon
        self._add_weapon_ammo(value, classname)

    def add_primary_ammo(self, value):
        '''Adds ammo for the player's primary weapon'''
        self._add_weapon_ammo(value, is_filters='primary')

    def add_secondary_ammo(self, value):
        '''Adds ammo for the player's secondary weapon'''
        self._add_weapon_ammo(value, is_filters='secondary')

    def _add_weapon_ammo(
            self, value, classname=None, is_filters=[], not_filters=[]):
        '''Adds ammo to the player's current value for the
            first weapon found with the given arguments'''

        # Get the index for the given arguments
        index = self.get_weapon_index(classname, is_filters, not_filters)

        # Was no index found?
        if index is None:

            # Raise an error
            raise LookupError(
                'No index found for given arguments '
                '"{0}, {1}, {2}" for player "{3}"'.format(
                    classname, is_filters, not_filters, self.userid))

        # Get the entity's BaseEntity instance
        weapon = BaseEntity(index, 'weapon')

        # Get the current ammo value
        current = self.GetPropInt(
            WeaponManager.ammoprop + '%03d' % weapon.ammoprop)

        # Add ammo to the current value
        self.SetPropInt(
            WeaponManager.ammoprop + '%03d' % weapon.ammoprop, current + value)

    # =========================================================================
    # >> ADD CLIP
    # =========================================================================
    def add_clip(self, weapon, value):
        '''Adds ammo to the given weapon's clip'''

        # Get the proper classname for the weapon
        classname = WeaponManager[weapon].name

        # Add ammo to the given weapon's clip
        self._add_weapon_clip(value, classname)

    def add_primary_clip(self, value):
        '''Adds ammo for the player's primary weapon's clip'''
        self._add_weapon_clip(value, is_filters='primary')

    def add_secondary_clip(self, value):
        '''Adds ammo for the player's secondary weapon's clip'''
        self._add_weapon_clip(value, is_filters='secondary')

    def _add_weapon_clip(
            self, value, classname=None, is_filters=[], not_filters=[]):
        '''Adds ammo to the weapon's current value for the
            first weapon found with the given arguments'''

        # Get the index for the given arguments
        index = self.get_weapon_index(classname, is_filters, not_filters)

        # Was no index found?
        if index is None:

            # Raise an error
            raise LookupError(
                'No index found for given arguments '
                '"{0}, {1}, {2}" for player "{3}"'.format(
                    classname, is_filters, not_filters, self.userid))

        # Get the entity's BaseEntity instance
        weapon = BaseEntity(index, 'weapon')

        # Add ammo to the weapon's clip
        weapon.clip += value

    # =========================================================================
    # >> WEAPON INDEXES
    # =========================================================================
    def get_primary(self):
        '''Returns the player's primary weapon's index'''
        return self.get_weapon_index(is_filters='primary')

    def get_secondary(self):
        '''Returns the player's secondary weapon's index'''
        return self.get_weapon_index(is_filters='secondary')

    def get_weapon_index(self, classname=None, is_filters=[], not_filters=[]):
        '''Returns the first instance of the given weapon classname/type'''

        # Loop through all weapon indexes for
        # the player with the given arguments
        for index in self.weapon_indexes(classname, is_filters, not_filters):

            # Return the first index found
            return index

        # If no index is found, return None
        return None

    def get_weapon_index_list(
            self, classname=None, is_filters=[], not_filters=[]):
        '''Returns a list of weapon indexes for the player'''
        return list(self.weapon_indexes(classname, is_filters, not_filters))

    def weapon_indexes(self, classname=None, is_filters=[], not_filters=[]):
        '''
            Iterates over all currently held weapons, and yields their indexes
        '''

        # Loop through the length of m_hMyWeapons
        for offset in range(64):

            # Get the player's current weapon at this offset
            handle = self.GetPropInt(WeaponManager.myweapons + '%03i' % offset)

            # Is this an invalid handle?
            if handle == -1:

                # Move onto the next offset
                continue

            # Get the weapon's index
            index = index_from_inthandle(handle)

            # Is this a valid index?
            if index is None:

                # Move onto the next offset
                continue

            # Get the weapon's edict
            edict = Edict(index)

            # Get the weapon's classname
            weapon_class = edict.get_class_name()

            # Was a classname given and the current
            # weapon is not of that classname?
            if not classname is None and weapon_class != classname:

                # Do not yield this index
                continue

            # Was a weapon type given and the
            # current weapon is not of that type?
            if ((is_filters or not_filters) and not weapon_class in list(
                    WeaponClassIter(is_filters, not_filters, 'classname'))):

                # Do not yield this index
                continue

            # Yield the index
            yield index

    # =========================================================================
    # >> COLOR METHODS
    # =========================================================================
    def get_weapon_color(self):
        '''Returns a tuple value for the player's active weapon's color'''

        # Get the handle of the player's active weapon
        handle = self.active_weapon

        # Get the weapon's BaseHandle instance
        index = index_from_inthandle(handle)

        # Was no index found?
        if index is None:

            # Raise an error
            raise ValueError(
                'No active weapon found for player "{0}"'.format(self.userid))

        # Return the entity's color
        return BaseEntity(index).color

    def set_weapon_color(self, red, green, blue, alpha=None):
        '''Sets the player's active weapon's color'''

        # Get the handle of the player's active weapon
        handle = self.active_weapon

        # Get the weapon's BaseHandle instance
        index = index_from_inthandle(handle)

        # Was no index found?
        if index is None:

            # Raise an error
            raise ValueError(
                'No active weapon found for player "{0}"'.format(self.userid))

        # Set the entity's color
        BaseEntity(index).color = (red, green, blue, alpha)
