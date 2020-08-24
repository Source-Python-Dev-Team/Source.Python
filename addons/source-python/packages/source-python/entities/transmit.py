# ../entities/transmit.py

"""Provides entity transmission filtering."""

# ============================================================================
# >> IMPORTS
# ============================================================================
# Source.Python
#   Core
from core import AutoUnload
#   Entities
from entities.entity import Entity
from entities.hooks import EntityCondition
#   Filters
from filters.entities import EntityIter
#   Listeners
from listeners import on_entity_created_listener_manager
from listeners import on_entity_deleted_listener_manager


# ============================================================================
# >> FORWARD IMPORTS
# ============================================================================
# Source.Python
#   Entities
from _entities._transmit import transmit_manager


# ============================================================================
# >> ALL DECLARATION
# ============================================================================
__all__ = (
    'transmit_manager',
)


# ============================================================================
# >> CLASSES
# ============================================================================

