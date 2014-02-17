# ../commands/say/manager.py

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
from command_c import get_say_command
from command_c import register_say_filter
from command_c import unregister_say_filter
#   Commands
from commands.player import _PlayerCommandManager


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
# Add all the global variables to __all__
__all__ = [
    'SayCommandManager',
]


# =============================================================================
# >> CLASSES
# =============================================================================
class _SayCommandManager(_PlayerCommandManager):
    '''Manager class used to register say commands and say filters'''

    # Store the base functions
    _get_command = staticmethod(get_say_command)
    register_filter = staticmethod(register_say_filter)
    unregister_filter = staticmethod(unregister_say_filter)

# Get the _SayCommandManager instance
SayCommandManager = _SayCommandManager()
