# ../weapons/_base.py

"""Provides simplified weapon functionality."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
#   Entities
from entities.entity import Entity
#   Weapons
from _weapons._entity import WeaponMixin
from weapons.manager import weapon_manager


# =============================================================================
# >> GLOBAL VARIABLES
# =============================================================================
# Add all the global variables to __all__
__all__ = ('Weapon',
           )


# =============================================================================
# >> CLASSES
# =============================================================================
class Weapon(WeaponMixin, Entity):
    """Allows easy usage of the weapon's attributes."""

    def __init__(self, index, caching=True):
        """Initialize the object.

        :param int index:
            A valid weapon index.
        :param bool caching:
            Whether to lookup the cache for an existing instance or not.
        :raise ValueError:
            Raised if the index is invalid.
        """
        WeaponMixin.__init__(self, index)
        Entity.__init__(self, index)

    def _validate_clip(self):
        """Test if the weapon has a clip."""
        if (
            self.classname in weapon_manager and
            weapon_manager[self.classname].clip is None
        ) or self._clip == -1:
            raise ValueError('Weapon does not have a clip.')

    def get_clip(self):
        """Return the amount of ammo in the weapon's clip."""
        self._validate_clip()
        return self._clip

    def set_clip(self, value):
        """Set the amount of ammo in the weapon's clip."""
        self._validate_clip()
        self._clip = value

    # Set the "clip" property methods
    clip = property(
        get_clip, set_clip,
        doc="""Property to get/set the weapon's clip.""")

    def _validate_ammo(self):
        """Test if the weapon has a valid ammoprop and an owner."""
        if (
            self.classname in weapon_manager and
            weapon_manager[self.classname].ammoprop is None
        ) or self.ammoprop == -1:
            raise ValueError(
                'Unable to get ammoprop for {weapon}'.format(
                    weapon=self.classname
                )
            )

        player = self.owner
        if player is None:
            raise ValueError('Unable to get the owner of the weapon.')

        return player

    def get_ammo(self):
        """Return the amount of ammo the player has for the weapon."""
        player = self._validate_ammo()
        return player.get_property_int(
            '{base}{prop:03d}'.format(
                base=weapon_manager.ammoprop,
                prop=self.ammoprop,
            )
        )

    def set_ammo(self, value):
        """Set the player's ammo property for the weapon."""
        player = self._validate_ammo()
        player.set_property_int(
            '{base}{prop:03d}'.format(
                base=weapon_manager.ammoprop,
                prop=self.ammoprop,
            ),
            value,
        )

    # Set the "ammo" property methods
    ammo = property(
        get_ammo, set_ammo,
        doc="""Property to get/set the weapon's ammo.""")

    def _validate_secondary_fire_clip(self):
        """Test if the weapon has a secondary fire clip."""
        if self._secondary_fire_clip == -1:
            raise ValueError('Weapon does not have a secondary fire clip.')

    def get_secondary_fire_clip(self):
        """Return the amount of ammo in the weapon's secondary fire clip."""
        self._validate_secondary_fire_clip()
        return self._secondary_fire_clip

    def set_secondary_fire_clip(self, value):
        """Set the amount of ammo in the weapon's secondary fire clip."""
        self._validate_secondary_fire_clip()
        self._secondary_fire_clip = value

    # Set the "secondary_fire_clip" property methods
    secondary_fire_clip = property(
        get_secondary_fire_clip, set_secondary_fire_clip,
        doc="""Property to get/set the weapon's secondary fire clip.""")

    def _validate_secondary_fire_ammo(self):
        """Test if the weapon has a valid secondary fire ammoprop and an owner."""
        if self.secondary_fire_ammoprop == -1:
            raise ValueError(
                'Unable to get secondary fire ammoprop for {0}'.format(
                    self.classname))

        player = self.owner
        if player is None:
            raise ValueError('Unable to get the owner of the weapon.')

        return player

    def get_secondary_fire_ammo(self):
        """Return the secondary fire ammo the player has for the weapon."""
        player = self._validate_secondary_fire_ammo()
        return player.get_property_int(
            '{base}{prop:03d}'.format(
                base=weapon_manager.ammoprop,
                prop=self.secondary_fire_ammoprop,
            )
        )

    def set_secondary_fire_ammo(self, value):
        """Set the player's secondary fire ammo property for the weapon."""
        player = self._validate_secondary_fire_ammo()
        player.set_property_int(
            '{base}{prop:03d}'.format(
                base=weapon_manager.ammoprop,
                prop=self.secondary_fire_ammoprop,
            ),
            value,
        )

    # Set the "secondary_fire_ammo" property methods
    secondary_fire_ammo = property(
        get_secondary_fire_ammo, set_secondary_fire_ammo,
        doc="""Property to get/set the weapon's secondary fire ammo.""")

    @property
    def weapon_name(self):
        """Return the full class name of the weapon."""
        return self.classname

    def remove(self):
        """Remove the weapon."""
        # Avoid a cyclic import
        from players.entity import Player

        owner = self.owner
        if owner is not None and owner.is_player():
            player = Player(owner.index)
            player.drop_weapon(self)

        super().remove()
