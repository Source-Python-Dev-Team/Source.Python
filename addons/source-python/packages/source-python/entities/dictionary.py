# ../entities/dictionary.py

"""Provides helper class to store entity instances."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
#   Core
from core import AutoUnload
#   Entities
from entities.entity import BaseEntity
#   Listeners
from listeners import on_entity_deleted_listener_manager


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('EntityDictionary',
           )


# ============================================================================
# >> CLASSES
# ============================================================================
class EntityDictionary(AutoUnload, dict):

    """Helper class used to store entity instances."""

    def __init__(self, entity_class=BaseEntity):
        """Initialize the dictionary."""
        # Store the given entity class...
        self._entity_class = entity_class

        # Register our OnEntityDeleted listener...
        on_entity_deleted_listener_manager.register_listener(
            self._on_entity_deleted)

        # Initialize the dictionary...
        super(EntityDictionary, self).__init__()

    def __missing__(self, index):
        """Add and return the entity instance for the given index."""
        instance = self[index] = self._entity_class(index)
        return instance

    def __delitem__(self, index):
        """Remove the given index from the dictionary."""
        # Is the given index not in the dictionary?
        if index not in self:

            # If so, no need to go further...
            return

        # Remove the given index from the dictionary...
        super(EntityDictionary, self).__delitem__(index)

    def _on_entity_deleted(self, index, ptr):
        """OnEntityDeleted listener callback."""
        del self[index]

    def _unload_instance(self):
        """Unregister our OnEntityDeleted listener."""
        on_entity_deleted_listener_manager.unregister_listener(
            self._on_entity_deleted)
