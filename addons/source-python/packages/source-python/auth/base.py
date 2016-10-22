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

    def group_added(self, node, group_name):
        """Called when a group has been added."""
        pass

    def group_removed(self, node, group_name):
        """Called when a group has been removed."""
        pass

    def sync(self):
        """Synchronize the auth_manager with the backend."""
        raise NotImplementedError('Backend must define a sync method.')

    @property
    def name(self):
        """Return the name of the backend."""
        raise NotImplementedError('Every backend must define a name.')
