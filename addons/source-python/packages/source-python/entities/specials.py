# ../entities/specials.py

"""Provides special methods that do not fit anywhere else."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
#   Core
from core import GAME_NAME
#   Entities
from entities.constants import CTakeDamageInfo
from entities.helpers import index_from_inthandle
#   Filters
from filters.weapons import WeaponClassIter


# =============================================================================
# >> GLOBAL VARIABLES
# =============================================================================
# Get a list of projectiles for the game
_projectile_weapons = list(
    WeaponClassIter('grenade', return_types='classname'))


# =============================================================================
# >> CLASSES
# =============================================================================
class _EntitySpecials(object):

    """Base class used to hold special functionality."""

    def damage(
            self, victim_index, damage=0, damage_type=0,
            weapon_index=None, hitgroup=0, **kwargs):
        """Method used to hurt another entity with the given arguments."""
        # Import BaseEntity classes
        # Doing this in the global scope causes cross import errors
        from entities.entity import BaseEntity
        from players.entity import PlayerEntity

        # Is the game supported?
        if 'take_damage' not in self.functions or CTakeDamageInfo is None:

            # Raise an error if not supported
            raise NotImplementedError(
                'damage is not implemented for {0}'.format(GAME_NAME))

        # Was no weapon index given?
        if weapon_index is None and hasattr(self, 'active_weapon'):

            # Get the player's active weapon
            weapon_index = index_from_inthandle(self.active_weapon)

        # Is the weapon invalid?
        if weapon_index is None:

            # Get a _FakeWeapon instance
            weapon = _FakeWeapon()

        # Is the weapon valid?
        else:

            # Get the weapon's BaseEntity instance
            weapon = BaseEntity(weapon_index)

        # Get the victim's BaseEntity instance.
        victim = BaseEntity(victim_index)

        # Is the victim a player?
        if victim.classname == 'player':

            # Get the victim's PlayerEntity instance instead
            victim = PlayerEntity(victim_index)

            # Is hitgroup a valid attribute?
            if hasattr(victim, 'm_LastHitGroup'):

                # Set the victim's hitgroup
                victim.m_LastHitGroup = hitgroup

        # Get a memory address for CTakeDamageInfo
        take_damage_info = CTakeDamageInfo()

        # Is the weapon a projectile?
        if weapon.classname in _projectile_weapons:

            # Set the hInflictor to the weapon's handle
            take_damage_info.hInflictor = weapon.inthandle

        # Is the weapon not a projectile?
        else:

            # Set the hInflictor to the entity's handle
            take_damage_info.hInflictor = self.inthandle

        # Set the hAttacker to the entity's handle
        take_damage_info.hAttacker = self.inthandle

        # Set the hWeapon to the weapon's handle
        take_damage_info.hWeapon = weapon.inthandle

        # Set the flDamage amount
        take_damage_info.flDamage = damage

        # Set the bitsDamageType value
        take_damage_info.bitsDamageType = damage_type

        # Loop through the given keywords
        for item in kwargs:

            # Is the keyword supported?
            if hasattr(take_damage_info, item):

                # Set the offset's value
                getattr(take_damage_info, item, kwargs[item])

        # Call the function with the victim's pointer and the CTakeDamageInfo
        victim.take_damage(take_damage_info)


class _FakeWeapon(object):

    """Class used if no weapon is provided in the damage method."""

    classname = None
    inthandle = 0
