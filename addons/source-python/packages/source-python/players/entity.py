# ../players/entity.py

"""Provides a class used to interact with a specific player."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Python Imports
#   Math
from math import cos
from math import radians
from math import sin

# Source.Python Imports
#   Constants
from constants import MAX_TRACE_LENGTH
#   Engines
from engines.trace import engine_trace
from engines.trace import ContentMasks
from engines.trace import GameTrace
from engines.trace import Ray
from engines.trace import TraceFilterSimple
#   Entities
from entities.entity import BaseEntity
from entities.helpers import index_from_pointer
#   Mathlib
from mathlib import Vector
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

    def get_view_player(self):
        """Return the player that the player is looking at."""
        # Get the entity that the player is looking at
        entity = self.get_view_entity()

        # Return a PlayerEntity instance of the player or None if not a player
        return PlayerEntity(
            entity.index) if entity.classname == 'player' else None

    def get_eye_location(self):
        """Return the eye location of the player."""
        return Vector(*tuple(self.get_property_float(
            'localdata.m_vecViewOffset[{0}]'.format(x)) + y for x, y in
            enumerate(self.origin)))

    def get_view_vector(self):
        """Return the view vector of the player."""
        eye_angle0 = self.get_property_float('m_angEyeAngles[0]')
        eye_angle1 = self.get_property_float('m_angEyeAngles[1]')
        return Vector(
            cos(radians(eye_angle1)),
            sin(radians(eye_angle1)),
            -1 * sin(radians(eye_angle0))
        )
