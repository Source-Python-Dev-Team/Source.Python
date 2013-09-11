# ../commands/say/filter.py

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
from public import public
#   Commands
from commands.filter import _BaseFilter
from commands.say.manager import SayCommandManager


# =============================================================================
# >> CLASSES
# =============================================================================
@public
class SayFilter(_BaseFilter):
    '''Class used to register a say filter'''

    # Store the class used to (un)register say filters
    _manager_class = SayCommandManager
