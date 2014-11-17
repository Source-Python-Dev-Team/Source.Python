# ../commands/say/filter.py

"""Provides a decorator class for registering say filters."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
#   Commands
from commands.filter import _BaseFilter
from commands.say.manager import say_command_manager


# =============================================================================
# >> CLASSES
# =============================================================================
class SayFilter(_BaseFilter):

    """Class used to register a say filter."""

    # Store the class used to (un)register say filters
    _manager_class = say_command_manager
