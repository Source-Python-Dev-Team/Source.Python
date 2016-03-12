# ../auth/base.py

"""Provides the base class for every backend."""

# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('PermissionSource',
    )


# =============================================================================
# >> CLASSES
# =============================================================================
class PermissionSource(object):
    options = {}

    def load(self):
        """Called when the backend is loaded."""
        pass

    def unload(self):
        """Called when the backend is unloaded."""
        pass

    def permission_added(self, store, permission):
        """Called when a permission has been added."""
        pass

    def permission_removed(self, store, permission):
        """Called when a permission has been removed."""
        pass

    def parent_added(self, store, permission):
        """Called when a parent has been added."""
        pass

    def parent_removed(self, store, permission):
        """Called when a parent has been removed."""
        pass

    @property
    def name(self):
        """Return the name of the backend."""
        raise NotImplementedError('Every backend must define a name.')
