# ../translations/manager.py

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
__all__ = ('LanguageManager',
           )


# =============================================================================
# >> CLASSES
# =============================================================================
class _LanguageManager(ConfigObj):
    """Dictionary class used to store languages and their shortnames"""

    # en should always be used as the "fallback",
    # since most plugins will have English values
    fallback = 'en'

    # Set the default language to en by default
    default = 'en'

    def _register_default_language(self, language):
        """Registers the default language for the current server"""

        # Get the shortname for the given language
        language = self.get_language(language)

        # Was a language returned?
        if language is not None:

            # Set the default language
            self.default = language

    def get_language(self, language):
        """Returns the shortname for the given language"""

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

# Get the _LanguageManager instance for the server's languages file
LanguageManager = _LanguageManager(SP_DATA_PATH.joinpath('languages.ini'))
