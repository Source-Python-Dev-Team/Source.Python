# ../entities/specials.py

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
from memory_c import alloc
from core import GAME_NAME
#   Entities
from entities.constants import DamageOffsets
from entities.helpers import index_from_inthandle
#   Filters
from filters.weapons import WeaponClassIter


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
# Set all to an empty list
__all__ = []


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
    '''Base class used to hold special functionality'''

    def damage(
            self, victim_index, damage=0, damage_type=0,
            weapon_index=None, hitgroup=0, **kwargs):
        '''Method used to hurt another entity with the given arguments'''

        # Import BaseEntity classes
        # Doing this in the global scope causes cross import errors
        from entities.entity import BaseEntity
        from players.entity import PlayerEntity

        # Is the game supported?
        if not 'take_damage' in self.functions:

            # Raise an error if not supported
            raise NotImplementedError(
                'damage is not implemented for {0}'.format(GAME_NAME))

        # Was no weapon index given?
        if weapon_index is None and hasattr(self, 'active_weapon'):

            # Get the player's active weapon
            weapon_index = index_from_inthandle(self.active_weapon)

        # Get the weapon's BaseEntity instance
        weapon = BaseEntity(weapon_index)

        # Get the victim's BaseEntity instance.
        victim = BaseEntity(victim_index)

        # Is the victim a player?
        if victim.classname == 'player':

            # Get the victim's PlayerEntity instance instead
            victim = PlayerEntity(victim_index)

            # Is hitgroup a valid attribute?
            if hasattr(victim, 'hitgroup'):

                # Set the victim's hitgroup
                victim.hitgroup = hitgroup

        # Get a memory address for CTakeDamageInfo
        take_damage_info = alloc(96, True)

        # Is the weapon a projectile?
        if weapon.classname in _projectile_weapons:

            # Set the hInflictor to the weapon's handle
            take_damage_info.set_int(
                weapon.inthandle, DamageOffsets.hInflictor['offset'])

        # Is the weapon not a projectile?
        else:

            # Set the hInflictor to the entity's handle
            take_damage_info.set_int(
                self.inthandle, DamageOffsets.hInflictor['offset'])

        # Set the hAttacker to the entity's handle
        take_damage_info.set_int(
            self.inthandle, DamageOffsets.hAttacker['offset'])

        # Set the hWeapon to the weapon's handle
        take_damage_info.set_int(
            weapon.inthandle, DamageOffsets.hWeapon['offset'])

        # Set the flDamage amount
        take_damage_info.set_float(damage, DamageOffsets.flDamage['offset'])

        # Set the bitsDamageType value
        take_damage_info.set_int(
            damage_type, DamageOffsets.bitsDamageType['offset'])

        # Loop through the given keywords
        for item in kwargs:

            # Is the keyword supported?
            if item in DamageOffsets:

                # Set the offset's value
                getattr(
                    take_damage_info, 'set_{0}'.format(
                    DamageOffsets[item]['type']))(
                    kwargs[item], DamageOffsets[item]['offset'])

        # Call the function with the victim's pointer and the CTakeDamageInfo
        victim.take_damage(take_damage_info)
