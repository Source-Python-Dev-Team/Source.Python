# ../core/settings.py

"""Contains the core settings which are used during plugin_load."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Site-Package Imports
#   Configobj
from configobj import ConfigObj

# Source.Python Imports
#   Core
from core.logger import core_logger
#   Paths
from paths import GAME_PATH
from paths import CFG_PATH
#   Translations
from translations.strings import LangStrings


# =============================================================================
# >> GLOBAL VARIABLES
# =============================================================================
# Get the core settings language strings
_core_strings = LangStrings('_core/core_settings_strings')

# Get the sp.core.settings logger
core_settings_logger = core_logger.settings


# =============================================================================
# >> CLASSES
# =============================================================================
class _CoreSettings(ConfigObj):
    """Class used to store core settings."""

    def __init__(self, infile, *args, **kwargs):
        """Add missing items and set comments using the server's language."""
        # Import the file
        super().__init__(infile, *args, **kwargs)
        self._language = None
        self.auto_data_update = True

    def load(self):
        """Load and update the core settings."""
        self._check_settings()

        # Add the initial comment
        self.initial_comment = ['..' +
            self.filename.replace(GAME_PATH, '').replace('\\', '/')]

        # Loop through all sections
        for section in self:

            # Add a blank line prior to the section
            self.comments[section] = ['']

        # Write the file
        self.write()

    def _check_settings(self):
        """Check all settings in the settings file."""
        self._check_base_settings()
        self._check_version_settings()
        self._check_logging_settings()
        self._check_user_settings()
        self._check_auth_settings()

    def _check_base_settings(self):
        """Add base settings if they are missing."""
        # Are there any base settings in the file?
        if 'BASE_SETTINGS' not in self:

            # Add the base settings
            self['BASE_SETTINGS'] = {}

        # Is there a language setting?
        if 'language' not in self['BASE_SETTINGS']:

            # Add the language setting
            self['BASE_SETTINGS']['language'] = 'english'

        # Get the server's language
        self._language = self['BASE_SETTINGS']['language']

        # Set the language comments
        self['BASE_SETTINGS'].comments['language'] = _core_strings[
            'language'].get_string(self._language).splitlines()

        # Auto data update
        if 'auto_data_update' not in self['BASE_SETTINGS']:
            self['BASE_SETTINGS']['auto_data_update'] = '1'

        self.auto_data_update = self['BASE_SETTINGS']['auto_data_update'] == '1'

        self['BASE_SETTINGS'].comments['auto_data_update'] = _core_strings[
            'auto_data_update'].get_string(self._language).splitlines()

    def _check_version_settings(self):
        """Add version settings if they are missing."""
        if 'VERSION_SETTINGS' not in self:
            self['VERSION_SETTINGS'] = {}

        if 'check_for_update' not in self['VERSION_SETTINGS']:
            self['VERSION_SETTINGS']['check_for_update'] = '1'

        if 'notify_on_update' not in self['VERSION_SETTINGS']:
            self['VERSION_SETTINGS']['notify_on_update'] = '1'

        self['VERSION_SETTINGS'].comments['check_for_update'] = _core_strings[
            'check_for_update'].get_string(self._language).splitlines()

        self['VERSION_SETTINGS'].comments['notify_on_update'] = _core_strings[
            'notify_on_update'].get_string(self._language).splitlines()

    def _check_logging_settings(self):
        """Add logging settings if they are missing."""
        # Are there any logging settings in the file?
        if 'LOG_SETTINGS' not in self:

            # Add the logging settings
            self['LOG_SETTINGS'] = {}

        # Is there a logging level setting?
        if 'level' not in self['LOG_SETTINGS']:

            # Add the logging level setting
            self['LOG_SETTINGS']['level'] = '2'

        # Set the logging level comments
        self['LOG_SETTINGS'].comments['level'] = _core_strings[
            'log_level'].get_string(self._language).splitlines()

        # Is there a logging areas setting?
        if 'areas' not in self['LOG_SETTINGS']:

            # Add the logging areas setting
            self['LOG_SETTINGS']['areas'] = '5'

        # Set the logging areas comments
        self['LOG_SETTINGS'].comments['areas'] = ['\n\n'] + _core_strings[
            'log_areas'].get_string(self._language).splitlines()

    def _check_user_settings(self):
        """Add user settings if they are missing."""
        # Are there any user settings in the file?
        if 'USER_SETTINGS' not in self:

            # Add the user settings
            self['USER_SETTINGS'] = {}

        # Is there a private say commands setting?
        if 'private_say_commands' not in self['USER_SETTINGS']:

            # Add the private say commands setting
            self['USER_SETTINGS']['private_say_commands'] = ''

        # Set the private say commands comments
        self['USER_SETTINGS'].comments['private_say_commands'] = _core_strings[
            'private_say_commands'].get_string(self._language).splitlines()

        # Is there a public say commands setting?
        if 'public_say_commands' not in self['USER_SETTINGS']:

            # Add the public say commands setting
            self['USER_SETTINGS']['public_say_commands'] = ''

        # Set the public say commands comments
        self['USER_SETTINGS'].comments['public_say_commands'] = _core_strings[
            'public_say_commands'].get_string(self._language).splitlines()

        # Is there a client commands setting?
        if 'client_commands' not in self['USER_SETTINGS']:

            # Add the client commands setting
            self['USER_SETTINGS']['client_commands'] = ''

        # Set the client commands comments
        self['USER_SETTINGS'].comments['client_commands'] = _core_strings[
            'client_commands'].get_string(self._language).splitlines()

    def _check_auth_settings(self):
        """Add auth settings if they are missing."""
        if 'AUTH_SETTINGS' not in self:
            self['AUTH_SETTINGS'] = {}

        if 'backend' not in self['AUTH_SETTINGS']:
            self['AUTH_SETTINGS']['backend'] = 'flatfile'

        if 'server_id' not in self['AUTH_SETTINGS']:
            self['AUTH_SETTINGS']['server_id'] = '-1'

        if 'BACKENDS' not in self['AUTH_SETTINGS']:
            self['AUTH_SETTINGS']['BACKENDS'] = {}

        self['AUTH_SETTINGS'].comments['BACKENDS'] = ['']

        from auth.manager import auth_manager
        auth_manager.find_and_add_available_backends()

        for backend in auth_manager.values():
            self._check_backend_settings(backend)

    def _check_backend_settings(self, backend):
        """Add settings for a backend if they are missing."""
        if backend.name not in self['AUTH_SETTINGS']['BACKENDS']:
            self['AUTH_SETTINGS']['BACKENDS'][backend.name] = {}

        self['AUTH_SETTINGS']['BACKENDS'].comments[backend.name] = ['']
        for option, value in backend.options.items():
            backend_settings = self['AUTH_SETTINGS']['BACKENDS'][backend.name]
            if option in backend_settings:
                backend.options[option] = backend_settings[option]
            else:
                backend_settings[option] = value

# Get the _CoreSettings instance
_core_settings = _CoreSettings(CFG_PATH / 'core_settings.ini',
    encoding='utf8')
