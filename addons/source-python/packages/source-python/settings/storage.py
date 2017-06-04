# ../settings/storage.py

"""Provides SQL storage and access to player specific settings."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Python Imports
#   SQLite3
from sqlite3 import connect

# Source.Python Imports
#   Listeners
from listeners import on_level_shutdown_listener_manager
#   Paths
from paths import SP_DATA_PATH


# =============================================================================
# >> GLOBAL VARIABLES
# =============================================================================
# Get the path to the user settings database file
_STORAGE_PATH = SP_DATA_PATH / 'settings' / 'users.db'

# Does the ../data/source-python/settings/ directory exist?
if not _STORAGE_PATH.parent.isdir():

    # Create the ../data/source-python/settings/ directory
    _STORAGE_PATH.parent.mkdir()


# =============================================================================
# >> CLASSES
# =============================================================================
class _UniqueSettings(dict):
    """Class used to interact with the database for a specific uniqueid."""

    def __init__(self, uniqueid):
        """Store the given uniqueid and adds it to the players table."""
        # Call the super class' __init__ to initialize the dictionary
        super().__init__()

        # Store the given uniqueid
        self.uniqueid = uniqueid

        # If _player_settings_storage is initializing, don't try to call it
        if _IN_INITIALIZATION:
            return

        # Add the uniqueid to the players table if it is not already a member
        _player_settings_storage.cursor.execute(
            """INSERT OR IGNORE INTO players VALUES(null, ?)""",
            (self.uniqueid, ))

    def __setitem__(self, variable, value):
        """Insert the given variable and value to their respective tables."""
        # Set the given variable's value in the dictionary
        super().__setitem__(variable, value)

        # If _player_settings_storage is initializing, don't try to call it
        if _IN_INITIALIZATION:
            return

        # Add the variable to the variables table if it is not already a member
        _player_settings_storage.cursor.execute(
            """INSERT OR IGNORE INTO variables VALUES(null, ?)""",
            (variable, ))

        # Set the value of the variable/uniqueid combination
        _player_settings_storage.cursor.execute(
            """INSERT OR REPLACE INTO variable_values SELECT """ +
            """variables.id, players.id, ? FROM variables, players """ +
            """WHERE variables.name=? AND players.uniqueid=?""",
            (value, variable, self.uniqueid))


class _PlayerSettingsDictionary(dict):
    """Dictionary class used to store user specific settings values."""

    def __init__(self):
        """Connect to the database and retrieve any stored values."""
        # Call the super class' __init__ to initialize the dictionary
        super().__init__()

        # Connect to the database
        self.connection = connect(_STORAGE_PATH)

        # Set the text factory
        self.connection.text_factory = str

        # Get the cursor instance
        self.cursor = self.connection.cursor()

        # Create the variables table if it does not exist
        self.cursor.execute(
            """CREATE TABLE IF NOT EXISTS variables (id INTEGER """
            """PRIMARY KEY AUTOINCREMENT, name TEXT UNIQUE)""")

        # Create the players table if it does not exist
        self.cursor.execute(
            """CREATE TABLE IF NOT EXISTS players (id INTEGER """ +
            """PRIMARY KEY AUTOINCREMENT, uniqueid TEXT UNIQUE)""")

        # Create the variable_values table if it does not exist
        self.cursor.execute(
            """CREATE TABLE IF NOT EXISTS variable_values (vid """
            """INTEGER, pid INTEGER, value, PRIMARY KEY (vid, pid))""")

        # Get all current data from the variable_values table
        data = self.cursor.execute(
            """SELECT V.name, P.uniqueid, value FROM """ +
            """variable_values AS R JOIN variables AS V ON """ +
            """R.vid=V.id JOIN players AS P ON R.pid=P.id""")

        # Fetch all objects from the result
        data = data.fetchall()

        # Is the table empty?
        if not data:

            # No need to go further
            return

        # Loop through all values in the data
        for variable, uniqueid, value in data:

            # Store the value for the given uniqueid and variable name
            self[uniqueid][variable] = value

    def __missing__(self, uniqueid):
        """Add the given uniqueid to the dictionary."""
        # Add the uniqueid to the dictionary
        value = self[uniqueid] = _UniqueSettings(uniqueid)

        # Return the _UniqueSettings instance
        return value

    def on_level_shutdown(self):
        """Store the dictionary to the database on map change."""
        self.connection.commit()

# Set a variable to make sure _UniqueSettings doesn't
#   call _player_settings_storage while it is being initialized.
_IN_INITIALIZATION = True

# Get the _PlayerSettingsDictionary instance
_player_settings_storage = _PlayerSettingsDictionary()

# Now that the initialization is done, allow
#   _UniqueSettings to call _player_settings_storage.
_IN_INITIALIZATION = False

# Register a level shutdown listener to store the database on map change
on_level_shutdown_listener_manager.register_listener(
    _player_settings_storage.on_level_shutdown)
