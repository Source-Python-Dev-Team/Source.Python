# ../players/_base.py

"""Provides a class used to interact with a specific player."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Python Imports
#   Math
import math

# Source.Python Imports
#   Bitbuffers
from bitbuffers import BitBufferWrite
#   Core
from core import GAME_NAME
from core.cache import cached_property
from core.cache import cached_result
#   Engines
from engines.server import server
from engines.server import engine_server
from engines.server import queue_server_command
from engines.server import server_game_dll
from engines.sound import Attenuation
from engines.sound import Channel
from engines.sound import Pitch
from engines.sound import Sound
from engines.sound import SoundFlags
from engines.sound import SOUND_FROM_WORLD
from engines.sound import StreamSound
from engines.sound import VOL_NORM
from engines.trace import engine_trace
from engines.trace import ContentMasks
from engines.trace import GameTrace
from engines.trace import MAX_TRACE_LENGTH
from engines.trace import Ray
from engines.trace import TraceFilterSimple
#   Entities
from entities import ServerClassGenerator
from entities.constants import CollisionGroup
from entities.constants import EntityEffects
from entities.constants import INVALID_ENTITY_INDEX
from entities.constants import MoveType
from entities.constants import TakeDamage
from entities.entity import Entity
from entities.helpers import edict_from_index
from entities.helpers import index_from_inthandle
from entities.helpers import wrap_entity_mem_func
from entities.props import SendPropType
#   Events
from events.manager import game_event_manager
#   Filters
from filters.entities import EntityIter
#   KeyValues
from keyvalues import KeyValues
#   Mathlib
from mathlib import NULL_VECTOR
from mathlib import Vector
from mathlib import QAngle
#   Memory
from memory import get_object_pointer
from memory import make_object
#   Players
from _players import PlayerMixin
from players.constants import PlayerStates
from players.helpers import address_from_playerinfo
from players.helpers import get_client_language
from players.helpers import playerinfo_from_index
from players.helpers import index_from_userid
from players.helpers import uniqueid_from_playerinfo
from players.voice import mute_manager
#   Weapons
from weapons.default import NoWeaponManager
from weapons.entity import Weapon
from weapons.manager import weapon_manager
#   Auth
from auth.manager import auth_manager


# =============================================================================
# >> CLASSES
# =============================================================================
class Player(PlayerMixin, Entity):
    """Class used to interact directly with players."""

    def __init__(self, index, caching=True):
        """Initialize the object.

        :param int index:
            A valid player index.
        :param bool caching:
            Whether to lookup the cache for an existing instance or not.
        :raise ValueError:
            Raised if the index is invalid.
        """
        PlayerMixin.__init__(self, index)
        Entity.__init__(self, index)

    @classmethod
    def from_userid(cls, userid, caching=None):
        """Create an instance from a userid.

        :param int userid:
            The userid.
        :param bool caching:
            Whether to lookup the cache for an existing instance or not.
        :rtype: Player
        """
        return cls(index_from_userid(userid), caching=caching)

    @cached_property
    def net_info(self):
        """Return the player's network channel information.
        :return:
            ``None`` if no network channel information exists. E. g. if the
            player is a bot.
        :rtype: NetChannelInfo
        """
        return engine_server.get_player_net_info(self.index)

    @cached_property
    def raw_steamid(self):
        """Return the player's unformatted SteamID.

        :rtype: SteamID
        """
        return engine_server.get_client_steamid(self.edict)

    @property
    def permissions(self):
        """Return the player's permissions.

        :rtype: PlayerPermissions
        """
        return auth_manager.get_player_permissions_from_steamid(self.steamid)

    @cached_property
    def playerinfo(self):
        """Return player information.

        :rtype: PlayerInfo
        """
        return playerinfo_from_index(self.index)

    @cached_property
    def userid(self):
        """Return the player's userid.

        :rtype: int
        """
        return self.playerinfo.userid

    @cached_property
    def steamid(self):
        """Return the player's SteamID.

        :rtype: str
        """
        return self.playerinfo.steamid

    def get_name(self):
        """Return the player's name.

        :rtype: str
        """
        return self.playerinfo.name

    def set_name(self, name):
        """Set the player's name."""
        self.base_client.set_name(name)

    name = property(get_name, set_name)

    @cached_property
    def client(self):
        """Return the player's client instance.

        :rtype: Client
        """
        return server.get_client(self.index - 1)

    @cached_property
    def base_client(self):
        """Return the player's base client instance.

        :rtype: BaseClient
        """
        from players import BaseClient
        return make_object(BaseClient, get_object_pointer(self.client) - 4)

    @cached_property
    def uniqueid(self):
        """Return the player's unique ID.

        :rtype: str
        """
        return uniqueid_from_playerinfo(self.playerinfo)

    @cached_property
    def address(self):
        """Return the player's IP address and port.

        If the player is a bot, an empty string will be returned.

        :return:
            The IP address. E.g. '127.0.0.1:27015'
        :rtype: str
        """
        return address_from_playerinfo(self.playerinfo)

    def is_connected(self):
        """Return whether the player is connected.

        :rtype: bool
        """
        return self.playerinfo.is_connected()

    def is_fake_client(self):
        """Return whether the player is a fake client.

        :rtype: bool
        """
        return self.playerinfo.is_fake_client()

    @cached_result
    def is_hltv(self):
        """Return whether the player is HLTV.

        :rtype: bool
        """
        return self.playerinfo.is_hltv()

    @cached_result
    def is_bot(self):
        """Return whether the player is a bot.

        :rtype: bool
        """
        return self.is_fake_client() or self.steamid == 'BOT'

    def is_in_a_vehicle(self):
        """Return whether the player is in a vehicle.

        :rtype: bool
        """
        return self.playerinfo.is_in_a_vehicle()

    def is_observer(self):
        """Return whether the player is an observer.

        :rtype: bool
        """
        return self.playerinfo.is_observer()

    def get_team(self):
        """Return the player's team.

        :rtype: int
        """
        return self.playerinfo.team

    def set_team(self, value):
        """Set the players team."""
        self.playerinfo.team = value

    team = property(get_team, set_team)

    def get_language(self):
        """Return the player's language.

        If the player is a bot, an empty string will be returned.

        :rtype: str
        """
        return get_client_language(self.index)

    # Only cache the language property for games it is already cached
    if GAME_NAME in ('csgo',):
        language = cached_property(get_language, doc=get_language.__doc__)
    else:
        language = property(get_language, doc=get_language.__doc__)

    def get_trace_ray(self, mask=ContentMasks.ALL, trace_filter=None):
        """Return the player's current trace data.

        :param ContentMasks mask:
            Will be passed to the trace filter.
        :param TraceFilter trace_filter:
            The trace filter to use. If ``None`` was given
            :class:`engines.trace.TraceFilterSimple` will be used.
        :rtype: GameTrace
        """
        # Get the eye location of the player
        start_vec = self.eye_location

        # Calculate the greatest possible distance
        end_vec = start_vec + self.view_vector * MAX_TRACE_LENGTH

        # Create a new trace object
        trace = GameTrace()

        # Start the trace
        engine_trace.trace_ray(
            Ray(start_vec, end_vec), mask, TraceFilterSimple(
                (self,)) if trace_filter is None else trace_filter,
            trace
        )

        # Return the trace data
        return trace

    def get_view_coordinates(self):
        """Return the coordinates the player is currently looking at.

        Return None if the player is not looking at anything.

        :rtype: Vector
        """
        # Get the player's current trace data
        trace = self.get_trace_ray()

        # Return the end position of the trace if it hit something
        return trace.end_position if trace.did_hit() else None

    def set_view_coordinates(self, coords):
        """Force the player to look at the given coordinates.

        :param Vector coords:
            The coordinates the player should look at.
        """
        coord_eye_vec = coords - self.eye_location

        # Calculate the y angle value
        atan = math.degrees(math.atan(coord_eye_vec.y / coord_eye_vec.x))
        if coord_eye_vec.x < 0:
            y_angle = atan + 180
        elif coord_eye_vec.y < 0:
            y_angle = atan + 360
        else:
            y_angle = atan

        # Calculate the x angle value
        x_angle = 0 - math.degrees(math.atan(coord_eye_vec.z / math.sqrt(
            coord_eye_vec.y ** 2 + coord_eye_vec.x ** 2)))

        # Set the new angle
        self.teleport(None, QAngle(x_angle, y_angle, self.rotation.z), None)

    view_coordinates = property(get_view_coordinates, set_view_coordinates)

    def get_view_entity(self):
        """Return the entity that the player is looking at.

        Return None if the player is not looking at an entity.

        :rtype: Entity
        """
        # Get the player's current trace data
        trace = self.get_trace_ray()

        # Did the trace hit?
        if not trace.did_hit():
            return None

        # Return the hit entity as an Entity instance
        return Entity(trace.entity_index)

    def set_view_entity(self, entity):
        """Force the player to look at the origin of the given entity.

        :param Entity entity:
            The entity the player should look at.
        """
        self.view_coordinates = entity.origin

    view_entity = property(get_view_entity, set_view_entity)

    def get_view_player(self):
        """Return the player that the player is looking at.

        Return None if the player is not looking at a player.

        :rtype: Player
        """
        # Get the entity that the player is looking at
        entity = self.view_entity

        # Return a Player instance of the player or None if not a player
        return (
            Player(entity.index) if entity is not None and
            entity.is_player() else None)

    def set_view_player(self, player):
        """Force the player to look at the other player's eye location.

        :param Player player: The other player.
        """
        self.view_coordinates = player.eye_location

    view_player = property(get_view_player, set_view_player)

    def set_eye_location(self, eye_location):
        """Set the player's eye location."""
        self.teleport(eye_location - self.view_offset, None, None)

    eye_location = property(Entity.get_eye_location, set_eye_location)

    def get_view_angle(self):
        """Return the player's view angle.

        :rtype: QAngle
        """
        return super().view_angle

    def set_view_angle(self, angle):
        """Set the player's view angle."""
        # Make sure that only QAngle objects are passed. Otherwise you can
        # easily crash the server or cause unexpected behaviour
        assert isinstance(angle, QAngle)
        self.teleport(None, angle, None)

    view_angle = property(get_view_angle, set_view_angle)

    def push(self, horiz_mul, vert_mul, vert_override=False):
        """Push the player along his view vector.

        :param float horiz_mul:
            Horizontal multiplier.
        :param float vert_mul:
            Vertical multiplier.
        :param bool vert_override:
            If ``True``, ``vert_mul`` will be used as a static value and not
            as a multiplier.
        """
        x, y, z = tuple(self.view_vector)
        self.base_velocity = Vector(
            x * horiz_mul, y * horiz_mul,
            z * vert_mul if not vert_override else vert_mul)

    def client_command(self, command, server_side=False):
        """Execute a command on the client.

        :param str command:
            The command to execute.
        :param bool server_side:
            If ``True`` the command will be emulated by the server.
        """
        engine_server.client_command(self.edict, command, server_side)

    def slay(self):
        """Slay the player."""
        self.client_command('kill', True)

    def say(self, message):
        """Force the player to say something in the global chat.

        :param str message:
            The text the player should say.
        """
        self.client_command('say {0}'.format(message), True)

    def say_team(self, message):
        """Force the player to say something in the team chat.

        :param str message:
            The text the player should say.
        """
        self.client_command('say_team {0}'.format(message), True)

    def mute(self, receivers=None):
        """Mute the player.

        See players.voice.mute_manager.mute_player for more information.
        """
        mute_manager.mute_player(self.index, receivers)

    def unmute(self, receivers=None):
        """Unmute the player.

        See players.voice.mute_manager.unmute_player for more information.
        """
        mute_manager.unmute_player(self.index, receivers)

    def is_muted(self, receivers=None):
        """Return True if the player is currently muted.

        See players.voice.mute_manager.is_muted for more information.
        """
        return mute_manager.is_muted(self.index, receivers)

    def set_noclip(self, enable):
        """Enable/disable noclip mode.

        Noclip mode gives the player the ability to fly through the map.

        :param bool enable:
            If ``True`` noclip mode will be enabled.
        """
        if enable:
            self.move_type = MoveType.NOCLIP
        else:
            self.move_type = MoveType.WALK

    def get_noclip(self):
        """Return whether noclip mode is enabled.

        :rtype: bool
        """
        return self.move_type == MoveType.NOCLIP

    noclip = property(get_noclip, set_noclip)

    def set_jetpack(self, enable):
        """Enable/disable jetpack mode.

        Jetpack mode gives the player the ability to use a jetpack.

        :param bool enable:
            If ``True`` jetpack mode will be enabled.
        """
        if enable:
            self.move_type = MoveType.FLY
        else:
            self.move_type = MoveType.WALK

    def get_jetpack(self):
        """Return whether jetpack mode is enabled.

        :rtype: bool
        """
        return self.move_type == MoveType.FLY

    jetpack = property(get_jetpack, set_jetpack)

    def set_godmode(self, enable):
        """Enable/disable god mode.

        Godmode makes the player invulnerable.

        :param bool enable:
            If ``True`` god mode will be enabled.

        .. todo::

            Add m_takedamage to the data files. Which name do we want to use?
            We can't use take_damage.
        """
        if enable:
            self.set_property_uchar('m_takedamage', TakeDamage.NO)
        else:
            self.set_property_uchar('m_takedamage', TakeDamage.YES)

    def get_godmode(self):
        """Return whether god mode is enabled.

        :rtype: bool

        .. todo::

            Add m_takedamage to the data files. Which name do we want to use?
            We can't use take_damage.
        """
        return self.get_property_uchar('m_takedamage') == TakeDamage.NO

    godmode = property(get_godmode, set_godmode)

    def set_noblock(self, enable):
        """Enable/disable noblock mode.

        Noblock mode assigns a new collision group to the player that doesn't
        block other players. That means players can run through each other.

        :param bool enable:
            If ``True`` noblock mode will be enabled.
        """
        if enable:
            self.collision_group = CollisionGroup.DEBRIS_TRIGGER
        else:
            self.collision_group = CollisionGroup.PLAYER

    def get_noblock(self):
        """Return whether noblock mode is enabled.

        :rtype: bool
        """
        return self.collision_group == CollisionGroup.DEBRIS_TRIGGER

    noblock = property(get_noblock, set_noblock)

    def set_frozen(self, enable):
        """Enable/disable frozen mode.

        Frozen mode makes the player unable to move, look and shoot.

        :param bool enable:
            If ``True`` frozen mode will be enabled.
        """
        if enable:
            self.flags |= PlayerStates.FROZEN
        else:
            self.flags &= ~PlayerStates.FROZEN

    def get_frozen(self):
        """Return whether frozen mode is enabled.

        :rtype: bool
        """
        return bool(self.flags & PlayerStates.FROZEN)

    frozen = property(get_frozen, set_frozen)

    def set_stuck(self, enable):
        """Enable/disable stuck mode.

        Stuck mode forces the player to stay exactly at his current position
        even if he is currently in the air. He's still able to look and shoot.

        :param bool enable:
            If ``True`` stuck mode will be enabled.
        """
        if enable:
            self.move_type = MoveType.NONE
        else:
            self.move_type = MoveType.WALK

    def get_stuck(self):
        """Return whether stuck mode is enabled.

        :rtype: bool
        """
        return self.move_type == MoveType.NONE

    stuck = property(get_stuck, set_stuck)

    def get_flashlight(self):
        """Return whether or not the flashlight of the player is turned on.

        :rtype: bool
        """
        return bool(self.effects & EntityEffects.DIMLIGHT)

    def set_flashlight(self, enable):
        """Turn on/off the flashlight of the player.

        :param bool enable:
            ``True`` to turn on, ``False`` to turn off.
        """
        if enable:
            self.effects |= EntityEffects.DIMLIGHT
        else:
            self.effects &= ~EntityEffects.DIMLIGHT

    flashlight = property(get_flashlight, set_flashlight)

    def send_convar_value(self, cvar_name, value):
        """Send a server convar value to the client.

        :param str cvar_name:
            Name of the convar.
        :param str value:
            Value to send.
        """
        buffer_size = 256
        buffer = BitBufferWrite(buffer_size)
        buffer.write_ubit_long(5, 6)
        buffer.write_byte(1)
        buffer.write_string(cvar_name)
        buffer.write_string(str(value))
        self.client.net_channel.send_data(buffer)

    def get_client_convar_value(self, name):
        """Wrapper for :meth:`engines.server.engine_server.get_client_convar_value`."""
        return engine_server.get_client_convar_value(self.index, name)

    @property
    def spectators(self):
        """Return all players observing this player.

        :return:
            The generator yields :class:`players.entity.Player` objects.
        :rtype: generator
        """
        from filters.players import PlayerIter
        for other in PlayerIter('dead'):
            if self.inthandle == other.observer_target:
                yield other

    def kick(self, message=''):
        """Kick the player from the server.

        :param str message:
            A message the kicked player will receive.
        """
        message = message.rstrip()
        if message:
            self.client.disconnect(message)
        else:
            queue_server_command('kickid', self.userid, message)

    def ban(self, duration=0, kick=True, write_ban=True):
        """Ban a player from the server.

        :param int duration:
            Duration of the ban in minutes. Use 0 for permament.
        :param bool kick:
            If ``True``, the player will be kicked as well.
        :param bool write_ban:
            If ``True``, the ban will be written to ``cfg/banned_users.cfg``.
        """
        queue_server_command(
            'banid', duration, self.userid, 'kick' if kick else '')
        if write_ban:
            queue_server_command('writeid')

    def play_sound(
            self, sample, volume=VOL_NORM, attenuation=Attenuation.NONE,
            channel=Channel.AUTO, flags=SoundFlags.NO_FLAGS,
            pitch=Pitch.NORMAL, origin=NULL_VECTOR, direction=NULL_VECTOR,
            origins=(), update_positions=True, sound_time=0.0,
            speaker_entity=INVALID_ENTITY_INDEX, download=False,
            stream=False):
        """Play a sound to the player.

        :param str sample:
            Sound file relative to the ``sounds`` directory.
        :param float volume:
            Volume of the sound.
        :param Attenuation attenuation:
            How far the sound should reaches.
        :param int channel:
            Channel to emit the sound with.
        :param SoundFlags flags:
            Flags of the sound.
        :param Pitch pitch:
            Pitch of the sound.
        :param Vector origin:
            Origin of the sound.
        :param Vector direction:
            Direction of the sound.
        :param tuple origins:
            Origins of the sound.
        :param bool update_positions:
            Whether or not the positions should be updated.
        :param float sound_time:
            Time to play the sound for.
        :param int speaker_entity:
            Index of the speaker entity.
        :param bool download:
            Whether or not the sample should be added to the downloadables.
        :param bool stream:
            Whether or not the sound should be streamed.
        """
        # Don't bother playing sounds to bots...
        if self.is_fake_client():
            return

        # Get the correct Sound class...
        if not stream:
            sound_class = Sound
        else:
            sound_class = StreamSound

        # Get the sound...
        sound = sound_class(sample, SOUND_FROM_WORLD, volume, attenuation,
            channel, flags, pitch, origin, direction, origins,
            update_positions, sound_time, speaker_entity, download)

        # Play the sound to the player...
        sound.play(self.index)

    def spawn(self, force=False):
        """Spawn the player.

        :param bool force:
            Whether or not the spawn should be forced.
        """
        # Is the player spawnable?
        if not force and (self.team <= 1 or not self.dead):
            return

        # Spawn the player...
        super().spawn()

    def fire_game_event(self, event_name, **kwargs):
        """Creates and fires an event to this player.

        :param str event_name:
            The name of the event to create and fire.
        :param dict kwargs:
            The variables to set to the event.
        """
        # Create the event
        game_event = game_event_manager.create_event(event_name, True)

        # Set the variables
        game_event.variables.recursive_copy(
            KeyValues.from_dict(event_name, kwargs)
        )

        # Fire the event
        self.base_client.fire_game_event(game_event)

        # Free the event
        game_event_manager.free_event(game_event)

    # =========================================================================
    # >> PLAYER WEAPON FUNCTIONALITY
    # =========================================================================
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

    def get_active_weapon(self):
        """Return the player's active weapon.

        :return:
            ``None`` if the player does not have an active weapon.
        :rtype: Weapon
        """
        try:
            index = index_from_inthandle(self.active_weapon_handle)
        except (ValueError, OverflowError):
            return None

        return Weapon(index)

    def set_active_weapon(self, weapon):
        """Set the player's active weapon.

        :param Weapon weapon:
            The weapon to set as active.
        """
        self.active_weapon_handle = weapon.inthandle

    active_weapon = property(get_active_weapon, set_active_weapon)

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

        :return:
            A generator of :class:`weapons.entity.Weapon` objects.
        :rtype: generator
        """
        # Loop through all the players weapons for the given arguments
        for index in self.weapon_indexes(classname, is_filters, not_filters):

            # Yield the current weapon
            yield Weapon(index)

    def weapon_indexes(
            self, classname=None, is_filters=None, not_filters=None):
        """Iterate over the player's weapon indexes for the given arguments.

        :return:
            A generator of indexes.
        :rtype: generator
        """
        # Is the weapon array supported for the current game?
        if _weapon_prop_length is None:
            return

        # Loop through the length of the weapon array
        for offset in range(_weapon_prop_length):

            # Get the player's current weapon at this offset
            handle = self.get_property_int(
                '{base}{offset:03d}'.format(
                    base=weapon_manager.myweapons,
                    offset=offset,
                )
            )

            # Try to get the index of the handle
            try:
                index = index_from_inthandle(handle)
            except (ValueError, OverflowError):
                continue

            # Get the weapon's classname
            weapon_class = edict_from_index(index).classname

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
                if weapon_class not in map(
                        lambda value: value.name,
                        WeaponClassIter(is_filters, not_filters)):

                    # Do not yield this index
                    continue

            # Yield the index
            yield index

    def has_c4(self):
        """Raise an error because this method is game specific."""
        raise NotImplementedError(
            'Method not supported for game "{game}".'.format(game=GAME_NAME)
        )

    def get_projectile_ammo(self, projectile):
        """Return the player's ammo value of the given projectile.

        :param str projectile:
            The name of the projectile to get the ammo of.
        :rtype: int
        """
        return self.get_property_int(
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
        self.set_property_int(
            '{base}{prop:03d}'.format(
                base=weapon_manager.ammoprop,
                prop=weapon_manager[projectile].ammoprop,
            ),
            value,
        )

    def projectile_indexes(self, projectile):
        """Yield all indexes of the given projectile for the player.

        :param str projectile:
            The name of the projectile to find indexes of.
        """
        if projectile in weapon_manager.projectiles:
            for entity in EntityIter(projectile):
                if entity.owner == self:
                    yield entity.index
        else:
            yield from self.weapon_indexes(weapon_manager[projectile].name)

    def restrict_weapons(self, *weapons):
        """Restrict the weapon for the player.

        :param str weapons:
            A weapon or any number of weapons to add as restricted for the
            player.
        """
        from weapons.restrictions import weapon_restriction_handler
        weapon_restriction_handler.add_player_restrictions(self, *weapons)

    def unrestrict_weapons(self, *weapons):
        """Restrict the weapon for the player.

        :param str weapons:
            A weapon or any number of weapons to remove as restricted for the
            player.
        """
        from weapons.restrictions import weapon_restriction_handler
        weapon_restriction_handler.remove_player_restrictions(self, *weapons)

    def is_weapon_restricted(self, weapon):
        """Return whether the player is restricted from the given weapon.

        :param str weapon:
            The name of the weapon to check against restriction.
        :rtype: bool
        """
        from weapons.restrictions import weapon_restriction_manager
        return weapon_restriction_manager.is_player_restricted(self, weapon)

    @wrap_entity_mem_func
    def drop_weapon(self, weapon, target=None, velocity=None):
        """Drop a weapon.

        :param Pointer weapon:
            Weapon to drop.
        :param Vector target:
            Target location to drop the weapon at.
        :param Vector velocity:
            Velocity to use to drop the weapon.
        """
        return [weapon, target, velocity]


# =============================================================================
# >> HELPER FUNCTIONS
# =============================================================================
def _find_weapon_prop_length(table):
    """Loop through a prop table to find the myweapons property length.

    :rtype: int
    """
    # Loop through the props in the table
    for item in table:

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

    # Loop through all ServerClass objects
    for _current_class in ServerClassGenerator():

        # Loop through the ServerClass' props
        _weapon_prop_length = _find_weapon_prop_length(_current_class.table)

        # Was m_hMyWeapons found?
        if _weapon_prop_length is not None:

            # No need to continue looping
            break
