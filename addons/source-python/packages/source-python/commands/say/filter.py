# ../commands/say/filter.py

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
#   Commands
from commands.filter import _BaseFilter
from commands.say.manager import SayCommandManager


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
# Add all the global variables to __all__
__all__ = [
    'SayFilter',
]


# =============================================================================
# >> CLASSES
# =============================================================================
class SayFilter(_BaseFilter):
    '''Class used to register a say filter'''

    # Store the class used to (un)register say filters
    _manager_class = SayCommandManager
