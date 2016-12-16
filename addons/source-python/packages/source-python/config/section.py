# ../config/section.py

"""Provides a way to create sections within configuration files."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
#   Translations
from translations.strings import TranslationStrings


# =============================================================================
# >> CLASSES
# =============================================================================
class _SectionManager(object):
    """Class used to store a section instance."""

    def __init__(self, name, separator):
        """Called on instantiation.

        :param str/TranslationStrings name:
            Name of the section to add.
        :param str separator:
            A single separator character to use to separate the section.
        """
        # Is the given name a TranslationStrings instance?
        if isinstance(name, TranslationStrings):

            # Store the name as the proper language string
            name = name.get_string()

        # Store the base attributes for the section
        self._name = name
        self._separator = separator

    @property
    def name(self):
        """Return the section's name.

        :rtype: str
        """
        return self._name

    @property
    def separator(self):
        """Return the separator character to use for the section.

        :rtype: str
        """
        return self._separator
