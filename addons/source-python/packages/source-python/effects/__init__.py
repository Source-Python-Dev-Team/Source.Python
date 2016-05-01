# ../effects.py

"""Provides access to all effects within the engine."""

# ============================================================================
# >> IMPORTS
# ============================================================================
# Source.Python Imports
#   Mathlib
from mathlib import Vector
#   Memory
from memory.manager import manager
#   Entities
from entities.entity import Entity


# =============================================================================
# >> FORWARD IMPORTS
# =============================================================================
# Source.Python Imports
#   Effects
from _effects import DispatchEffectData
from _effects import PredictionSystem
from _effects import ShatterSurface


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('DispatchEffectData',
           'PredictionSystem',
           'ShatterSurface',
           'ball',
           'beam',
           'box',
           'polygon',
           'square',
           'temp_entities',
           )


# ============================================================================
# >> GLOBAL VARIABLES
# ============================================================================
# Get the temp_entities instance...
try:
    from _effects import temp_entities
except ImportError:
    temp_entities = manager.get_global_pointer('_TempEntities')

# Get the first temp entity in the chain...
try:
    from _effects._base import _first_temp_entity
except ImportError:
    _first_temp_entity = manager.get_global_pointer('BaseTempEntity')


# =============================================================================
# >> FUNCTIONS
# =============================================================================
def beam(
        recipients, delay, start, end, model_index, halo_index, start_frame,
        frame_rate, life, width, end_width, fade_length, amplitude, red,
        green, blue, alpha, speed, parent=False):
    """A simple wrapper for _TempEntities.beam_ent_point().

    <start> and <end> can be entity indexes or Vector objects.

    If you set <parent> to True, the beam is parented to the given entity
    index(es).
    """
    start_entity = -1
    start_vector = None
    end_entity = -1
    end_vector = None

    # Check start point
    if isinstance(start, int):
        if parent:
            start_entity = start
        else:
            start_vector = Entity(start).origin
    else:
        start_vector = start

    # Check end point
    if isinstance(end, int):
        if parent:
            end_entity = end
        else:
            end_vector = Entity(end).origin
    else:
        end_vector = end

    # Create the effect
    temp_entities.beam_ent_point(
        recipients, delay, start_entity, start_vector, end_entity, end_vector,
        model_index, halo_index, start_frame, frame_rate, life, width,
        end_width, fade_length, amplitude, red, green, blue, alpha, speed)


def polygon(
        recipients, delay, points, model_index, halo_index, start_frame,
        frame_rate, life, width, end_width, fade_length, amplitude, red,
        green, blue, alpha, speed, parent=False):
    """Create a polygon by using _TempEntities.beam_points().

    <points> must be an iterable with at least 3 elements.

    See beam() for more information.
    """
    if len(points) < 3:
        raise ValueError('At least 3 points are required.')

    start = points[0]
    points = dict(enumerate(points))
    for index, point in points.items():
        beam(
            recipients, delay, point, points.get(index + 1, start),
            model_index, halo_index, start_frame, frame_rate, life, width,
            end_width, fade_length, amplitude, red, green, blue, alpha, speed,
            parent)


def square(
        recipients, delay, start, end, model_index, halo_index, start_frame,
        frame_rate, life, width, end_width, fade_length, amplitude, red,
        green, blue, alpha, speed):
    """Create a square by using _TempEntities.beam_points()."""
    if not isinstance(start, Vector) or not isinstance(end, Vector):
        raise TypeError('"start" and "end" must be Vector objects.')

    x1, y1, z1 = tuple(start)
    x2, y2, z2 = tuple(end)

    a = start
    b = Vector(x2, y2, z1)
    c = Vector(x1, y1, z2)
    d = end

    lines = (
        (a, b),
        (b, d),
        (d, c),
        (c, a),
    )

    for p1, p2 in lines:
        temp_entities.beam_points(
            recipients, delay, p1, p2, model_index,
            halo_index, start_frame, frame_rate, life, width, end_width,
            fade_length, amplitude, red, green, blue, alpha, speed)


def box(
        recipients, delay, start, end, model_index, halo_index, start_frame,
        frame_rate, life, width, end_width, fade_length, amplitude, red,
        green, blue, alpha, speed):
    """Create a box by using _TempEntities.beam_points()."""
    if not isinstance(start, Vector) or not isinstance(end, Vector):
        raise TypeError('"start" and "end" must be Vector objects.')

    x1, y1, z1 = tuple(start)
    x2, y2, z2 = tuple(end)

    a = start
    b = Vector(x2, y1, z1)
    c = Vector(x2, y1, z2)
    d = Vector(x1, y1, z2)
    e = Vector(x1, y2, z1)
    f = Vector(x2, y2, z1)
    h = Vector(x1, y2, z2)
    g = end

    lines = (
        # First square
        (a, b),
        (b, c),
        (c, d),
        (d, a),

        # Second square
        (e, f),
        (f, g),
        (g, h),
        (h, e),

        # Connection between both squares
        (a, e),
        (b, f),
        (c, g),
        (d, h)
    )

    for p1, p2 in lines:
        temp_entities.beam_points(
            recipients, delay, p1, p2, model_index,
            halo_index, start_frame, frame_rate, life, width, end_width,
            fade_length, amplitude, red, green, blue, alpha, speed)


def ball(
        recipients, delay, center, radius, model_index, halo_index,
        start_frame, frame_rate, life, width, spread, amplitude, red, green,
        blue, alpha, speed, flags, steps=15, upper_half=True,
        lower_half=True):
    """Create a ball by using _TempEntities.beam_ring_point().

    NOTE:
    The number of steps is used for the lower and upper half. So, if you define
    15 steps, 29 rings are created (the center ring is shared by both halves).
    """
    if not isinstance(center, Vector):
        raise TypeError('"center" must be Vector object.')

    step = float(radius) / steps
    for x in range(steps):
        dist = step * x
        org = Vector(*center)
        org.z += dist
        rad = 2 * radius * (1 - (float(x) / steps) ** 2) ** 0.5
        args = (
            recipients, delay, org, rad, rad - 0.1, model_index,
            halo_index, start_frame, frame_rate, life, width, spread,
            amplitude, red, green, blue, alpha, speed, flags)

        if upper_half:
            temp_entities.beam_ring_point(*args)

        if x and lower_half:
            org.z -= 2 * dist
            temp_entities.beam_ring_point(*args)
