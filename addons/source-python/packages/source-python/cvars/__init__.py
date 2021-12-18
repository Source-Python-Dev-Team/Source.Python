# ../cvars/__init__.py

"""Provides access to ConVar instances and the ability to create a ConVar."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
#   Cvars
from cvars.flags import ConVarFlags


# =============================================================================
# >> FORWARD IMPORTS
# =============================================================================
# Source.Python Imports
#   Cvars
from _cvars import cvar
from _cvars import ConVar
from _cvars import SP_CVAR_DLL_IDENTIFIER
from _cvars import _Cvar


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('ConVar',
           'SP_CVAR_DLL_IDENTIFIER',
           'cvar',
           )
