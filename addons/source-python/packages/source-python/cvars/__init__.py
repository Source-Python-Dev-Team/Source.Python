# ../cvars/__init__.py

"""Provides access to ConVar instances and the ability to create a ConVar."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
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
           'cvar',
           )
