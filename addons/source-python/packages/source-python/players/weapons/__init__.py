# ../players/weapons/__init__.py

"""Provides base player weapon functionality."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
#   Entities
from entities.constants import INVALID_ENTITY_INDEX
from entities.helpers import edict_from_index
from entities.helpers import index_from_inthandle
from entities.props import SendPropType
#   Engines
from engines.server import server_game_dll
#   Weapons
from weapons.default import NoWeaponManager
from weapons.entity import Weapon
from weapons.manager import weapon_manager


# =============================================================================
# >> CLASSES
# =============================================================================
class _PlayerWeapons(object):
    """Extends Player to provide basic weapon functionality."""

    @property
    def primary(self):
        """Return the player's primary weapon.

        :rtype: Weapon
        """
        return self.get_weapon(is_filters='primary')

    @property
    def secondary(self):
        """Return the player's secondary weapon.

        :rtype: Weapon
        """
        return self.get_weapon(is_filters='secondary')

    @property
    def active_weapon(self):
        """Return the player's active weapon.

        :rtype: Weapon
        """
        # Get the player's active weapon's index
        index = index_from_inthandle(self.active_weapon_handle)

        # Does the player have an active weapon?
        if index is not None:

            # Return a Weapon instance for the player's active weapon
            return Weapon(index)

        # If no active weapon, return None
        return None

    def get_weapon(self, classname=None, is_filters=None, not_filters=None):
        """Return the first found weapon for the given arguments.

        :rtype: Weapon
        """
        # Loop through all weapons for the given arguments
        for weapon in self.weapons(classname, is_filters, not_filters):

            # Return the first found weapon
            return weapon

        # If no weapon is found, return None
        return None

    def weapons(self, classname=None, is_filters=None, not_filters=None):
        """Iterate over the player's weapons for the given arguments.

        :return: A generator of :class:`weapons.entity.Weapon` objects
        :rtype: generator
        """
        # Loop through all the players weapons for the given arguments
        for index in self.weapon_indexes(classname, is_filters, not_filters):

            # Yield the current weapon
            yield Weapon(index)

    def weapon_indexes(
            self, classname=None, is_filters=None, not_filters=None):
        """Iterate over the player's weapon indexes for the given arguments.

        :return: A generator of indexes
        :rtype: generator
        """
        # Is the weapon array supported for the current game?
        if _weapon_prop_length is None:
            return

        # Loop through the length of the weapon array
        for offset in range(_weapon_prop_length):

            # Get the player's current weapon at this offset
            handle = self.get_property_int(
                weapon_manager.myweapons + '%03i' % offset)

            # Get the weapon's index
            index = index_from_inthandle(handle, raise_exception=False)

            # Is this a valid index?
            if index == INVALID_ENTITY_INDEX:

                # Move onto the next offset
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
                if weapon_class not in [
                        weapon.name for weapon in WeaponClassIter(
                            is_filters, not_filters)]:

                    # Do not yield this index
                    continue

            # Yield the index
            yield index


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
