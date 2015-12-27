# ../cvars/__init__.py

"""Provides access to ConVar instances and the ability to create a ConVar."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
#   Cvars
from _cvars import _ConVar
from cvars.flags import ConVarFlags


# =============================================================================
# >> FORWARD IMPORTS
# =============================================================================
# Source.Python Imports
#   Cvars
from _cvars import cvar


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('ConVar',
           'cvar',
           )


# =============================================================================
# >> CLASSES
# =============================================================================
class ConVar(_ConVar):
    """ConVar wrapper to provide easy access to cvars."""

    def __init__(
            self, name, value='0', description='',
            flags=0, min_value=None, max_value=None):
        """Called when a server-var is initilized.

        If the ConVar already exists, all other parameters
        except ``name`` are inconsequential.

        :param str name: The name of the ConVar.
        :param str value: The initial value of the
            ConVar if it doesn't already exist.
        :param str description: The description of the ConVar.
        :param ConVarFlags flags: The initial flags of the
            ConVar if it doesn't already exist.
        :param float min_value: The minimum value allowed for the ConVar.
        :param float max_value: The maximum value allowed for the ConVar.
        """
        super().__init__(
            name, value, description, flags,
            min_value is not None, min_value or 0.0,
            max_value is not None, max_value or 0.0)

    def __getattr__(self, attr):
        """Retrieve the value of the given attribute."""
        # Get the flag
        flag = getattr(ConVarFlags, attr.upper(), None)

        # Is the attribute a flag?
        if flag is not None:

            # Return the value of the cvar's flag
            return self.is_flag_set(flag)

        # If not, raise an error
        raise AttributeError(
            '"ConVar" object has no attribute "{0}"'.format(attr))

    def __setattr__(self, attr, value):
        """Set the value of the given attribute."""
        # Get the flag
        flag = getattr(ConVarFlags, attr.upper(), None)

        # Is the attribute not a flag?
        if flag is None:

            # Set the attribute
            super().__setattr__(attr, value)

            # No need to go further
            return

        # Is the value "True"
        if value:

            # Add the flag
            self.add_flags(flag)

            # No need to go further
            return

        # Remove the flag
        self.remove_flags(flag)

    def make_public(self):
        """Set the notify flag and makes the cvar public."""
        self.add_flags(ConVarFlags.NOTIFY)
        cvar.call_global_change_callbacks(
            self, self.get_string(), self.get_float())

    def remove_public(self):
        """Remove the notify flag and makes the cvar no longer public."""
        self.remove_flags(ConVarFlags.NOTIFY)
        cvar.call_global_change_callbacks(
            self, self.get_string(), self.get_float())
