# ../commands/say/manager.py

"""Provides a manager class used to register say commands and filters."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
#   Commands
from commands.player import _PlayerCommandManager
from commands.say import get_say_command
from commands.say import register_say_filter
from commands.say import unregister_say_filter


# =============================================================================
# >> CLASSES
# =============================================================================
class _SayCommandManager(_PlayerCommandManager):
    """Manager class used to register say commands and say filters."""

    # Store the base functions
    _get_command = staticmethod(get_say_command)
    register_filter = staticmethod(register_say_filter)
    unregister_filter = staticmethod(unregister_say_filter)

#: The singleton object of the :class:`_SayCommandManager` class
say_command_manager = _SayCommandManager()
