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
from _studio import ModelAttachment
from _studio import ModelBone
from _studio import BoneController
from _studio import SequenceEvent
from _studio import Hierarchy
from _studio import Hitbox
from _studio import HitboxSet
from _studio import ModelHeader
from _studio import Movement
from _studio import Sequence


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('AnimSections',
           'Animation',
           'BoneController',
           'Hierarchy',
           'Hitbox',
           'HitboxSet',
           'ModelAttachment',
           'ModelBone',
           'ModelHeader',
           'Movement',
           'Sequence',
           'SequenceEvent',
           )


# =============================================================================
# >> GLOBAL VARIABLES
# =============================================================================
# Get the sp.studio logger
studio_logger = _sp_logger.studio
