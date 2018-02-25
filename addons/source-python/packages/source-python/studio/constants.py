# ../studio/constants.py

"""Provides model based functionality."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Python Imports
#   Enum
from enum import IntFlag


# =============================================================================
# >> FORWARD IMPORTS
# =============================================================================
# Source.Python Imports
#   Studio
from _studio._constants import INVALID_ATTACHMENT_INDEX
from _studio._constants import INVALID_BONE_INDEX
from _studio._constants import ModelType
from _studio._constants import STUDIO_NONE
from _studio._constants import STUDIO_RENDER
from _studio._constants import STUDIO_VIEWXFORMATTACHMENTS
from _studio._constants import STUDIO_DRAWTRANSLUCENTSUBMODELS
from _studio._constants import STUDIO_TWOPASS
from _studio._constants import STUDIO_STATIC_LIGHTING
from _studio._constants import STUDIO_WIREFRAME
from _studio._constants import STUDIO_ITEM_BLINK
from _studio._constants import STUDIO_NOSHADOWS
from _studio._constants import STUDIO_WIREFRAME_VCOLLIDE
from _studio._constants import STUDIO_TRANSPARENCY
from _studio._constants import STUDIO_SHADOWDEPTHTEXTURE


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('INVALID_ATTACHMENT_INDEX',
           'INVALID_BONE_INDEX',
           'ModelType',
           'StudioFlags',
           )


# =============================================================================
# >> ENUMERATORS
# =============================================================================
class StudioFlags(IntFlag):
    """Studio flags wrapper enumerator."""

    NONE = STUDIO_NONE
    RENDER = STUDIO_RENDER
    VIEWXFORMATTACHMENTS = STUDIO_VIEWXFORMATTACHMENTS
    DRAWTRANSLUCENTSUBMODELS = STUDIO_DRAWTRANSLUCENTSUBMODELS
    TWOPASS = STUDIO_TWOPASS
    STATIC_LIGHTING = STUDIO_STATIC_LIGHTING
    WIREFRAME = STUDIO_WIREFRAME
    ITEM_BLINK = STUDIO_ITEM_BLINK
    NOSHADOWS = STUDIO_NOSHADOWS
    WIREFRAME_VCOLLIDE = STUDIO_WIREFRAME_VCOLLIDE
    TRANSPARENCY = STUDIO_TRANSPARENCY
    SHADOWDEPTHTEXTURE = STUDIO_SHADOWDEPTHTEXTURE
