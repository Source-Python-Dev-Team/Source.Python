# ../auth/base.py

"""TODO"""

# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('PermissionSource',)


# =============================================================================
# >> CLASSES
# =============================================================================
class PermissionSource(object):
    options = {}

    def load(self):
        pass

    def unload(self):
        pass
        
    @property
    def name(self):
        raise NotImplementedError('Every backend must define a name.')
