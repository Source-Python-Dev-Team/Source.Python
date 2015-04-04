# ../players/entity.py

"""Provides a class used to interact with a specific player."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Python Imports
#   Math
import math

# Source.Python Imports
#   Engines
from engines.trace import engine_trace
from engines.trace import ContentMasks
from engines.trace import GameTrace
from engines.trace import MAX_TRACE_LENGTH
from engines.trace import Ray
from engines.trace import TraceFilterSimple
#   Entities
from entities.entity import BaseEntity
from entities.helpers import index_from_pointer
#   Mathlib
from mathlib import Vector
from mathlib import QAngle
#   Memory
from memory import Pointer
#   Players
from players.helpers import address_from_playerinfo
from players.helpers import get_client_language
from players.helpers import playerinfo_from_index
from players.helpers import uniqueid_from_playerinfo
from players.games import _GameWeapons
from players.weapons import _PlayerWeapons


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('PlayerEntity',
           )


# =============================================================================
# >> CLASSES
# =============================================================================
class PlayerEntity(BaseEntity, _GameWeapons, _PlayerWeapons):

    """Class used to interact directly with players."""

    def __new__(cls, index):
        """Set the "entities" attribute and set the PlayerInfo."""
        # Get the "self" object using the super class' __new__
        self = super(PlayerEntity, cls).__new__(cls, index)

        # Set the player's playerinfo attribute
        self._playerinfo = playerinfo_from_index(self.index)

        # Is the IPlayerInfo instance valid?
        if self.playerinfo is None:

            raise ValueError(
                'Invalid IPlayerInfo instance for index "{0}"'.format(index))

        # Return the instance
        return self

    @property
    def playerinfo(self):
        """Return the player's IPlayerInfo instance."""
        return self._playerinfo

    @property
    def instances(self):
        """Yield the player's IPlayerInfo and Edict instances."""
        yield self.playerinfo
        yield self.edict
        yield self.pointer

    @property
    def userid(self):
        """Return the player's userid."""
        return self.playerinfo.get_userid()

    @property
    def steamid(self):
        """Return the player's SteamID."""
        return self.playerinfo.get_networkid_string()

    @property
    def name(self):
        """Return the player's name."""
        return self.playerinfo.get_name()

    @property
    def isdead(self):
        """Return if the player is dead or alive."""
        return self.playerinfo.is_dead()

    @property
    def uniqueid(self):
        """Return the player's uniqueid."""
        return uniqueid_from_playerinfo(self.playerinfo)

    @property
    def address(self):
        """Return the player's IP address."""
        return address_from_playerinfo(self.playerinfo)

    def get_team(self):
        """Return the player's team."""
        return self.playerinfo.get_team_index()

    def set_team(self, value):
        """Set the players team."""
        self.playerinfo.change_team(value)

    # Set the "team" property methods
    team = property(
        get_team, set_team,
        doc="""Property to get/set the player's team.""")

    @property
    def language(self):
        """Return the player's language."""
        return get_client_language(self.index)

    def get_trace_ray(self, mask=ContentMasks.ALL, trace_filter=None):
        """Return the player's current trace data."""
        # Get the eye location of the player
        start_vec = self.get_eye_location()

        # Calculate the greatest possible distance
        end_vec = start_vec + self.get_view_vector() * MAX_TRACE_LENGTH

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
        """Return the coordinates the player is currently looking at."""
        # Get the player's current trace data
        trace = self.get_trace_ray()

        # Return the end position of the trace if it hit something
        return trace.end_position if trace.did_hit() else None

    def set_view_coordinates(self, coords):
        """Force the player to look at the given coordinates."""
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
        self.teleport(
            None,
            QAngle(x_angle, y_angle, self.get_property_vector('m_angRotation').z),
            None
        )

    view_coordinates = property(get_view_coordinates, set_view_coordinates)

    def get_view_entity(self):
        """Return the entity that the player is looking at."""
        # Get the player's current trace data
        trace = self.get_trace_ray()

        # Did the trace hit?
        if not trace.did_hit():
            return None

        # Get the entity that the trace hit
        pointer = Pointer(trace.get_entity().get_base_entity())

        # Get the index of the entity the trace hit
        index = index_from_pointer(pointer, False)

        # Return a BaseEntity instance of the hit entity if it has an index
        return BaseEntity(index) if index else None

    def set_view_entity(self, entity):
        """Force the player to look at the origin of the given entity."""
        self.set_view_coordinates(entity.origin)

    view_entity = property(get_view_entity, set_view_entity)

    def get_view_player(self):
        """Return the player that the player is looking at."""
        # Get the entity that the player is looking at
        entity = self.get_view_entity()

        # Return a PlayerEntity instance of the player or None if not a player
        return (
            PlayerEntity(entity.index) if entity is not None and
            entity.classname == 'player' else None)

    def set_view_player(self, player):
        """Force the player to look at the other player's eye location."""
        self.set_view_coordinates(player.get_eye_location())

    view_player = property(get_view_player, set_view_player)

    def get_eye_location(self):
        """Return the eye location of the player."""
        return Vector(*tuple(self.get_property_float(
            'localdata.m_vecViewOffset[{0}]'.format(x)) + y for x, y in
            enumerate(self.origin)))

    def get_view_vector(self):
        """Return the view vector of the player."""
        eye_angle_y = self.eye_angle_y
        return Vector(
            math.cos(math.radians(eye_angle_y)),
            math.sin(math.radians(eye_angle_y)),
            -1 * math.sin(math.radians(self.eye_angle_x))
        )

    def get_view_angle(self):
        """Return the view angle."""
        eye_angle_y = self.eye_angle_y
        eye_angle_y = (eye_angle_y + 360) if eye_angle_y < 0 else eye_angle_y
        return QAngle(
            self.eye_angle_x,
            eye_angle_y,
            self.get_property_vector('m_angRotation').z
        )

    def set_view_angle(self, angle):
        """Set the view angle."""
        # Make sure that only QAngle objects are passed. Otherwise you can
        # easily crash the server or cause unexpected behaviour
        assert isinstance(angle, QAngle)
        self.teleport(None, angle, None)

    view_angle = property(get_view_angle, set_view_angle)
