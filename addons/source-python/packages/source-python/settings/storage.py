# ../settings/storage.py

# =============================================================================
# >> IMPORTS
# =============================================================================
# Python Imports
#   Collections
from collections import defaultdict
#   Pickle
import pickle

# Source.Python Imports
from paths import SP_DATA_PATH
#   Events
from events.manager import EventRegistry


# =============================================================================
# >> GLOBAL VARIABLES
# =============================================================================
# Get the path to the user settings database file
_STORAGE_PATH = SP_DATA_PATH.joinpath('settings', 'users.db')


# =============================================================================
# >> CLASSES
# =============================================================================
class _PlayerSettingsDictionary(defaultdict):
    '''Dictionary class used to store user specific settings values'''

    def __init__(self, default_factory):
        '''Loads all values from the database into the dictionary'''

        # Call the super class' __init__ to initialize the defaultdict
        super(_PlayerSettingsDictionary, self).__init__(default_factory)

        # Has the database previously been saved?
        if not _STORAGE_PATH.isfile():

            # No need to go further
            return

        # Open/close the settings database
        with _STORAGE_PATH.open('rb') as _user_settings:

            # Get the stored settings
            _settings = pickle.load(_user_settings)

            # Loop through all stored settings
            for key, value in _settings.items():

                # Store the setting
                self[key] = value

    def server_spawn(self, game_event):
        '''Stores the dictionary to the database on map change'''

        # Are there any values stored in the dictionary?
        if not self:

            # If not, no need to go further
            return

        # Open/close the settings database as write
        with _STORAGE_PATH.open('wb') as _user_settings:

            # Dump the dictionary to the database file
            pickle.dump(self, _user_settings)

# Get the _PlayerSettingsDictionary instance
_PlayerSettingsStorage = _PlayerSettingsDictionary(dict)

# Register for the event server_spawn in order
# to store the database to file on map change
EventRegistry.register_for_event(
    'server_spawn', _PlayerSettingsStorage.server_spawn)
