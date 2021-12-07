# ../entities/collisions.py

"""Provides entity collisions functionality."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
#   Core
from core import AutoUnload
from core import WeakAutoUnload


# =============================================================================
# >> FORWARD IMPORTS
# =============================================================================
# Source.Python Imports
#   Entities
from _entities._collisions import CollisionHash
from _entities._collisions import CollisionManager
from _entities._collisions import CollisionMap
from _entities._collisions import CollisionRules
from _entities._collisions import CollisionSet
from _entities._collisions import collision_manager


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = [
    'CollisionHash',
    'CollisionHook',
    'CollisionManager',
    'CollisionMap',
    'CollisionRules',
    'CollisionSet',
    'collision_manager',
]


# =============================================================================
# >> INITIALIZATION
# =============================================================================
# Inject WeakAutoUnload into CollisionRules's hierarchy.
if not issubclass(CollisionRules, WeakAutoUnload):
    CollisionRules.__bases__ = (WeakAutoUnload,) + CollisionRules.__bases__


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
