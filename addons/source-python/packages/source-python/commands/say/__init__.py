# ../commands/say/__init__.py

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
from commands.say.command import SayCommand
from commands.say.filter import SayFilter
from commands.say.manager import say_command_manager


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('SayCommand',
           'SayCommandDispatcher',
           'SayCommandGenerator',
           'SayFilter',
           'get_say_command',
           'register_say_filter',
           'say_command_manager',
           'unregister_say_filter',
           )
