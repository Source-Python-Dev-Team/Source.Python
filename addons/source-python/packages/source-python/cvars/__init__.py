# ../cvars/__init__.py

"""Provides access to ConVar instances and the ability to create a ConVar."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
#   Core
from core import AutoUnload
#   Cvars
from _cvars import ConVar
from _cvars import _Cvar
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
           'ConVarChanged',
           'cvar',
           )


# =============================================================================
# >> CLASSES
# =============================================================================
class ConVarChanged(AutoUnload):
    """ConVarChanged decorator class."""

    def __init__(self, *convars):
        """Store the convars."""
        self._convars = ()
        self.callback = None

        # Validate convars
        if not convars:
            raise ValueError('At least one convar is required.')

        _convars = []
        for convar in convars:
            if isinstance(convar, str):
                convar_name = convar
            elif isinstance(convar, ConVar):
                convar_name = convar.name
            else:
                raise ValueError('Given convar is not ConVar or ConVar name.')

            convar = cvar.find_var(convar_name)
            if convar is None:
                raise ValueError(f'"{convar_name}" is not a valid ConVar.')

            _convars.append(convar)

        self._convars = tuple(_convars)

    def __call__(self, callback):
        """Store the callback and add it to convars."""
        # Is the callback callable?
        if not callable(callback):
            raise ValueError('Given callback is not callable.')

        # Store the callback
        self.callback = callback

        # Loop through all convars
        for convar in self._convars:

            # Add the callback
            convar.add_changed_callback(self.callback)

        # Return the callback
        return self.callback

    def _unload_instance(self):
        """Remove the callback from convars."""
        # Was no callback registered?
        if self.callback is None:
            return

        # Loop through all convars
        for convar in self._convars:

            # Remove the callback
            convar.remove_changed_callback(self.callback)
