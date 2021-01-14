# ../weapons/restrictions.py

"""Provides player weapon restriction functionality."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Python Imports
#   Collections
from collections import defaultdict

# Source.Python Imports
#   Core
from core import AutoUnload
from core import GAME_NAME
from core import SOURCE_ENGINE
#   Entities
from entities.constants import INVALID_ENTITY_INDEX
from entities.helpers import edict_from_pointer
from entities.hooks import EntityCondition
from entities.hooks import EntityPreHook
#   Filters
from filters.players import PlayerIter
#   Listeners
from listeners import OnLevelInit
#   Memory
from memory import make_object
#   Players
from players.entity import Player
from players.teams import teams_by_name
from players.teams import teams_by_number
#   Weapons
from weapons.entity import Weapon
from weapons.manager import weapon_manager


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('_WeaponRestrictionManager',
           'WeaponRestrictionHandler',
           'weapon_restriction_handler',
           'weapon_restriction_manager',
           )


# =============================================================================
# >> CLASSES
# =============================================================================
class _WeaponRestrictionManager(set):
    """Class used to store weapon restriction handlers."""

    def on_player_bumping_weapon(self, player, weapon):
        """Return whether the player is restricted from bumping the weapon.

        :param Player player: The player that is bumping the weapon.
        :param str weapon: The name of the weapon the player is bumping.
        :return: Return whether to block bumping the weapon.
            None is used if the bumping should be allowed.
        :rtype: bool/None
        """
        # Get the weapon's basename
        weapon_name = weapon_manager[weapon].basename

        # Loop through all of the current handlers
        for handler in self:

            # If the current handler wishes to restrict, return such
            value = handler.on_player_bumping_weapon(player, weapon_name)
            if not value and value is not None:
                return True

        # If no handlers restrict the weapon, explicitly return None
        return None

    def on_player_purchasing_weapon(self, player, weapon):
        """Return whether the player is restricted from buying the weapon.

        :param Player player: The player that is purchasing the weapon.
        :param str weapon: The name of the weapon the player is purchasing.
        :return: Return whether to block purchasing the weapon.
            None is used if the purchasing should be allowed.
        :rtype: bool/None
        """
        # This conditional prevents an AttributeError when purchasing armor/nvg
        if weapon not in weapon_manager:
            return None

        # Get the weapon's basename
        weapon_name = weapon_manager[weapon].basename

        # Loop through all of the current handlers
        for handler in self:

            # If the current handler wishes to restrict, return such
            value = handler.on_player_purchasing_weapon(player, weapon_name)
            if not value and value is not None:
                return True

        # If no handlers restrict the weapon, explicitly return None
        return None

    def is_player_restricted(self, player, weapon):
        """Return whether the player is restricted from the weapon.

        :param Player player: The player to check for restriction.
        :param str weapon: The name of the weapon to check for restriction.
        :rtype: bool
        """
        # Get the weapon's basename
        weapon_name = weapon_manager[weapon].basename

        # Loop through all of the current handlers
        for handler in self:

            # If the current handler wishes to restrict, return such
            if handler.is_player_restricted(player, weapon_name):
                return True

        # If no handlers restrict the weapon, explicitly return None
        return None

    def is_team_restricted(self, team, weapon):
        """Return whether the team is restricted from the weapon.

        :param int team: The team to check for restriction.
            Value can also be a str alias for the team (ie t/ct).
        :param str weapon: The name of the weapon to check for restriction.
        :rtype: bool
        """
        # Get the weapon's basename
        weapon_name = weapon_manager[weapon].basename

        # Loop through all of the current handlers
        for handler in self:

            # If the current handler wishes to restrict, return such
            if handler.is_team_restricted(team, weapon_name):
                return True

        # If no handlers restrict the weapon, explicitly return None
        return None

    def add_handler(self, handler):
        """Add the handler to the set.

        :param WeaponRestrictionHandler handler: The handler
            to add to the dictionary.
        """
        self.add(handler)

    def remove_handler(self, handler):
        """Remove the handler from the set.

        :param WeaponRestrictionHandler handler: The handler
            to remove from the dictionary.
        """
        self.discard(handler)

    def clear(self):
        """Clear all handlers of any restrictions."""
        for handler in self:
            handler.clear()

# Get the _WeaponRestrictionManager instance
weapon_restriction_manager = _WeaponRestrictionManager()


class _TeamRestrictions(dict):
    """Class used to store team weapon restrictions."""

    def __init__(self):
        """Store all teams and their aliases for the current game."""
        super().__init__()

        # Store all of the aliases
        self.aliases = {**teams_by_name}
        self.update({x: set() for x in self.aliases.values()})

    def __getitem__(self, item):
        """Return the proper set object for the given team."""
        # Was a known team number given?
        if item in self:
            return super().__getitem__(item)

        # Was a known team alias given?
        if item in self.aliases:
            return super().__getitem__(self.aliases[item])

        # Invalid item was passed
        raise KeyError(
            '"{0}" is neither a team number nor an alias.'.format(item))

    def clear(self):
        for x in self.values():
            x.clear()


class WeaponRestrictionHandler(AutoUnload):
    """Class used to handle player and team restrictions."""

    def __init__(self):
        """Initialize the instance and add it to the manager."""
        super().__init__()
        self.team_restrictions = _TeamRestrictions()
        self.player_restrictions = defaultdict(set)
        weapon_restriction_manager.add_handler(self)

    def clear(self):
        """Remove all team and player restrictions."""
        self.player_restrictions.clear()
        self.team_restrictions.clear()

    def add_player_restrictions(self, player, *weapons):
        """Add the weapons to the player's restriction set.

        :param Player player: The player to add the restrictions to.
        :param str weapons: A weapon or any number of weapons to add
            as restricted for the player.
        """
        # Get the player's current restrictions
        current_restrictions = self.player_restrictions[player.userid].copy()

        # Add the weapons to the player's restrictions
        self.player_restrictions[player.userid].update([
            weapon_manager[weapon].basename for weapon in weapons])

        # Get all weapons added to the player's restriction set
        new_restrictions = self.player_restrictions[
            player.userid].difference(current_restrictions)

        # Notify of each new weapon restriction
        for weapon in new_restrictions:
            self.on_player_restriction_added(player, weapon)

    def remove_player_restrictions(self, player, *weapons):
        """Remove the weapons from the player's restriction set.

        :param Player player: The player to remove the restrictions from.
        :param str weapons: A weapon or any number of weapons to remove
            as restricted for the player.
        """
        self.player_restrictions[player.userid].difference_update([
            weapon_manager[weapon].basename for weapon in weapons])

    def add_team_restrictions(self, team, *weapons):
        """Add the weapons to the team's restriction set.

        :param int team: The team to add the restrictions to.
            Value can also be a team alias (ie t/ct).
        :param str weapons: A weapon or any number of weapons to add
            as restricted for the team.
        """
        # Get the number of the given team
        if isinstance(team, str):
            team = teams_by_name[team]

        # Get the team's current restrictions
        current_restrictions = set(self.team_restrictions[team])

        # Add the weapons to the team's restrictions
        self.team_restrictions[team].update([
            weapon_manager[weapon].basename for weapon in weapons])

        # Get all weapons added to the player's restriction set
        new_restrictions = self.team_restrictions[
            team].difference(current_restrictions)

        # Notify of each new weapon restriction
        for weapon in new_restrictions:
            self.on_team_restriction_added(team, weapon)

    def remove_team_restrictions(self, team, *weapons):
        """Remove the weapons from the team's restriction set.

        :param int team: The team to remove the restrictions from.
            Value can also be a team alias (ie t/ct).
        :param str weapons: A weapon or any number of weapons to remove
            as restricted for the team.
        """
        self.team_restrictions[team].difference_update([
            weapon_manager[weapon].basename for weapon in weapons])

    def on_player_bumping_weapon(self, player, weapon):
        """Return whether the player can bump the weapon.

        :param Player player: The player that is bumping the weapon.
        :param str weapon: The name of the weapon the player is bumping.
        :rtype: bool
        """
        return not self.is_player_restricted(player, weapon)

    def on_player_purchasing_weapon(self, player, weapon):
        """Return whether the player can purchase the weapon.

        :param Player player: The player that is purchasing the weapon.
        :param str weapon: The name of the weapon the player is purchasing.
        :rtype: bool
        """
        return not self.is_player_restricted(player, weapon)

    def is_player_restricted(self, player, weapon):
        """Return whether the player is restricted from the weapon.

        :param Player player: The player to check against restriction.
        :param str weapon: The name of the weapon to check against restriction.
        :rtype: bool
        """
        # Get the weapon's proper basename
        weapon_name = weapon_manager[weapon].basename

        # Is the player restricted from the weapon?
        if weapon_name in self.player_restrictions[player.userid]:
            return True

        # Return whether the player's team is restricted from the weapon
        return self.is_team_restricted(player.team, weapon_name)

    def is_team_restricted(self, team, weapon):
        """Return whether the team is restricted from the weapon.

        :param int team: The team to check for restriction.
            Value can also be a str alias for the team (ie t/ct).
        :param str weapon: The name of the weapon to check for restriction.
        :rtype: bool
        """
        return weapon_manager[weapon].basename in self.team_restrictions[team]

    def on_player_restriction_added(self, player, weapon):
        """Notify the handler if the player is carrying the weapon.

        :param Player player: The player that just had a restriction added.
        :param str weapon: The weapon that was just restricted.
        """
        # Does the player own the weapon type?
        weapon_index = self._get_player_weapon_index(player, weapon)
        if weapon_index is INVALID_ENTITY_INDEX:
            return

        # Notify the player is carrying the weapon
        self.on_player_carrying_restricted_weapon(player, Weapon(weapon_index))

    def on_team_restriction_added(self, team, weapon):
        """Notify the handler of each player on the restricted team.

        :param int team: The team that just had a restriction added.
        :param str weapon: The weapon that was just restricted.
        """
        for player in PlayerIter(teams_by_number[team]):
            self.on_player_restriction_added(player, weapon)

    @staticmethod
    def on_player_carrying_restricted_weapon(player, weapon):
        """Force the player to drop the weapon.

        :param Player player: The player that is carrying a restricted weapon.
        :param Weapon weapon: The weapon that the player is carrying
            and is newly restricted.
        """
        player.drop_weapon(weapon.pointer, None, None)

    @staticmethod
    def _get_player_weapon_index(player, weapon):
        """Return the index of the weapon if the player is carrying one."""
        # Return the index
        for weapon_index in player.weapon_indexes(weapon_manager[weapon].name):
            return weapon_index

        # If no index was found, return invalid
        return INVALID_ENTITY_INDEX

    def _unload_instance(self):
        """Remove the instance from the manager."""
        weapon_restriction_manager.remove_handler(self)

# Get the default WeaponRestrictionHandler
#   instance to be used by the Player class.
weapon_restriction_handler = WeaponRestrictionHandler()


# =============================================================================
# >> FUNCTION HOOKS
# =============================================================================
@EntityPreHook(EntityCondition.is_human_player, 'bump_weapon')
@EntityPreHook(EntityCondition.is_bot_player, 'bump_weapon')
def _on_weapon_bump(args):
    """Return whether the player is allowed to pickup the weapon."""
    return weapon_restriction_manager.on_player_bumping_weapon(
        make_object(Player, args[0]), edict_from_pointer(args[1]).classname)


if GAME_NAME in ('css', 'csgo'):
    @EntityPreHook(EntityCondition.is_player, 'buy_internal')
    def _on_weapon_purchase(args):
        """Return whether the player is allowed to purchase the weapon."""
        # TODO:
        # In CS:GO it seems like the weapon isn't passed as a string anymore.
        # Instead it's rather a pointer that might be NULL. If it's not NULL,
        # the function sets it to some value:
        #if ( a3 )
        #        *(_DWORD *)a3 = v16;
        return weapon_restriction_manager.on_player_purchasing_weapon(
            make_object(Player, args[0]),
            args[1 if SOURCE_ENGINE != 'csgo' else 2]
        )


# =============================================================================
# >> LISTENERS
# =============================================================================
@OnLevelInit
def _level_init(map_name):
    """Clear all restrictions."""
    weapon_restriction_manager.clear()
