# ../studio/__init__.py

"""Provides model based functionality."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
#   Loggers
from loggers import _sp_logger


# =============================================================================
# >> FORWARD IMPORTS
# =============================================================================
# Source.Python Imports
#   Studio
from _studio import Animation
from _studio import AnimSections
from _studio import Attachment
from _studio import Bone
from _studio import BoneController
from _studio import Event
from _studio import Hierarchy
from _studio import Hitbox
from _studio import HitboxSet
from _studio import ModelHeader
from _studio import Movement
from _studio import Sequence


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('studio_logger',
           'Animation',
           'AnimSections',
           'Attachment',
           'Bone',
           'BoneController',
           'Event',
           'Hierarchy',
           'Hitbox',
           'HitboxSet',
           'ModelHeader',
           'Movement',
           'Sequence',
           )


# =============================================================================
# >> GLOBAL VARIABLES
# =============================================================================
# Get the sp.listeners logger
studio_logger = _sp_logger.studio
