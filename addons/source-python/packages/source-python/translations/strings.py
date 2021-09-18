# ../translations/strings.py

"""Provides translation functionality."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Python Imports
#   Binascii
from binascii import unhexlify
#   Codecs
from codecs import unicode_escape_decode
#   Re
from re import compile as re_compile
from re import VERBOSE

# Site-Package Imports
#   Configobj
from configobj import Section

# Source.Python Imports
#   Core
from core import GameConfigObj
#   Paths
from paths import TRANSLATION_PATH
from paths import GAME_PATH
#   Translations
from translations.manager import language_manager


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('LangStrings',
           'TranslationStrings',
           )


# =============================================================================
# >> GLOBAL VARIABLES
# =============================================================================
# Get an re.compile instance to correct all double escaped strings
_double_escaped_pattern = re_compile(
    r"""(\\(?:(?P<octal>[0-7]{1,3})|x(?P<hexadecimal>[0-9|a-f|A-F]{2})|
    (?P<notation>a|b|e|f|n|r|s|t|v)))""", VERBOSE)


# =============================================================================
# >> CLASSES
# =============================================================================
class LangStrings(dict):
    """Dictionary class used to store all strings for a plugin."""

    def __init__(self, infile, encoding='utf_8'):
        """Add all strings and fix double escaped strings."""
        # Initialize the dictionary
        super().__init__()

        # Get the path to the given file
        self._mainfile = TRANSLATION_PATH / infile + '.ini'
        self._encoding = encoding

        # Does the file exist?
        if not self._mainfile.isfile():

            # Raise an error
            raise FileNotFoundError(
                'No file found at {0}'.format(self._mainfile))

        # Get the path to the server specific file
        self._serverfile = self._mainfile.parent / '{0}_server.ini'.format(
            self._mainfile.namebase)

        # Get the strings from the main file
        main_strings = GameConfigObj(self._mainfile, encoding=encoding)

        # Does the server specific file exist?
        if not self._serverfile.isfile() and not infile.startswith('_core/'):

            # Create the server specific file
            self._create_server_file()

        # Otherwise
        else:

            # Get any strings from the server specific file
            server_strings = GameConfigObj(self._serverfile, encoding=encoding)

            # Merge the two ConfigObj instances together
            main_strings.merge(server_strings)

        # Loop through all strings
        for key in main_strings:

            # Is the current string not a Section?
            if not isinstance(main_strings[key], Section):

                # No need to go further
                continue

            # Get a TranslationStrings instance for the current string
            translation_strings = TranslationStrings()

            # Loop through all languages for the current string
            for lang in main_strings[key]:

                # Get the shortname of the current language
                language = language_manager.get_language(lang)

                # Was the language found?
                if language is None:

                    # Do not add this translation
                    # Possibly raise an error silently here
                    continue

                # Get the language's string and fix any escaped strings
                translation_strings[
                    language] = self._replace_escaped_sequences(
                    main_strings[key][lang])

            # Add the TranslationStrings instance for the current string
            self[key] = translation_strings

        # Is there any default language specified into the main file?
        if 'DEFAULT_LANGUAGE' in main_strings:

            # Get the default language
            default_language = main_strings['DEFAULT_LANGUAGE']

            # Make sure it is not a Section
            if not isinstance(default_language, Section):

                # Get the given language code
                language_code = language_manager.get_language(default_language)

                # Is the language valid?
                if language_code is not None:

                    # Set the default language
                    self.default_language = language_code

                # Delete the key from the main file as we are done with it
                del main_strings['DEFAULT_LANGUAGE']

    def __setattr__(self, attribute, value):
        """Register the default language."""
        # Is the given attribute the default language?
        if attribute == 'default_language':

            # Get the given language code
            language_code = language_manager.get_language(value)

            # Is the given language code valid?
            if language_code is not None:

                # Loop through all strings
                for key in self:

                    # Set the default language to use for that string
                    self[key]._default_language = language_code

                # Override the given value
                value = language_code

        # Set the attribute
        super().__setattr__(attribute, value)

    def _create_server_file(self):
        """Create a server specific langstrings file."""
        # Get the server specific file's ConfigObj instance
        server_file = GameConfigObj(self._serverfile, encoding=self._encoding)

        # Set the initial comments to explain what the file is for
        server_file.initial_comment = _translation_strings[
            'Initial Comment'].get_string(
            language_manager.default,
            filename=self._mainfile.replace(GAME_PATH, '')).splitlines()

        # Write the server specific file
        server_file.write()

    @staticmethod
    def _replace_escaped_sequences(given_string):
        """Fix all double escaped strings."""
        # Loop through all matches
        for escaped_match in set(
                _double_escaped_pattern.finditer(given_string)):

            # Get the match as a string
            matching_string = escaped_match.group()

            # Get a dictionnary of all groups
            matching_groups = escaped_match.groupdict()

            # Are we matching any octal sequences?
            if matching_groups['octal']:

                # Replace it
                given_string = given_string.replace(
                    matching_string, chr(int(matching_groups['octal'])))

            # Otherwise, are we matching any hexadecimal sequences?
            elif matching_groups['hexadecimal']:

                # Replace it
                given_string = given_string.replace(
                    matching_string, str(unhexlify(
                        matching_groups['hexadecimal']), encoding='ascii'))

            # Otherwise, that means we are matching a notation
            else:

                # Replace it
                given_string = given_string.replace(
                    matching_string, unicode_escape_decode(matching_string)[0])

        # Return the replaced string
        return given_string

    def get_strings(self, key, **tokens):
        """Return a TranslationStrings object with updated tokens."""
        strings = self[key]
        strings.tokens.update(tokens)
        return strings


class TranslationStrings(dict):
    """Stores and get language strings for a particular string."""

    def __init__(self):
        """Store an empty dictionary as the tokens."""
        super().__init__()
        self.tokens = {}

    def get_string(self, language=None, **tokens):
        """Return the language string for the given language/tokens."""
        # Was no language passed?
        if language is None:

            # Set the language to the server's default
            language = language_manager.default

        # Get the language shortname to be used
        language = self.get_language(language)

        # Was a valid language found?
        if language is None:

            # Return an empty string
            # Possibly raise an error silently here
            return ''

        # Expose all TranslationStrings instances in self.tokens
        exposed_tokens = {}

        # Pass additional kwargs - these will be used to format the string
        self._update_exposed_tokens(
            exposed_tokens, language, self.tokens, **tokens)

        # Don't pass any additional kwargs, each token should either
        # be trivial or rely on itself (self.tokens)
        self._update_exposed_tokens(exposed_tokens, language, tokens)

        # Return the formatted message
        return self[language].format(**exposed_tokens)

    @staticmethod
    def _update_exposed_tokens(exposed_tokens, language, tokens, **kwargs):
        for token_name, token in tokens.items():
            if isinstance(token, TranslationStrings):
                token = token.get_string(language, **kwargs)

            exposed_tokens[token_name] = token

    def get_language(self, language):
        """Return the language to be used."""
        # Get the given language's shortname
        language = language_manager.get_language(language)

        # Was a language found?
        if language is not None and language in self:

            # Return the language
            return language

        # Is the server's default language in the dictionary?
        if language_manager.default in self:

            # Return the server's default language
            return language_manager.default

        # Is there any default language defined?
        if hasattr(self, '_default_language'):

            # Is the default language available for that string?
            if self._default_language in self:

                # Return the default language to use
                return self._default_language

        # Is the server's fallback language in the dictionary?
        if language_manager.fallback in self:

            # Return the server's fallback language
            return language_manager.fallback

        # Return None as the language, as no language has been found
        return None

    def tokenized(self, **tokens):
        """Create a new TranslationStrings instance and store tokens in it.

        :param dict tokens: Tokens to store in the instance.
        :return: New TranslationStrings instance with tokens stored in it.
        :rtype: TranslationStrings
        """
        result = TranslationStrings()
        result.tokens.update(tokens)

        result.update(self)

        return result

# Get the translations language strings
_translation_strings = LangStrings('_core/translations_strings')
