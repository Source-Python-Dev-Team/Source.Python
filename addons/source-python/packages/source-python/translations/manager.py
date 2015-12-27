# ../translations/manager.py

"""Contains a class that stores languages that can be used for translations."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Site-Package Imports
#   Configobj
from configobj import ConfigObj

# Source.Python Imports
#   Paths
from paths import SP_DATA_PATH


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('_LanguageManager',
           'language_manager',
           )


# =============================================================================
# >> CLASSES
# =============================================================================
class _LanguageManager(ConfigObj):
    """Dictionary class used to store languages and their shortnames."""

    # en should always be used as the "fallback",
    # since most plugins will have English values
    fallback = 'en'

    # Set the default language to en by default
    default = 'en'

    def _register_default_language(self, language):
        """Register the default language for the current server."""
        # Get the shortname for the given language
        language = self.get_language(language)

        # Was a language returned?
        if language is not None:

            # Set the default language
            self.default = language

    def get_language(self, language):
        """Return the shortname for the given language."""
        # Is the given language an item in the dictionary?
        if language in self:

            # Return the language's shortname
            return self[language]

        # Is the given language a known shortname?
        if language in self.values():

            # Return the shortname
            return language

        # If the value is not found, return None
        return None

# The singleton object of the :class:`_LanguageManager` class
language_manager = _LanguageManager(SP_DATA_PATH / 'languages.ini')
