# ../entities/specials.py

"""Provides special methods that do not fit anywhere else."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Python Imports
#   Contextlib
from contextlib import suppress

# Source.Python Imports
#   Core
from core import GAME_NAME
#   Entities
from entities import TakeDamageInfo
from entities.constants import DamageTypes
from entities.helpers import index_from_inthandle
#   Filters
from filters.weapons import WeaponClassIter
#   Players
from players.constants import HitGroup


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

    def hurt(
            self, damage, damage_type=DamageTypes.GENERIC, attacker_index=None,
            weapon_index=None, hitgroup=HitGroup.GENERIC, **kwargs):
        """Method used to hurt the entity with the given arguments."""
        # Import BaseEntity classes
        # Doing this in the global scope causes cross import errors
        from entities.entity import BaseEntity
        from weapons.entity import WeaponEntity

        # Is the game supported?
        if not hasattr(self, 'take_damage'):

            # Raise an error if not supported
            raise NotImplementedError(
                '"take_damage" is not implemented for {0}'.format(GAME_NAME))

        # Store values for later use
        attacker = None
        weapon = None

        # Was an attacker given?
        if attacker_index is not None:

            # Try to get the BaseEntity instance of the attacker
            with suppress(ValueError):
                attacker = BaseEntity(attacker_index)

        # Was a weapon given?
        if weapon_index is not None:

            # Try to get the WeaponEntity instance of the weapon
            with suppress(ValueError):
                weapon = WeaponEntity(weapon_index)

        # Is there a weapon but no attacker?
        if attacker is None and weapon is not None:

            # Try to get the attacker based off of the weapon's owner
            with suppress(ValueError):
                attacker_index = index_from_inthandle(weapon.current_owner)
                if attacker_index is not None:
                    attacker = BaseEntity(attacker_index)

        # Is there an attacker but no weapon?
        if attacker is not None and weapon is None:

            # Does the attacker have a weapon attribute?
            if hasattr(attacker, 'active_weapon'):

                # Get the attacker's current weapon index
                weapon_index = index_from_inthandle(
                    attacker.active_weapon, False)

                # Get the weapon's WeaponEntity instance if it is valid
                if weapon_index is not None:
                    weapon = WeaponEntity(weapon_index)

        # Is hitgroup a valid attribute?
        if hasattr(self, 'hitgroup'):

            # Set the hitgroup
            self.hitgroup = hitgroup

        # Get a TakeDamageInfo instance
        take_damage_info = TakeDamageInfo()

        # Is there a valid weapon?
        if weapon is not None:

            # Is the weapon a projectile?
            if weapon.classname in _projectile_weapons:

                # Set the inflictor to the weapon's index
                take_damage_info.inflictor = weapon.index

            # Is the weapon not a projectile and the attacker is valid?
            elif attacker_index is not None:

                # Set the inflictor to the attacker's index
                take_damage_info.inflictor = attacker_index

            # Set the weapon to the weapon's index
            take_damage_info.weapon = weapon.index

        # Is the attacker valid?
        if attacker_index is not None:

            # Set the attacker to the attacker's index
            take_damage_info.attacker = attacker_index

        # Set the damage amount
        take_damage_info.damage = damage

        # Set the damage type value
        take_damage_info.type = damage_type

        # Loop through the given keywords
        for item in kwargs:

            # Set the offset's value
            setattr(take_damage_info, item, kwargs[item])

        # Call the function with the victim's pointer and the CTakeDamageInfo
        self.take_damage(take_damage_info)
