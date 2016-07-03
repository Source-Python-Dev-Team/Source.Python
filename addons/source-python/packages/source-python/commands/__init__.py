# ../commands/__init__.py

"""Provides server, client, and say command functionality."""

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
#   Commands
from _commands import Command
from _commands import CommandReturn
from _commands import ConCommand
from _commands import ConCommandBase


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('Command',
           'CommandReturn',
           'ConCommand',
           'ConCommandBase',
           )


# =============================================================================
# >> GLOBAL VARIABLES
# =============================================================================
# Get the sp.commands logger
commands_logger = _sp_logger.commands
