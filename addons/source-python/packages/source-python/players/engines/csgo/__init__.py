# ../players/engines/csgo/__init__.py

"""Provides CS:GO specific Player based functionality."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
#   Bitbuffers
from bitbuffers import BitBufferWrite
#   ConVars
from cvars import ConVar
#   Engines
from engines.server import engine_server
#   Entities
from entities.helpers import wrap_entity_mem_func
#   Filters
from filters.entities import EntityIter
#   Memory
from memory import NULL
from memory import get_virtual_function
from memory.hooks import PreHook
#   Messages
from _messages import ProtobufMessage
#   Players
from players._base import Player as _Player
from players.constants import LifeState
#   Weapons
from weapons.manager import weapon_manager


# =============================================================================
# >> GLOBAL VARIABLES
# =============================================================================
_disable_immunity_alpha = ConVar('sv_disable_immunity_alpha')


# =============================================================================
# >> CLASSES
# =============================================================================
class Player(_Player):
    """Class used to interact directly with players."""

    # Instances of this class will be cached by default
    caching = True

    def _get_kills(self):
        """Return the number of kills the player has."""
        return super().kills

    def _set_kills(self, value):
        """Set the number of kills the player has."""
        self.increment_frag_count(value - self.kills, 0)

    kills = property(
        _get_kills, _set_kills, doc="""The number of kills a player has.""")

    def _get_deaths(self):
        """Return the number of deaths the player has."""
        return super().deaths

    def _set_deaths(self, value):
        """Set the number of deaths the player has."""
        self.increment_death_count(value - self.deaths)

    deaths = property(
        _get_deaths, _set_deaths, doc="""The number of deaths a player has.""")

    def _get_assists(self):
        """Return the number of assists the player has."""
        return super().assists

    def _set_assists(self, value):
        """Set the number of assists the player has."""
        self.increment_assists_count(value - self.assists)

    assists = property(
        _get_assists, _set_assists,
        doc="""The number of assists a player has.""")

    @_Player.godmode.getter
    def godmode(self):
        """Return whether god mode is enabled.
        :rtype: bool
        """
        return super().get_godmode() or self.gungame_immunity

    def send_convar_value(self, cvar_name, value):
        """Send a convar value.

        :param str cvar_name:
            Name of the convar.
        :param str value:
            Value to send.
        """
        buffer_size = 256
        buffer = BitBufferWrite(buffer_size)
        msg = ProtobufMessage('CNETMsg_SetConVar')
        cvar = msg.mutable_message('convars').add_message('cvars')
        cvar.set_string('name', cvar_name)
        cvar.set_string('value', str(value))

        msg_size = msg.byte_size
        buffer.write_var_int32(6)
        buffer.write_var_int32(msg_size)
        msg.serialize_to_array(
            buffer.data + buffer.num_bytes_written, buffer_size)
        buffer.seek_to_bit((buffer.num_bytes_written + msg_size) * 8)
        self.client.net_channel.send_data(buffer)

    def has_c4(self):
        """Return whether or not the player is carrying C4."""
        # Loop through all c4 entities on the server
        for weapon in EntityIter('weapon_c4'):

            # Is the entity's "owner" the player?
            if weapon.owner_handle == self.inthandle:

                # Return True
                return True

        # If no c4 is owned by the player, return False
        return False

    def get_projectile_ammo(self, projectile):
        """Return the player's ammo value of the given projectile.

        :param str projectile:
            The name of the projectile to get the ammo of.
        :rtype: int
        """
        return self.get_property_ushort(
            '{base}{prop:03d}'.format(
                base=weapon_manager.ammoprop,
                prop=weapon_manager[projectile].ammoprop,
            )
        )

    def set_projectile_ammo(self, projectile, value):
        """Set the player's ammo value for the given projectile.

        :param str projectile:
            The name of the projectile to set the ammo of.
        :param int value:
            The value to set the projectile's ammo to.
        """
        self.set_property_ushort(
            '{base}{prop:03d}'.format(
                base=weapon_manager.ammoprop,
                prop=weapon_manager[projectile].ammoprop,
            ),
            value,
        )

    def spawn(self, force=False):
        """Spawn the player.

        :param bool force:
            Whether or not the spawn should be forced.
        """
        # Is the player spawnable?
        if not force and (self.team <= 1 or not self.dead):
            return

        # Spawn the player...
        self._spawn()

    @wrap_entity_mem_func
    def give_named_item(
            self, item, sub_type=0, econ_item_view=None, unk=False, unk2=NULL):
        """Give the player a named item."""
        # TODO: What's the unk argument for?
        # unk2 is a Vector (position)? Should do some tests...
        return [item, sub_type, econ_item_view, unk, unk2]


# =============================================================================
# >> CALLBACKS
# =============================================================================
@PreHook(get_virtual_function(engine_server, 'ClientCommand'))
def _pre_client_command(args):
    """Block name changes started by the server.

    Pre-hook on IVEngineServer::ClientCommand to block the name changes.
    """
    if args[2] == 'name "%s"':
        return 0
