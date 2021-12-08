# ../entities/collisions.py

"""Provides entity collisions functionality."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
#   Core
from core import GAME_NAME
from core import AutoUnload
#   Engines
from engines.trace import ContentFlags
from engines.trace import ContentMasks


# =============================================================================
# >> FORWARD IMPORTS
# =============================================================================
# Source.Python Imports
#   Entities
from _entities._collisions import CollisionFilter
from _entities._collisions import CollisionManager
from _entities._collisions import CollisionTable
from _entities._collisions import collision_manager


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('SOLID_MASKS',
           'CollisionFilter',
           'CollisionHook',
           'CollisionManager',
           'CollisionTable',
           'collision_manager',
           )


# =============================================================================
# >> INITIALIZATION
# =============================================================================
# Inject AutoUnload into CollisionFilter's hierarchy.
if not issubclass(CollisionFilter, AutoUnload):
    CollisionFilter.__bases__ = (AutoUnload,) + CollisionFilter.__bases__


# =============================================================================
# >> CLASSES
# =============================================================================
class CollisionHook(AutoUnload):
    """Decorator used to create collision hooks that auto unload."""

    def __init__(self, callback):
        """Registers the collision hook."""
        self.callback = callback
        collision_manager.register_hook(callback)

    def _unload_instance(self):
        """Unregisters the collision hook."""
        collision_manager.unregister_hook(self.callback)
