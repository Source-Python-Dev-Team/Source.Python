# ../entities/transmit.py

"""Provides entity transmission filtering."""


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
    'reset_hidden_state',
)


# =============================================================================
# >> FUNCTIONS
# =============================================================================
def reset_hidden_state():
    """Reset all entities' hidden/shown state."""
    transmit_manager.reset_all()

