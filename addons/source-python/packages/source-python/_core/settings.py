# ../_core/settings.py

# =============================================================================
# >> IMPORTS
# =============================================================================
# Python Imports
#   Collections
from collections import OrderedDict

# Site-Package Imports
#   Configobj
from configobj import ConfigObj

# Source.Python Imports
from _core import _CoreLogger
#   Auth
from auth.paths import AUTH_PROVIDER_PATH
#   Paths
from paths import GAME_PATH
from paths import CFG_PATH
#   Translations
from translations.strings import LangStrings


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
# Set all to an empty list
__all__ = []


# =============================================================================
# >> GLOBAL VARIABLES
# =============================================================================
# Get the core settings language strings
_core_strings = LangStrings('_core/core_settings_strings')

# Get a list of auth providers
_auth_providers = [
    provider.namebase for provider in AUTH_PROVIDER_PATH.files() +
    AUTH_PROVIDER_PATH.dirs() if not provider.namebase.startswith('__')]

# Get the sp._core.settings logger
_CoreSettingsLogger = _CoreLogger.settings


# =============================================================================
# >> CLASSES
# =============================================================================
class _SettingsMeta(type):
    '''Metaclass used to store methods in order of creation'''

    @classmethod
    def __prepare__(mcl, name, bases):
        '''Returns an ordered dictionary'''
        return OrderedDict()

    def __new__(mcl, name, bases, odict):
        '''
            Stores methods by name and instance in the order they were created
        '''

        # Get the class object
        cls = super().__new__(mcl, name, bases, dict(odict))

        # Create an ordered dictionary to store methods in
        cls._odict = OrderedDict()

        # Loop through the methods
        for item in odict:

            # Is the current method one that needs stored?
            if item.startswith('_check_'):

                # Store the method by its name
                cls._odict[item] = odict[item]

        # Return the class
        return cls


class _CoreSettings(ConfigObj, metaclass=_SettingsMeta):
    '''Class used to store core settings'''

    def __init__(self, infile):
        '''
            Adds missing items and sets their comments to the server's language
        '''

        # Import the file
        super(_CoreSettings, self).__init__(infile)

        # Loop through the registered methods
        for item in self._odict:

            # Call the method
            self._odict[item](self)

        # Add the initial comment
        self.initial_comment = ['../' + self.filename.replace(GAME_PATH, '')]

        # Loop through all sections
        for section in self:

            # Add a blank line prior to the section
            self.comments[section] = ['']

        # Write the file
        self.write()

    def _check_base_settings(self):
        '''Adds base settings if they are missing'''

        # Are there any base settings in the file?
        if not 'BASE_SETTINGS' in self:

            # Add the base settings
            self['BASE_SETTINGS'] = {}

        # Is there a language setting?
        if not 'language' in self['BASE_SETTINGS']:

            # Add the language setting
            self['BASE_SETTINGS']['language'] = 'english'

        # Get the server's language
        self._language = self['BASE_SETTINGS']['language']

        # Set the language comments
        self['BASE_SETTINGS'].comments['language'] = _core_strings[
            'language'].get_string(self._language).splitlines()

    def _check_auth_settings(self):
        '''Adds auth settings if they are missing'''

        # Are there any auth settings in the file?
        if not 'AUTH_SETTINGS' in self:

            # Add the auth settings
            self['AUTH_SETTINGS'] = {}

        # Is there a providers setting?
        if not 'providers' in self['AUTH_SETTINGS']:

            # Add the providers setting
            self['AUTH_SETTINGS']['providers'] = ''

        # Set the auth provider comments
        self['AUTH_SETTINGS'].comments['providers'] = _core_strings[
            'providers'].get_string(
            self._language,
            providers='\n'.join(_auth_providers),
            single=_auth_providers[0],
            multiple=' '.join(_auth_providers[:3])).splitlines()

    def _check_logging_settings(self):
        '''Adds logging settings if they are missing'''

        # Are there any logging settings in the file?
        if not 'LOG_SETTINGS' in self:

            # Add the logging settings
            self['LOG_SETTINGS'] = {}

        # Is there a logging level setting?
        if not 'level' in self['LOG_SETTINGS']:

            # Add the logging level setting
            self['LOG_SETTINGS']['level'] = '2'

        # Set the logging level comments
        self['LOG_SETTINGS'].comments['level'] = _core_strings[
            'log_level'].get_string(self._language).splitlines()

        # Is there a logging areas setting?
        if not 'areas' in self['LOG_SETTINGS']:

            # Add the logging areas setting
            self['LOG_SETTINGS']['areas'] = '1'

        # Set the logging areas comments
        self['LOG_SETTINGS'].comments['areas'] = ['\n\n'] + _core_strings[
            'log_areas'].get_string(self._language).splitlines()

    def _check_user_settings(self):
        '''Adds user settings if they are missing'''

        # Are there any user settings in the file?
        if not 'USER_SETTINGS' in self:

            # Add the user settings
            self['USER_SETTINGS'] = {}

        # Is there a private say commands setting?
        if not 'private_say_commands' in self['USER_SETTINGS']:

            # Add the private say commands setting
            self['USER_SETTINGS']['private_say_commands'] = ''

        # Set the private say commands comments
        self['USER_SETTINGS'].comments['private_say_commands'] = _core_strings[
            'private_say_commands'].get_string(self._language).splitlines()

        # Is there a public say commands setting?
        if not 'public_say_commands' in self['USER_SETTINGS']:

            # Add the public say commands setting
            self['USER_SETTINGS']['public_say_commands'] = ''

        # Set the public say commands comments
        self['USER_SETTINGS'].comments['public_say_commands'] = _core_strings[
            'public_say_commands'].get_string(self._language).splitlines()

        # Is there a client commands setting?
        if not 'client_commands' in self['USER_SETTINGS']:

            # Add the client commands setting
            self['USER_SETTINGS']['client_commands'] = ''

        # Set the client commands comments
        self['USER_SETTINGS'].comments['client_commands'] = _core_strings[
            'client_commands'].get_string(self._language).splitlines()

# Get the _CoreSettings instance
_CoreSettingsInstance = _CoreSettings(CFG_PATH.joinpath('core_settings.ini'))
