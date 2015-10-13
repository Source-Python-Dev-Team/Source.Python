# ../config/command.py

"""Provides server command functionality in configuration files."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
#   Translations
from translations.strings import TranslationStrings


# =============================================================================
# >> CLASSES
# =============================================================================
class _CommandManager(object):
    """Class used to store a command instance."""

    def __init__(self, name, description):
        """Called on instantiation."""
        # Is the given description a TranslationStrings instance?
        if isinstance(description, TranslationStrings):

            # Store the description as the proper language string
            description = description.get_string()

        # Store the base attributes for the command
        self._name = name
        self._description = description

    @property
    def name(self):
        """Return the command's name."""
        return self._name

    @property
    def description(self):
        """Return the command's description."""
        return self._description
