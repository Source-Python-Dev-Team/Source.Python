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
_STORAGE_PATH = SP_DATA_PATH.joinpath('settings', 'users.db')


# =============================================================================
# >> CLASSES
# =============================================================================
class _PlayerSettingsDictionary(defaultdict):
    '''Dictionary class used to store user specific settings values'''

    def __init__(self, default_factory):
        '''Loads all values from the database into the dictionary'''

        # Has the database previously been saved?
        if _STORAGE_PATH.isfile():

            # Open/close the settings database
            with _STORAGE_PATH.open('r') as _user_settings:

                # Load the database into the dictionary
                super(_PlayerSettingsDictionary, self).__init__(
                    pickle.load(_user_settings))

        # Has the database never been saved?
        else:

            # Call the super class' __init__ to initialize the defaultdict
            super(_PlayerSettingsDictionary, self).__init__(default_factory)

    def server_spawn(self, game_event):
        '''Stores the dictionary to the database on map change'''

        """
        # Open/close the settings database as write
        with _STORAGE_PATH.open('w') as _user_settings:

            # Dump the dictionary to the database file
            pickle.dump(self, _user_settings)
        """

# Get the _PlayerSettingsDictionary instance
_PlayerSettingsStorage = _PlayerSettingsDictionary(dict)

# Register for the event server_spawn in order
# to store the database to file on map change
EventRegistry.register_for_event(
    'server_spawn', _PlayerSettingsStorage.server_spawn)
