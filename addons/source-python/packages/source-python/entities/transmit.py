# ../entities/transmit.py

"""Provides entity transmission functionality."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
#   Core
from core import AutoUnload
from core import WeakAutoUnload


# =============================================================================
# >> FORWARD IMPORTS
# =============================================================================
# Source.Python Imports
#   Entities
from _entities._transmit import BaseTransmitRules
from _entities._transmit import TransmitHash
from _entities._transmit import TransmitManager
from _entities._transmit import TransmitMap
from _entities._transmit import TransmitMode
from _entities._transmit import TransmitSet
from _entities._transmit import TransmitStates
from _entities._transmit import transmit_manager


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('BaseTransmitRules',
           'TransmitHash',
           'TransmitHook',
           'TransmitManager',
           'TransmitMap',
           'TransmitMode',
           'TransmitSet',
           'TransmitStates',
           'transmit_manager',
           )


# =============================================================================
# >> INITIALIZATION
# =============================================================================
# Inject WeakAutoUnload into BaseTransmitRules's hierarchy.
if not issubclass(BaseTransmitRules, WeakAutoUnload):
    BaseTransmitRules.__bases__ = (
        WeakAutoUnload,) + BaseTransmitRules.__bases__


# =============================================================================
# >> CLASSES
# =============================================================================
class TransmitHook(AutoUnload):
    """Decorator used to create transmit hooks that auto unload."""

    def __init__(self, callback):
        """Registers the transmit hook."""
        self.callback = callback
        transmit_manager.register_hook(callback)

    def _unload_instance(self):
        """Unregisters the transmit hook."""
        transmit_manager.unregister_hook(self.callback)
