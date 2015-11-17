# ../listeners/tick/__init__.py

"""Provides tick listener based functionality."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
#   Listeners
from listeners import listeners_logger
from listeners.tick.delays import Delay
from listeners.tick.delays import tick_delays
from listeners.tick.repeat import TickRepeat
from listeners.tick.repeat import TickRepeatStatus


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('_TickDelays',
           'Delay',
           'TickRepeat',
           'TickRepeatStatus',
           'tick_delays',
           )


# =============================================================================
# >> GLOBAL VARIABLES
# =============================================================================
# Get the sp.listeners.tick logger
listeners_tick_logger = listeners_logger.tick
