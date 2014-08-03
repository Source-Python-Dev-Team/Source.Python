# ../config/section.py

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
#   Translations
from translations.strings import TranslationStrings


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ()


# =============================================================================
# >> CLASSES
# =============================================================================
class _SectionManager(object):
    '''Class used to store a section instance'''

    def __init__(self, name, separator):
        '''Called on instantiation'''

        # Is the given name a TranslationStrings instance?
        if isinstance(name, TranslationStrings):

            # Store the name as the proper language string
            name = name.get_string()

        # Store the base attributes for the section
        self._name = name
        self._separator = separator

    @property
    def name(self):
        '''Returns the section's name'''
        return self._name

    @property
    def separator(self):
        '''Returns the separator to use for the section'''
        return self._separator
