# ../players/entity.py

"""Provides a class used to interact with a specific player."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Python Imports
#   Math
import math

# Source.Python Imports
#   Core
from core import SOURCE_ENGINE_BRANCH
#   Engines
from engines.server import server
from engines.server import engine_server
from engines.trace import engine_trace
from engines.trace import ContentMasks
from engines.trace import GameTrace
from engines.trace import MAX_TRACE_LENGTH
from engines.trace import Ray
from engines.trace import TraceFilterSimple
#   Entities
from entities.constants import CollisionGroup
from entities.constants import INVALID_ENTITY_INDEX
from entities.constants import MoveType
from entities.constants import TakeDamage
from entities.entity import Entity
from entities.helpers import index_from_inthandle
#   Filters
from filters.weapons import WeaponIter
#   Mathlib
from mathlib import Vector
from mathlib import QAngle
#   Memory
import memory
from memory.hooks import PreHook
#   Players
from players import BaseClient
from players.constants import PlayerStates
from players.helpers import address_from_playerinfo
from players.helpers import get_client_language
from players.helpers import playerinfo_from_index
from players.helpers import uniqueid_from_playerinfo
from players.voice import mute_manager
#   Weapons
from weapons.entity import Weapon


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('Player',
           )


# =============================================================================
# >> CLASSES
# =============================================================================
class Player(Entity):
    """Class used to interact directly with players."""

    def __init__(self, index):
        """Initialize the object.

        :param int index: A valid player index.
        :raise ValueError: Raised if the index is invalid.
        """
        super().__init__(index)
        super(Entity, self).__setattr__('_playerinfo', None)

    @property
    def playerinfo(self):
        """Return the player's :class:`PlayerInfo` object."""
        if self._playerinfo is None:
            self._playerinfo = playerinfo_from_index(self.index)

        return self._playerinfo

    @property
    def instances(self):
        """Yield the player's base instances.

        Values yielded are the player's :class:`players.PlayerInfo`,
        :class:`entities.Edict` and :class:`memory.Pointer` objects.
        """
        yield self.playerinfo
        yield self.edict
        yield self.pointer

    @property
    def userid(self):
        """Return the player's userid.

        :rtype: int
        """
        return self.playerinfo.get_userid()

    @property
    def steamid(self):
        """Return the player's SteamID.

        :rtype: str
        """
        return self.playerinfo.get_networkid_string()

    def get_name(self):
        """Return the player's name.

        :rtype: str
        """
        return self.playerinfo.get_name()

    def set_name(self, name):
        """Set the player's name."""
        self.base_client.set_name(name)

    name = property(get_name, set_name)

    @property
    def client(self):
        """Return the player's :class:`players.Client` object."""
        return server.get_client(self.index - 1)

    @property
    def base_client(self):
        """Return the player's :class:`players.BaseClient` object."""
        return memory.make_object(
            BaseClient, memory.get_object_pointer(self.client) - 4)

    @property
    def isdead(self):
        """Return if the player is dead or alive.

        :rtype: bool
        """
        return self.playerinfo.is_dead()

    @property
    def uniqueid(self):
        """Return the player's uniqueid."""
        return uniqueid_from_playerinfo(self.playerinfo)

    @property
    def address(self):
        """Return the player's IP address and port.

        If the player is a bot, '0' will be returned.

        :return: The IP address. E.g. '127.0.0.1:27015'
        :rtype: str
        """
        return address_from_playerinfo(self.playerinfo)

    def get_team(self):
        """Return the player's team.

        :rtype: int
        """
        return self.playerinfo.get_team_index()

    def set_team(self, value):
        """Set the players team."""
        self.playerinfo.change_team(value)

    team = property(get_team, set_team)

    @property
    def language(self):
        """Return the player's language.

        If the player is a bot, an empty string will be returned.

        :rtype: str
        """
        return get_client_language(self.index)

    def get_trace_ray(self, mask=ContentMasks.ALL, trace_filter=None):
        """Return the player's current trace data.

        :param ContentMasks mask: Will be passed to the trace filter.
        :param TraceFilter trace_filter: The trace filter to use. If None was
            given :class:`engines.trace.TraceFilterSimple` will be used.
        :rtype: GameTrace
        """
        # Get the eye location of the player
        start_vec = self.get_eye_location()

        # Calculate the greatest possible distance
        end_vec = start_vec + self.view_vector * MAX_TRACE_LENGTH

        # Create a new trace object
        trace = GameTrace()

        # Start the trace
        engine_trace.trace_ray(
            Ray(start_vec, end_vec), mask, TraceFilterSimple(
                (self.index,)) if trace_filter is None else trace_filter,
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

        :param Vector coords: The coordinates the player should look at.
        """
        coord_eye_vec = coords - self.get_eye_location()

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
        return Entity(trace.get_entity_index())

    def set_view_entity(self, entity):
        """Force the player to look at the origin of the given entity.

        :param Entity entity: The entity the player should look at.
        """
        self.set_view_coordinates(entity.origin)

    view_entity = property(get_view_entity, set_view_entity)

    def get_view_player(self):
        """Return the player that the player is looking at.

        Return None if the player is not looking at a player.

        :rtype: Player
        """
        # Get the entity that the player is looking at
        entity = self.get_view_entity()

        # Return a Player instance of the player or None if not a player
        return (
            Player(entity.index) if entity is not None and
            entity.is_player() else None)

    def set_view_player(self, player):
        """Force the player to look at the other player's eye location.

        :param Player player: The other player.
        """
        self.set_view_coordinates(player.get_eye_location())

    view_player = property(get_view_player, set_view_player)

    def get_eye_location(self):
        """Return the player's eye location.

        :rtype: Vector
        """
        return self.view_offset + self.origin

    def set_eye_location(self, eye_location):
        """Set the player's eye location."""
        self.teleport(eye_location - self.view_offset, None, None)

    eye_location = property(get_eye_location, set_eye_location)

    @property
    def view_vector(self):
        """Return the view vector of the player.

        :rtype: Vector
        """
        eye_angle = self.eye_angle
        return Vector(
            math.cos(math.radians(eye_angle.y)),
            math.sin(math.radians(eye_angle.y)),
            -1 * math.sin(math.radians(self.eye_angle.x))
        )

    def get_view_angle(self):
        """Return the player's view angle.

        :rtype: QAngle
        """
        eye_angle_y = self.eye_angle.y
        eye_angle_y = (eye_angle_y + 360) if eye_angle_y < 0 else eye_angle_y
        return QAngle(self.eye_angle.x, eye_angle_y, self.rotation.z)

    def set_view_angle(self, angle):
        """Set the player's view angle."""
        # Make sure that only QAngle objects are passed. Otherwise you can
        # easily crash the server or cause unexpected behaviour
        assert isinstance(angle, QAngle)
        self.teleport(None, angle, None)

    view_angle = property(get_view_angle, set_view_angle)

    def push(self, horiz_mul, vert_mul, vert_override=False):
        """Push the player along his view vector.

        :param float horiz_mul: Horizontal multiplier.
        :param float vert_mul: Vertical multiplier.
        :param bool vert_override: If True ``vert_mul`` will be used as a
            static value and not as a multiplier.
        """
        x, y, z = tuple(self.view_vector)
        self.base_velocity = Vector(
            x * horiz_mul, y * horiz_mul,
            z * vert_mul if not vert_override else vert_mul)

    def client_command(self, command, server_side=False):
        """Execute a command on the client.

        :param str command: The command to execute.
        :param bool server_side: If True the command will be emulated by the
            server.
        """
        engine_server.client_command(self.edict, command, server_side)

    def slay(self):
        """Slay the player."""
        self.client_command('kill', True)

    def say(self, message):
        """Force the player to say something in the global chat.

        :param str message: The text the player should say.
        """
        self.client_command('say {0}'.format(message), True)

    def say_team(self, message):
        """Force the player to say something in the team chat.

        :param str message: The text the player should say.
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

        :param bool enable: If True noclip mode will be enabled.
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

        :param bool enable: If True jetpack mode will be enabled.
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

        :param bool enable: If True god mode will be enabled.

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

        :param bool enable: If True noblock mode will be enabled.
        """
        if enable:
            self.collision_group = CollisionGroup.DEBRIS_TRIGGER
        else:
            self.collison_group = CollisionGroup.PLAYER

    def get_noblock(self):
        """Return whether noblock mode is enabled.

        :rtype: bool
        """
        return self.collision_group == CollisionGroup.DEBRIS_TRIGGER

    noblock = property(get_noblock, set_noblock)

    def set_freeze(self, enable):
        """Enable/disable freeze mode.

        Freeze mode makes the player unable to move, look and shoot.

        :param bool enable: If True freeze mode will be enabled.
        """
        if enable:
            self.flags |= PlayerStates.FROZEN
        else:
            self.flags &= ~PlayerStates.FROZEN

    def get_freeze(self):
        """Return whether freeze mode is enabled.

        :rtype: bool
        """
        return bool(self.flags & PlayerStates.FROZEN)

    freeze = property(get_freeze, set_freeze)

    def set_stuck(self, enable):
        """Enable/disable stuck mode.

        Stuck mode forces the player to stay exactly at his current position
        even if he is currently in the air. He's still able to look and shoot.

        :param bool enable: If True stuck mode will be enabled.
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

    # =========================================================================
    # >> WEAPONS
    # =========================================================================
    def get_active_weapon(self):
        """Return the player's active weapon.

        :rtype: Weapon
        """
        index = index_from_inthandle(self.active_weapon_handle)
        if index is INVALID_ENTITY_INDEX:
            return None
        return Weapon(index)

    active_weapon = property(get_active_weapon)

    def get_weapons(self, is_filters=None, not_filters=None):
        """Return the player's weapons.

        :return: A generator of :class:`weapons.entity.Weapon` objects
        :rtype: generator
        """
        for weapon in WeaponIter(is_filters, not_filters):
            if index_from_inthandle(weapon.owner) == self.index:
                yield weapon

    def get_primary_weapon(self):
        """Return the player's primary weapon.

        :rtype: Weapon
        """
        try:
            return next(self.get_weapons(is_filters='primary'))
        except ValueError:
            return None

    primary_weapon = property(get_primary_weapon)

    def get_secondary_weapon(self):
        """Return the player's secondary weapon.

        :rtype: Weapon
        """
        try:
            return next(self.get_weapons(is_filters='secondary'))
        except ValueError:
            return None

    secondary_weapon = property(get_secondary_weapon)


# =============================================================================
# >> CALLBACKS
# =============================================================================
if SOURCE_ENGINE_BRANCH in ('css', 'csgo'):
    @PreHook(memory.get_virtual_function(engine_server, 'ClientCommand'))
    def _pre_client_command(args):
        """Block name changes started by the server.

        Pre-hook on IVEngineServer::ClientCommand to block the name changes.
        """
        if args[2] == 'name "%s"':
            return 0
