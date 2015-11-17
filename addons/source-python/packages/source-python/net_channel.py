# ../net_channel.py

"""Provides access to the net channel interfaces."""

# ============================================================================
# >> IMPORTS
# ============================================================================
# Python
from enum import IntEnum


# =============================================================================
# >> FORWARD IMPORTS
# =============================================================================
# Source.Python Imports
#   Effects
from _net_channel import FLOW_IN_OUT
from _net_channel import FLOW_INCOMING
from _net_channel import FLOW_OUTGOING
from _net_channel import NetChannel
from _net_channel import NetChannelHandler
from _net_channel import NetChannelInfo
from _net_channel import NetMessage


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('NetChannel',
           'NetChannelHandler',
           'NetChannelInfo',
           'NetFlow',
           'NetMessage',
           )


# =============================================================================
# >> CLASSES
# =============================================================================
class NetFlow(IntEnum):
    """Network Channel Enum class."""

    IN_OUT = FLOW_IN_OUT
    INCOMING = FLOW_INCOMING
    OUTGOING = FLOW_OUTGOING
