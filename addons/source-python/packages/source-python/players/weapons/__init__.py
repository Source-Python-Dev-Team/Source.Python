# ../players/weapons/__init__.py

"""Provides base player weapon functionality."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
#   Entities
from entities.entity import Entity
from entities.helpers import edict_from_index
from entities.helpers import index_from_inthandle
from entities.props import SendPropType
#   Engines
from engines.server import server_game_dll
#   Weapons
from weapons.default import NoWeaponManager
from weapons.manager import weapon_manager


# =============================================================================
# >> CLASSES
# =============================================================================
class _PlayerWeapons(object):
    """Extends Player to provide basic weapon functionality."""

    # =========================================================================
    # >> GET AMMO
    # =========================================================================
    def get_ammo(self, weapon):
        """Return the amount of ammo the player has for the given weapon."""
        # Get the proper classname for the weapon
        classname = weapon_manager[weapon].name

        # Return the amount of ammo the player has for the weapon
        return self._get_weapon_ammo(classname)

    def get_primary_ammo(self):
        """Return the amount of ammo the player has for their primary."""
        return self._get_weapon_ammo(is_filters='primary')

    def get_secondary_ammo(self):
        """Return the amount of ammo the player has for their secondary."""
        return self._get_weapon_ammo(is_filters='secondary')

    def _get_weapon_ammo(
            self, classname=None, is_filters=None, not_filters=None):
        """Return the amount of ammo for the given arguments.

        Returns for the first weapon found from the given arguments.
        """
        # Get the index for the weapon with the given arguments
        index = self.get_weapon_index(classname, is_filters, not_filters)

        # Was no index found for the given arguments?
        if index is None:

            # Return 0 as the amount
            return 0

        # Get the Entity instance for the index
        weapon = Entity(index)

        # Return the amount of ammo the player has for the weapon
        return self.get_property_int(
            weapon_manager.ammoprop + '%03d' % weapon.ammoprop)

    # =========================================================================
    # >> GET CLIP
    # =========================================================================
    def get_clip(self, weapon):
        """Return the amount of ammo in the clip for the given weapon."""
        # Get the proper classname for the weapon
        classname = weapon_manager[weapon].name

        # Return the amount of ammo in the weapon's clip
        return self._get_weapon_clip(classname)

    def get_primary_clip(self):
        """Return the amount of ammo in the player's primary weapon clip."""
        return self._get_weapon_clip(is_filters='primary')

    def get_secondary_clip(self):
        """Return the amount of ammo in the player's secondary weapon clip."""
        return self._get_weapon_clip(is_filters='secondary')

    def _get_weapon_clip(
            self, classname=None, is_filters=None, not_filters=None):
        """Return the clip amount for the given arguments.

        Returns for the first weapon found from the given arguments.
        """
        # Get the index for the weapon with the given arguments
        index = self.get_weapon_index(classname, is_filters, not_filters)

        # Was no index found for the given arguments?
        if index is None:

            # Return 0 as the amount
            return 0

        # Get the Entity instance for the index
        weapon = Entity(index)

        # Return the amount of ammo in the weapon's clip
        return weapon.clip

    # =========================================================================
    # >> SET AMMO
    # =========================================================================
    def set_ammo(self, weapon, value):
        """Set the player's ammo property for the given weapon."""
        # Get the proper classname for the weapon
        classname = weapon_manager[weapon].name

        # Set the player's ammo value
        self._set_weapon_ammo(value, classname)

    def set_primary_ammo(self, value):
        """Set the player's primary weapon's ammo value."""
        self._set_weapon_ammo(value, is_filters='primary')

    def set_secondary_ammo(self, value):
        """Set the player's secondary weapon's ammo value."""
        self._set_weapon_ammo(value, is_filters='secondary')

    def _set_weapon_ammo(
            self, value, classname=None, is_filters=None, not_filters=None):
        """Set the player's ammo value for the given arguments.

        Sets for the first weapon found from the given arguments.
        """
        # Get the index for the given arguments
        index = self.get_weapon_index(classname, is_filters, not_filters)

        # Was no index found?
        if index is None:

            # Raise an error
            raise LookupError(
                'No index found for given arguments '
                '"{0}, {1}, {2}" for player "{3}"'.format(
                    classname, is_filters, not_filters, self.userid))

        # Get the entity's Entity instance
        weapon = Entity(index)

        # Set the player's ammo value
        self.set_property_int(
            weapon_manager.ammoprop + '%03d' % weapon.ammoprop, value)

    # =========================================================================
    # >> SET CLIP
    # =========================================================================
    def set_clip(self, weapon, value):
        """Set the player's clip value for the given weapon."""
        # Get the proper classname for the weapon
        classname = weapon_manager[weapon].name

        # Set the clip value for the given weapon
        self._set_weapon_clip(value, classname)

    def set_primary_clip(self, value):
        """Set the player's primary weapon's clip value."""
        self._set_weapon_clip(value, is_filters='primary')

    def set_secondary_clip(self, value):
        """Set the player's secondary weapon's clip value."""
        self._set_weapon_clip(value, is_filters='secondary')

    def _set_weapon_clip(
            self, value, classname=None, is_filters=None, not_filters=None):
        """Set the player's clip value for the given arguments.

        Sets for the first weapon found from the given arguments.
        """
        # Get the index for the given arguments
        index = self.get_weapon_index(classname, is_filters, not_filters)

        # Was no index found?
        if index is None:

            # Raise an error
            raise LookupError(
                'No index found for given arguments '
                '"{0}, {1}, {2}" for player "{3}"'.format(
                    classname, is_filters, not_filters, self.userid))

        # Get the entity's Entity instance
        weapon = Entity(index)

        # Set the weapon's clip value
        weapon.clip = value

    # =========================================================================
    # >> ADD AMMO
    # =========================================================================
    def add_ammo(self, weapon, value):
        """Add ammo to the given weapon."""
        # Get the proper classname for the weapon
        classname = weapon_manager[weapon].name

        # Add ammo for the given weapon
        self._add_weapon_ammo(value, classname)

    def add_primary_ammo(self, value):
        """Add ammo for the player's primary weapon."""
        self._add_weapon_ammo(value, is_filters='primary')

    def add_secondary_ammo(self, value):
        """Add ammo for the player's secondary weapon."""
        self._add_weapon_ammo(value, is_filters='secondary')

    def _add_weapon_ammo(
            self, value, classname=None, is_filters=None, not_filters=None):
        """Add ammo to the player's current value for the given arguments.

        Adds for the first weapon found from the given arguments.
        """
        # Get the index for the given arguments
        index = self.get_weapon_index(classname, is_filters, not_filters)

        # Was no index found?
        if index is None:

            # Raise an error
            raise LookupError(
                'No index found for given arguments '
                '"{0}, {1}, {2}" for player "{3}"'.format(
                    classname, is_filters, not_filters, self.userid))

        # Get the entity's Entity instance
        weapon = Entity(index)

        # Get the current ammo value
        current = self.get_property_int(
            weapon_manager.ammoprop + '%03d' % weapon.ammoprop)

        # Add ammo to the current value
        self.set_property_int(
            weapon_manager.ammoprop + '%03d' % weapon.ammoprop,
            current + value)

    # =========================================================================
    # >> ADD CLIP
    # =========================================================================
    def add_clip(self, weapon, value):
        """Add ammo to the given weapon's clip."""
        # Get the proper classname for the weapon
        classname = weapon_manager[weapon].name

        # Add ammo to the given weapon's clip
        self._add_weapon_clip(value, classname)

    def add_primary_clip(self, value):
        """Add ammo for the player's primary weapon's clip."""
        self._add_weapon_clip(value, is_filters='primary')

    def add_secondary_clip(self, value):
        """Add ammo for the player's secondary weapon's clip."""
        self._add_weapon_clip(value, is_filters='secondary')

    def _add_weapon_clip(
            self, value, classname=None, is_filters=None, not_filters=None):
        """Add ammo to the weapon's current value for the given arguments.

        Adds for the first weapon found from the given arguments.
        """
        # Get the index for the given arguments
        index = self.get_weapon_index(classname, is_filters, not_filters)

        # Was no index found?
        if index is None:

            # Raise an error
            raise LookupError(
                'No index found for given arguments '
                '"{0}, {1}, {2}" for player "{3}"'.format(
                    classname, is_filters, not_filters, self.userid))

        # Get the entity's Entity instance
        weapon = Entity(index)

        # Add ammo to the weapon's clip
        weapon.clip += value

    # =========================================================================
    # >> WEAPON INDEXES
    # =========================================================================
    def get_primary(self):
        """Return the player's primary weapon's index."""
        return self.get_weapon_index(is_filters='primary')

    def get_secondary(self):
        """Return the player's secondary weapon's index."""
        return self.get_weapon_index(is_filters='secondary')

    def get_weapon_index(
            self, classname=None, is_filters=None, not_filters=None):
        """Return the first instance of the given weapon classname/type."""
        # Loop through all weapon indexes for
        # the player with the given arguments
        for index in self.weapon_indexes(classname, is_filters, not_filters):

            # Return the first index found
            return index

        # If no index is found, return None
        return None

    def get_weapon_index_list(
            self, classname=None, is_filters=None, not_filters=None):
        """Return a list of weapon indexes for the player."""
        return list(self.weapon_indexes(classname, is_filters, not_filters))

    def weapon_indexes(
            self, classname=None, is_filters=None, not_filters=None):
        """Iterate over all currently held weapons by thier index."""
        # Is the weapon array supported for the current game?
        if _weapon_prop_length is None:
            return

        # Loop through the length of the weapon array
        for offset in range(_weapon_prop_length):

            # Get the player's current weapon at this offset
            handle = self.get_property_int(
                weapon_manager.myweapons + '%03i' % offset)

            try:
                index = index_from_inthandle(handle)
            except (ValueError, OverflowError):
                continue

            # Get the weapon's classname
            weapon_class = edict_from_index(index).get_class_name()

            # Was a classname given and the current
            # weapon is not of that classname?
            if classname is not None and weapon_class != classname:

                # Do not yield this index
                continue

            # Import WeaponClassIter to use its functionality
            from filters.weapons import WeaponClassIter

            # Was a weapon type given and the
            # current weapon is not of that type?
            if not (is_filters is None and not_filters is None):
                if weapon_class not in map(lambda value: value.name,
                        WeaponClassIter(is_filters, not_filters)):

                    # Do not yield this index
                    continue

            # Yield the index
            yield index

    # =========================================================================
    # >> COLOR METHODS
    # =========================================================================
    def get_weapon_color(self):
        """Return a tuple value for the player's active weapon's color."""
        try:
            index = index_from_inthandle(self.active_weapon)
        except (ValueError, OverflowError):
            raise ValueError(
                'No active weapon found for player "{0}"'.format(self.userid))

        return Entity(index).color

    def set_weapon_color(self, color):
        """Set the player's active weapon's color."""
        try:
            index = index_from_inthandle(self.active_weapon)
        except (ValueError, OverflowError):
            raise ValueError(
                'No active weapon found for player "{0}"'.format(self.userid))

        Entity(index).color = color


# =============================================================================
# >> HELPER FUNCTIONS
# =============================================================================
def _find_weapon_prop_length(table):
    """Loop through a prop table to find the myweapons property length."""
    # Loop through the props in the table
    for offset in range(len(table)):

        # Get the prop
        item = table[offset]

        # Is this the m_hMyWeapons prop?
        if item.name == weapon_manager.myweapons[:~0]:

            # If so, return the length of the prop table
            return len(item.data_table)

        # Is the current prop a table?
        if item.type == SendPropType.DATATABLE:

            # Loop through the table
            _find_weapon_prop_length(item.data_table)

# Default the weapon prop length to None
_weapon_prop_length = None

# Is the game supported?
if not isinstance(weapon_manager, NoWeaponManager):

    # Get the first ServerClass object
    _current_class = server_game_dll.get_all_server_classes()

    # Use "while" to loop through all ServerClass objects
    while _current_class:

        # Loop through the ServerClass' props
        _weapon_prop_length = _find_weapon_prop_length(_current_class.table)

        # Was m_hMyWeapons found?
        if _weapon_prop_length is not None:

            # No need to continue looping
            break

        # Move to the next ServerClass
        _current_class = _current_class.next
