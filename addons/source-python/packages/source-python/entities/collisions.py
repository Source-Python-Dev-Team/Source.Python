# ../entities/collisions.py

"""Provides entity collisions functionality."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
#   Core
from core import AutoUnload


# =============================================================================
# >> FORWARD IMPORTS
# =============================================================================
# Source.Python Imports
#   Entities
from _entities._collisions import CollisionHash


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = [
    'CollisionHash',
]


# =============================================================================
# >> INITIALIZATION
# =============================================================================
# Inject AutoUnload into CollisionHash's hierarchy.
CollisionHash.__bases__ = (AutoUnload,) + CollisionHash.__bases__
