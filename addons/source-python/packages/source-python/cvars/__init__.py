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
from _cvars import SP_CVAR_DLL_IDENTIFIER
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
           'SP_CVAR_DLL_IDENTIFIER',
           'cvar',
           )


# =============================================================================
# >> INITIALIZATION
# =============================================================================
# Inject AutoUnload into ConVar's inheritance tree so that custom instances
#   are kept alive until the plugin creating them remains loaded.
ConVar.__bases__ = (AutoUnload,) + ConVar.__bases__
