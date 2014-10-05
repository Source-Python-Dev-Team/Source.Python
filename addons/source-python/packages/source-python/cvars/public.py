# ../cvars/public.py

"""Provides a class that automatically sets the notify flag on load/unload."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
#   Core
from core import AutoUnload
#   Cvars
from cvars import ConVar


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('PublicConVar',
           )


# =============================================================================
# >> CLASSES
# =============================================================================
class PublicConVar(AutoUnload, ConVar):

    """Class used to set the notify flag on load/unload."""

    def __init__(
            self, name, value='0', flags=0,
            description='', min_value=None, max_value=None):
        """Set the cvar's value and set the notify flag."""
        super(PublicConVar, self).__init__(
            name, value, flags, description, min_value, max_value)
        self.set_string(value)
        self.make_public()

    def _unload_instance(self):
        """Remove the notify flag."""
        self.remove_public()
