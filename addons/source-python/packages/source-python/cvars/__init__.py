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
