# ../commands/say/command.py

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
from public import public
#   Commands
from commands.command import _BaseCommand
from commands.say.manager import SayCommandManager


# =============================================================================
# >> CLASSES
# =============================================================================
@public
class SayCommand(_BaseCommand):
    '''Decorator class used to register a say command'''

    # Store the class used to (un)register say commands
    _manager_class = SayCommandManager
