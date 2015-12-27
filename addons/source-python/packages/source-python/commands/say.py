# ../commands/say.py

"""Provides say command and say filter functionality."""

# =============================================================================
# >> FORWARD IMPORTS
# =============================================================================
# Source.Python Imports
#   Commands
from _commands._say import SayCommandDispatcher
from _commands._say import SayCommandGenerator
from _commands._say import get_say_command
from _commands._say import register_say_filter
from _commands._say import unregister_say_filter
from commands.command import _BaseCommand
from commands.filter import _BaseFilter
from commands.player import _PlayerCommandManager


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('SayCommand',
           'SayCommandDispatcher',
           'SayCommandGenerator',
           'SayFilter',
           '_SayCommandManager',
           'get_say_command',
           'register_say_filter',
           'say_command_manager',
           'unregister_say_filter',
           )


# =============================================================================
# >> CLASSES
# =============================================================================
class _SayCommandManager(_PlayerCommandManager):
    """Manager class used to register say commands and say filters."""

    # Store the base functions
    _get_command = staticmethod(get_say_command)
    register_filter = staticmethod(register_say_filter)
    unregister_filter = staticmethod(unregister_say_filter)

# The singleton object of the :class:`_SayCommandManager` class
say_command_manager = _SayCommandManager()


class SayCommand(_BaseCommand):
    """Decorator class used to register a say command."""

    # Store the class used to (un)register say commands
    _manager_class = say_command_manager


class SayFilter(_BaseFilter):
    """Class used to register a say filter."""

    # Store the class used to (un)register say filters
    _manager_class = say_command_manager
