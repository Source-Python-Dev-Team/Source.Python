# ../effects/__init__.py

"""Provides access to all effects within the engine."""

# ============================================================================
# >> IMPORTS
# ============================================================================
# Source.Python Imports
#   Effects
from effects.base import TempEntity
#   Mathlib
from mathlib import Vector
#   Memory
from memory.manager import manager
#   Entities
from entities.entity import BaseEntity
from entities.entity import Entity


# =============================================================================
# >> FORWARD IMPORTS
# =============================================================================
# Source.Python Imports
#   Effects
from _effects import ShatterSurface
from _effects import BloodColor


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('BloodColor',
           'ShatterSurface',
           'ball',
           'beam',
           'box',
           'polygon',
           'square',
           )


# ============================================================================
# >> GLOBAL VARIABLES
# ============================================================================
# Get the first temp entity in the chain...
try:
    from _effects._base import _first_temp_entity
except ImportError:
    _first_temp_entity = manager.get_global_pointer('BaseTempEntity')


# =============================================================================
# >> FUNCTIONS
# =============================================================================
def beam(recipients, start, end, parent=False, **kwargs):
    """A simple wrapper for the temporary effect ``BeamEntPoint``.

    :param RecipientFilter recipients:
        Players that should see the beam.
    :param int/BaseEntity/Vector start:
        The start location of the beam.
    :param int/BaseEntity/Vector end:
        The end location of the beam.
    :param bool parent:
        If True, the beam will be parented to the given entities.
    :param kwargs:
        Additional attributes that will be send to the effect.
    """
    _entity = kwargs.pop('_entity', None)
    if _entity is None:
        entity = TempEntity('BeamEntPoint')
        for attr, value in kwargs.items():
            setattr(entity, attr, value)
    else:
        entity = _entity

    def get_vec(value):
        if isinstance(value, int):
            return Entity(value).origin
        elif isinstance(value, BaseEntity):
            return Entity(value.index).origin
        else:
            return value

    if parent:
        if isinstance(start, int):
            entity.start_entity_index = start
        elif isinstance(start, BaseEntity):
            entity.start_entity_index = start.index
        else:
            entity.start_point = start
    else:
        entity.start_point = get_vec(start)

    if parent:
        if isinstance(end, int):
            entity.end_entity_index = end
        elif isinstance(end, BaseEntity):
            entity.end_entity_index = end.index
        else:
            entity.end_point = end
    else:
        entity.end_point = get_vec(end)

    for attr, value in kwargs.items():
        setattr(entity, attr, value)

    entity.create(recipients)


def polygon(recipients, points, parent=False, **kwargs):
    """Create a polygon using :func:`beam`.

    :param RecipientFilter recipients:
        Players that should see the beam.
    :param iterable points:
        An iterable that contains at least 3 :class:`mathlib.Vector` instances.
    :param bool parent:
        If True, the beam will be parented to the given entities.
    :param kwargs:
        Additional attributes that will be send to the effect.
    """
    if len(points) < 3:
        raise ValueError('At least 3 points are required.')

    start = points[0]
    points = dict(enumerate(points))

    entity = TempEntity('BeamEntPoint')
    for attr, value in kwargs.items():
        setattr(entity, attr, value)

    for index, point in points.items():
        beam(
            recipients, point, points.get(index + 1, start), parent,
            _entity=entity, **kwargs)


def square(recipients, start, end, **kwargs):
    """Create a square using the temporary effect ``BeamPoints``.

    :param RecipientFilter recipients:
        Players that should see the beam.
    :param Vector start:
        Upper left corner of the square.
    :param Vector end:
        Lower right corner of the square.
    :param kwargs:
        Additional attributes that will be send to the effect.
    """
    x1, y1, z1 = start
    x2, y2, z2 = end

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

    entity = TempEntity('BeamPoints')
    for attr, value in kwargs.items():
        setattr(entity, attr, value)

    for p1, p2 in lines:
        entity.start_point = p1
        entity.end_point = p2

        entity.create(recipients)


def box(recipients, start, end, **kwargs):
    """Create a box using the temporary effect ``BeamPoints``.

    :param RecipientFilter recipients:
        Players that should see the beam.
    :param Vector start:
        Upper left corner of the box.
    :param Vector end:
        Lower right corner of the box.
    :param kwargs:
        Additional attributes that will be send to the effect.
    """
    x1, y1, z1 = start
    x2, y2, z2 = end

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

    entity = TempEntity('BeamPoints')
    for attr, value in kwargs.items():
        setattr(entity, attr, value)

    for p1, p2 in lines:
        entity.start_point = p1
        entity.end_point = p2

        entity.create(recipients)


def ball(recipients, center, radius, steps=15, upper_half=True, lower_half=True, **kwargs):
    """Create a ball by using the remporary effect ``BeamRingPoint``.

    :param RecipientFilter recipients:
        Players that should see the beam.
    :param Vector center:
        The center location of the ball.
    :param float radius:
        The radius of the ball.
    :param int steps:
        Number of rings that should be used to create the ball.
    :param bool upper_half:
        If False, the upper half of the ball isn't created.
    :param bool lower_half:
        If False, the lower half of the ball isn't created.

    .. note::

        The number of steps is used for the lower and upper half. So, if you
        define 15 steps, 29 rings are created (the center ring is shared by
        both halves).
    """
    # Make sure that at least one argument is True
    assert not (upper_half == lower_half == False)

    entity = TempEntity('BeamRingPoint')
    for attr, value in kwargs.items():
        setattr(entity, attr, value)

    step = float(radius) / steps
    for x in range(steps):
        dist = step * x
        org = Vector(*center)
        org.z += dist
        rad = 2 * radius * (1 - (float(x) / steps) ** 2) ** 0.5

        if upper_half:
            entity.center = org
            entity.start_radius = rad
            entity.end_radius = rad - 0.1

            entity.create(recipients)

        if x and lower_half:
            org.z -= 2 * dist
            entity.center = org
            entity.start_radius = rad
            entity.end_radius = rad - 0.1

            entity.create(recipients)
