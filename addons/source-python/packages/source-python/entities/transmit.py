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


# ============================================================================
# >> FORWARD IMPORTS
# ============================================================================
# Source.Python
#   Entities
from _entities._transmit import TransmitManager


# ============================================================================
# >> ALL DECLARATION
# ============================================================================
__all__ = (
    'TransmitManager',
    'reset_hidden_state',
)


# ============================================================================
# >> CLASSES
# ============================================================================


# =============================================================================
# >> FUNCTIONS
# =============================================================================
def reset_hidden_state():
    """Reset all entities' hidden/shown state."""
    TransmitManager.reset_all()

