# ../translations/strings.py

# =============================================================================
# >> IMPORTS
# =============================================================================
# Python Imports
#   Binascii
from binascii import unhexlify
#   Codecs
from codecs import unicode_escape_decode
#   String
from string import Template
#   Re
from re import compile as re_compile
from re import VERBOSE

# Site Package Imports
#   Configobj
from configobj import ConfigObj
from configobj import Section
#   Path
from path import Path

# Source.Python Imports
from paths import TRANSLATION_PATH
from paths import GAME_PATH
#   Translations
from translations.manager import LanguageManager


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
# Add all the global variables to __all__
__all__ = [
    'LangStrings',
    'TranslationStrings',
]


# =============================================================================
# >> GLOBAL VARIABLES
# =============================================================================
# Get an re.compile instance to correct all double escaped strings
_double_escaped_pattern = re_compile(
    r'''(\\(?:(?P<octal>[0-7]{1,3})|x(?P<hexadecimal>[0-9|a-f|A-F]{2})|
    (?P<notation>a|b|e|f|n|r|s|t|v)))''', VERBOSE)


# =============================================================================
# >> CLASSES
# =============================================================================
class LangStrings(dict):
    '''Dictionary class used to store all strings for a plugin'''

    def __init__(self, infile, encoding='utf_8'):
        '''Adds all strings as TranslationStrings
            instances and fixes double escaped strings'''

        # Get the path to the given file
        self._mainfile = TRANSLATION_PATH.joinpath(infile + '.ini')

        # Does the file exist?
        if not self._mainfile.isfile():

            # Raise an error
            raise FileNotFoundError(
                'No file found at {0}'.format(self._mainfile))

        # Get the path to the server specific file
        self._serverfile = self._mainfile.parent.joinpath(
            self._mainfile.namebase + '_server.ini')

        # Get the strings from the main file
        main_strings = ConfigObj(self._mainfile, encoding=encoding)

        # Does the server specific file exist?
        if not self._serverfile.isfile() and not infile.startswith('_core/'):

            # Create the server specific file
            self._create_server_file()

        # Otherwise
        else:

            # Get any strings from the server specific file
            server_strings = ConfigObj(self._serverfile, encoding=encoding)

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
                language = LanguageManager.get_language(lang)

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
                language_code = LanguageManager.get_language(default_language)

                # Is the language valid?
                if not language_code is None:

                    # Set the default language
                    self.default_language = language_code

                # Delete the key from the main file as we are done with it
                del main_strings['DEFAULT_LANGUAGE']

    def __setattr__(self, attribute, value):
        '''Override the __setattr__ method to register the default language'''

        # Is the given attribute the default language?
        if attribute == 'default_language':

            # Get the given language code
            language_code = LanguageManager.get_language(value)

            # Is the given language code valid?
            if not language_code is None:

                # Loop through all strings
                for key in self:

                    # Set the default language to use for that string
                    self[key]._default_language = language_code

                # Override the given value
                value = language_code

        # Set the attribute
        super(LangStrings, self).__setattr__(attribute, value)

    def _create_server_file(self):
        '''Creates a server specific langstrings file'''

        # Get the server specific file's ConfigObj instance
        server_file = ConfigObj(self._serverfile)

        # Set the initial comments to explain what the file is for
        server_file.initial_comment = _translation_strings[
            'Initial Comment'].get_string(
            LanguageManager.default,
            filename=self._mainfile.replace(GAME_PATH, '')).splitlines()

        # Write the server specific file
        server_file.write()

    @staticmethod
    def _replace_escaped_sequences(given_string):
        '''Fixes all double escaped strings'''

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


class TranslationStrings(dict):
    '''Dictionary used to store and get language
        strings for a particular string'''

    def __init__(self):
        '''Stores an empty dictionary as the tokens'''

        self.tokens = {}

    def get_string(self, language=None, **tokens):
        '''Returns the language string for the given language/tokens'''

        # Was no language passed?
        if language is None:

            # Set the language to the server's default
            language = LanguageManager.default

        # Get the language shortname to be used
        language = self.get_language(language)

        # Was a valid language found?
        if language is None:

            # Return an empty string
            '''
                Possibly raise an error silently here
            '''
            return ''

        # Get the message's Template instance
        message = Template(self[language])

        # Update the stored tokens with the given ones
        self.tokens.update(tokens)

        # Substitute the token in the message
        message = message.substitute(self.tokens)

        # Return the message
        return message

    def get_language(self, language):
        '''Returns the language to be used'''

        # Get the given language's shortname
        language = LanguageManager.get_language(language)

        # Was a language found?
        if not language is None and language in self:

            # Return the language
            return language

        # Is the server's default language in the dictionary?
        if LanguageManager.default in self:

            # Return the server's default language
            return LanguageManager.default

        # Is there any default language defined?
        if hasattr(self, '_default_language'):

            # Is the default language available for that string?
            if self._default_language in self:

                # Return the default language to use
                return self._default_language

        # Is the server's fallback language in the dictionary?
        if LanguageManager.fallback in self:

            # Return the server's fallback language
            return LanguageManager.fallback

        # Return None as the language, as no language has been found
        return None

# Get the translations language strings
_translation_strings = LangStrings('_core/translations_strings')
