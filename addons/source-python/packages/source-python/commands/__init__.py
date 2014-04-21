# ../commands/__init__.py

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
from _commands import Command
from _commands import CommandReturn
from loggers import _SPLogger


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
# Set all to an empty list
__all__ = [
    'Command',
    'CommandReturn'
]


# =============================================================================
# >> GLOBAL VARIABLES
# =============================================================================
# Get the sp.commands logger
CommandsLogger = _SPLogger.commands
