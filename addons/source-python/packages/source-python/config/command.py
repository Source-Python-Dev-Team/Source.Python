# ../config/command.py

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
#   Translations
from translations.strings import TranslationStrings


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
# Set all to an empty list
__all__ = []


# =============================================================================
# >> CLASSES
# =============================================================================
class _CommandManager(object):
    '''Class used to store a command instance'''

    def __init__(self, name, description):
        '''Called on instantiation'''

        # Is the given description a TranslationStrings instance?
        if isinstance(description, TranslationStrings):

            # Store the description as the proper language string
            description = description.get_string()

        # Store the base attributes for the command
        self._name = name
        self._description = description

    @property
    def name(self):
        '''Returns the command's name'''
        return self._name

    @property
    def description(self):
        '''Returns the command's description'''
        return self._description
