# ../auth/base.py

"""Provides the base class for every backend."""

# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('Backend',
    )


# =============================================================================
# >> CLASSES
# =============================================================================
class Backend(object):
    options = {}

    def load(self):
        """Called when the backend is loaded."""
        pass

    def unload(self):
        """Called when the backend is unloaded."""
        pass

    def permission_added(self, node, permission, server_id):
        """Called when a permission has been added."""
        pass

    def permission_removed(self, node, permission, server_id):
        """Called when a permission has been removed."""
        pass

    def parent_added(self, node, parent_name):
        """Called when a parent has been added."""
        pass

    def parent_removed(self, node, parent_name):
        """Called when a parent has been removed."""
        pass

    @property
    def name(self):
        """Return the name of the backend."""
        raise NotImplementedError('Every backend must define a name.')
