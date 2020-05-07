# ../entities/dictionary.py

"""Provides helper class to store entity instances."""

# ============================================================================
# >> IMPORTS
# ============================================================================
# Python Imports
#   ContextLib
from contextlib import suppress

# Source.Python Imports
#   Core
from core import AutoUnload
#   Entities
from entities.entity import Entity
from entities.helpers import index_from_inthandle
#   Listeners
from listeners import on_entity_deleted_listener_manager


# ============================================================================
# >> ALL DECLARATION
# ============================================================================
__all__ = ('EntityDictionary',
           )


# ============================================================================
# >> CLASSES
# ============================================================================
class EntityDictionary(AutoUnload, dict):
    """Helper class used to store entity instances."""

    def __init__(self, factory=Entity, *args, **kwargs):
        """Initialize the dictionary."""
        # Store the given entity class...
        self._factory = factory

        # Store given arguments/keywords
        self._args = args
        self._kwargs = kwargs

        # Register our OnEntityDeleted listener...
        on_entity_deleted_listener_manager.register_listener(
            self._on_entity_deleted)

        # Initialize the dictionary...
        super().__init__()

    def __missing__(self, index):
        """Add and return the entity instance for the given index."""
        instance = self._factory(index, *self._args, **self._kwargs)

        # Only cache entities that are not marked for deletion.
        # This is required, because if someone request an entity instance
        # after we invalidated our cache but before the engine processed
        # the deletion we would now have an invalid instance in the cache.
        if not instance.is_marked_for_deletion():
            self[index] = instance

        return instance

    def __delitem__(self, index):
        """Remove the given index from the dictionary."""
        # Remove the given index from the dictionary...
        with suppress(KeyError):
            super().__delitem__(index)

    def from_inthandle(self, inthandle):
        """Get an entity instance from an inthandle.
        
        :param int inthandle:
            The inthandle.
        :rtype: Entity
        """
        return self[index_from_inthandle(inthandle)]

    def on_automatically_removed(self, index):
        """Called when an index is automatically removed."""

    def _on_entity_deleted(self, base_entity):
        """OnEntityDeleted listener callback."""
        try:
            # Get the index of the entity...
            index = base_entity.index
        except ValueError:
            return

        if index in self:
            # Call the deletion callback for the index...
            self.on_automatically_removed(index)

            # Remove the index from the dictionary...
            super().__delitem__(index)

    def _unload_instance(self):
        """Unregister our OnEntityDeleted listener."""
        on_entity_deleted_listener_manager.unregister_listener(
            self._on_entity_deleted)
