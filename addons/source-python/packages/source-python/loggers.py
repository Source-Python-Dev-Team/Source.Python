# ../loggers.py

# =============================================================================
# >> IMPORTS
# =============================================================================
# Python Imports
#   Logging
from logging import CRITICAL
from logging import DEBUG
from logging import ERROR
from logging import INFO
from logging import WARNING
from logging import FileHandler
from logging import Formatter
from logging import addLevelName
from logging import getLogger

# Source.Python Imports
from cvar_c import CConVar
from core import GameEngine
from core import echo_console
from paths import LOG_PATH
from public import public


# =============================================================================
# >> GLOBAL VARIABLES
# =============================================================================
# Store the areas constants
CONSOLE = 1 << 0
MAIN_LOG = 1 << 1
SP_LOG = 1 << 2
SCRIPT_LOG = 1 << 3

# Store the new level names
ALWAYS = 60

# Add the new level names
addLevelName(ALWAYS, 'MESSAGE')


# =============================================================================
# >> CLASSES
# =============================================================================
class _LogInstance(dict):
    '''Base logging class used to create child logging instances'''

    parent = None

    def __init__(self, name, parent):
        '''Stores the parent and gets a child of the parent'''

        # Store the parent instance
        self.parent = parent

        # Store a child logging instance
        self._logger = self.parent.logger.getChild(name)

    def __missing__(self, item):
        '''Adds new items as logging instances'''

        # Get the new logging instance
        value = self[item] = _LogInstance(item, self)

        # Return the logging instance
        return value

    def __getattr__(self, attr):
        '''Calls __getitem__ to return the item in the dictionary'''
        return self[attr]

    def __delitem__(self, item):
        '''Removes all children of the instance and closes the logger'''

        # Remove all children
        self[item].clear()

        # Close the logger
        self.logger.close()

    def clear(self):
        '''Deletes each item individually to close all loggers'''

        # Loop through each child
        for item in list(self):

            # Remove the child
            del self[item]

    def log_critical(self, msg, *args, **kwargs):
        '''Use to call a critical message'''
        self._log(CRITICAL, msg, *args, **kwargs)

    def log_debug(self, msg, *args, **kwargs):
        '''Use to call a debug message'''
        self._log(DEBUG, msg, *args, **kwargs)

    def log_exception(self, msg, *args, **kwargs):
        '''Use to call an exception message'''
        self._log(ERROR, msg, *args, **kwargs)

    def log_info(self, msg, *args, **kwargs):
        '''Use to call a basic info message'''
        self._log(INFO, msg, *args, **kwargs)

    def log_warning(self, msg, *args, **kwargs):
        '''Use to call a warning message'''
        self._log(WARNING, msg, *args, **kwargs)

    def log_message(self, msg, *args, **kwargs):
        '''Use to call a message that should always print'''
        self._log(ALWAYS, msg, *args, **kwargs)

    def log(self, level, msg, *args, **kwargs):
        '''Use to call a message with the given logging level'''

        # Get the value of the given level
        level = self._get_level_value(level)

        # Call the main logging method
        self._log(level, msg, *args, **kwargs)

    def _log(self, level, msg, *args, **kwargs):
        '''Main logging method'''

        # Does the message need logged?
        if self.level > level:

            # If not, simply return
            return

        # Get the areas to be used
        areas = self.areas

        # Print to main log file?
        if MAIN_LOG & areas:

            # Get the record
            record = self.logger.makeRecord(
                self.logger.name, level, '(unknown file)', 0, msg, args, None)

            # Get the message to send
            message = self.formatter.format(record)

            # Print to the main log
            GameEngine.log_print(message + '\n')

        # Print to the console?
        if CONSOLE & areas:

            # Print to the console
            echo_console(msg)

        # Print to the script's log file?
        if SCRIPT_LOG & areas and self.root != _SPLogger:

            # Print message to the log file
            self.logger.log(level, msg, *args, **kwargs)

        # Print to the main SP log file?
        if SP_LOG & areas:

            # Print to the SP log file
            _SPLogger.logger.log(level, msg, *args, **kwargs)

    @staticmethod
    def _get_level_value(level):
        '''Returns a level value used by the logging package'''
        return 50 - (10 * level)

    @property
    def root(self):
        '''Returns the root class'''

        # Store the current instance
        instance = self

        # Use a while statement to get the parent until root is found
        while not instance.parent is None:

            # Set the current instance to the parent
            instance = instance.parent

        # Return the root instance
        return instance

    @property
    def areas(self):
        '''Returns the root's areas value'''
        return self.root.areas

    @property
    def level(self):
        '''Returns the root's level value'''
        return self.root.level

    @property
    def formatter(self):
        '''Return's the root's formatter'''
        return self.root._formatter

    @property
    def logger(self):
        '''Returns the instance's logger'''
        return self._logger


@public
class LogManager(_LogInstance):
    '''Main log class used as a root to create children instances'''

    def __init__(
            self, name, level, areas, filepath=None,
            format=None, date_format=None):
        '''Stores the base values and creates the logger'''

        # Store the base formatter
        self._formatter = Formatter(format, date_format)

        # Store the base attributes
        self._level = level
        self._areas = areas

        # Create the logger
        self._logger = getLogger(name)

        # Was a filepath given?
        if not filepath is None:

            # Create the handler an add it to the logger
            handler = FileHandler(LOG_PATH.joinpath(filepath + '.log'))
            handler.setFormatter(self.formatter)
            self.logger.addHandler(handler)

    @property
    def level(self):
        '''Returns the needed level value'''
        return 50 - (self._level.get_int() * 10)

    @property
    def areas(self):
        '''Returns the areas to print messages to'''
        return self._areas.get_int()

# Set the core ConVars
_level = CConVar(
    'sp_logging_level', '0', 0, 'The Source.Python base logging level')
_areas = CConVar(
    'sp_logging_areas', '1', 0, 'The Source.Python base logging areas')

# Get the Source.Python main LogManager instance
_SPLogger = LogManager(
    'sp', _level, _areas, 'source-python',
    '%(asctime)s - %(name)s\t-\t%(levelname)s\n%(message)s',
    '%m-%d-%Y %H:%M:%S')
