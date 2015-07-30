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
           'NetMessage',
           )


# =============================================================================
# >> CLASSES
# =============================================================================
class NetFlow(IntEnum):
    FLOW_IN_OUT = FLOW_IN_OUT
    FLOW_INCOMING = FLOW_INCOMING
    FLOW_OUTGOING = FLOW_OUTGOING