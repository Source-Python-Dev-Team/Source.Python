# ../players/weapons/projectiles.py

"""Provides player projectile weapon functionality."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Python Imports
#   Weapons
from weapons.manager import weapon_manager


# =============================================================================
# >> CLASSES
# =============================================================================
class _HEGrenade(object):
    """Class that interacts with a player based on the hegrenade weapon."""

    def hegrenade_indexes(self):
        """Yield all hegrenade indexes for the player."""
        yield from self.weapon_indexes('weapon_hegrenade')

    def get_hegrenade_ammo(self):
        """Return the amount of hegrenade ammo for the player."""
        return self.get_property_int(
            weapon_manager.ammoprop +
            '%03d' % weapon_manager['hegrenade'].ammoprop)

    def set_hegrenade_ammo(self, value):
        """Set the amount of hegrenade ammo for the player."""
        self.set_property_int(
            weapon_manager.ammoprop +
            '%03d' % weapon_manager['hegrenade'].ammoprop, value)


class _Flashbang(object):
    """Class that interacts with a player based on the flashbang weapon."""

    def flashbang_indexes(self):
        """Yield all flashbang indexes for the player."""
        yield from self.weapon_indexes('weapon_flashbang')

    def get_flashbang_ammo(self):
        """Return the amount of flashbang ammo for the player."""
        return self.get_property_int(
            weapon_manager.ammoprop +
            '%03d' % weapon_manager['flashbang'].ammoprop)

    def set_flashbang_ammo(self, value):
        """Set the amount of flashbang ammo for the player."""
        self.set_property_int(
            weapon_manager.ammoprop +
            '%03d' % weapon_manager['flashbang'].ammoprop, value)


class _SmokeGrenade(object):
    """Class that interacts with a player based on the smokegrenade weapon."""

    def smokegrenade_indexes(self):
        """Yield all smokegrenade indexes for the player."""
        yield from self.weapon_indexes('weapon_smokegrenade')

    def get_smokegrenade_ammo(self):
        """Return the amount of smokegrenade ammo for the player."""
        return self.get_property_int(
            weapon_manager.ammoprop +
            '%03d' % weapon_manager['smokegrenade'].ammoprop)

    def set_smokegrenade_ammo(self, value):
        """Set the amount of smokegrenade ammo for the player."""
        self.set_property_int(
            weapon_manager.ammoprop +
            '%03d' % weapon_manager['smokegrenade'].ammoprop, value)


class _Decoy(object):
    """Class that interacts with a player based on the decoy weapon."""

    def decoy_indexes(self):
        """Yield all decoy indexes for the player."""
        yield from self.weapon_indexes('weapon_decoy')

    def get_decoy_ammo(self):
        """Return the amount of decoy ammo for the player."""
        return self.get_property_int(
            weapon_manager.ammoprop +
            '%03d' % weapon_manager['decoy'].ammoprop)

    def set_decoy_ammo(self, value):
        """Set the amount of decoy ammo for the player."""
        self.set_property_int(
            weapon_manager.ammoprop +
            '%03d' % weapon_manager['decoy'].ammoprop, value)


class _Incendiary(object):
    """Class that interacts with a player based on incendiary weapons."""

    def incendiary_indexes(self):
        """Yield all incendiary indexes for the player."""
        yield from self.weapon_indexes(is_filters='incendiary')

    def get_incendiary_ammo(self):
        """Return the amount of incendiary ammo for the player."""
        return self.get_property_int(
            weapon_manager.ammoprop +
            '%03d' % weapon_manager['molotov'].ammoprop)

    def set_incendiary_ammo(self, value):
        """Set the amount of incendiary ammo for the player."""
        self.set_property_int(
            weapon_manager.ammoprop +
            '%03d' % weapon_manager['molotov'].ammoprop, value)
