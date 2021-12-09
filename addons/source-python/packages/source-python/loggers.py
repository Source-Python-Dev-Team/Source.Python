# ../loggers.py

"""Provides logging functionality."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Python Imports
#   Datetime
from datetime import date
from datetime import datetime
#   Logging
from logging import CRITICAL
from logging import DEBUG
from logging import ERROR
from logging import INFO
from logging import WARNING
from logging import Formatter
from logging import addLevelName
from logging import getLogger
from logging.handlers import TimedRotatingFileHandler

# Site-Packages Imports
#   Path
from path import Path

# Source.Python Imports
#   Core
from core import AutoUnload
#   Cvars
from cvars import ConVar
#   Paths
from paths import LOG_PATH


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('LogManager',
           '_LogInstance',
           )


# =============================================================================
# >> GLOBAL VARIABLES
# =============================================================================
# Store the areas constants
CONSOLE = 1 << 0
MAIN_LOG = 1 << 1
SP_LOG = 1 << 2
SCRIPT_LOG = 1 << 3

# Store the new level names
MESSAGE = 60
EXCEPTION = ERROR

# Add the new level names
addLevelName(MESSAGE, 'MESSAGE')
addLevelName(EXCEPTION, 'EXCEPTION')

# Store a formatter for use with the main log
_main_log_formatter = Formatter('- %(name)s\t-\t%(levelname)s\n\t%(message)s')

# SP's logs prefix
SP_LOG_PREFIX = '[Source.Python] '


# =============================================================================
# >> CLASSES
# =============================================================================
class _LogInstance(dict):
    """Base logging class used to create child logging instances."""

    def __init__(self, parent=None, name=None, prefix=None):
        """Store the parent and gets a child of the parent.

        :param _LogInstance parent:
            A parent logger or None.
        :param str name:
            A name for the logger or None.
        """
        # Initialize the dictionary
        super().__init__()

        # Store the parent instance
        self.parent = parent

        # Was a parent class passed?
        if self.parent is not None:

            # Store a child logging instance
            self._logger = self.parent.logger.getChild(name)

        # Store the given prefix
        self.prefix = prefix

    def __missing__(self, item):
        """Add new items as logging instances.

        :rtype: _LogInstance
        """
        # Get the new logging instance
        value = self[item] = _LogInstance(self, item, prefix=self.prefix)

        # Return the logging instance
        return value

    def __getattr__(self, attr):
        """Return the logger with the specified name.

        :rtype: _LogInstance
        """
        return self[attr]

    def __delitem__(self, item):
        """Remove all children of the instance and
        remove the logger from logging manager.
        """
        instance = self.pop(item)

        # Remove all children
        instance.clear()

        # Remove the logger from logging manager
        self.logger.manager.loggerDict.pop(instance.logger.name, None)

    def clear(self):
        """Delete each item individually to close all loggers."""
        # Loop through each child
        for item in list(self):

            # Remove the child
            del self[item]

    def log_critical(self, msg, *args, **kwargs):
        """Use to call a critical message.

        :param str msg:
            Message to log.
        :param args:
            Additional arguments that are used for formatting purpose.
        :param kwargs:
            Additional keywords that are used for formatting purpose.
        """
        self._log(CRITICAL, msg, *args, **kwargs)

    def log_debug(self, msg, *args, **kwargs):
        """Use to call a debug message.

        :param str msg:
            Message to log.
        :param args:
            Additional arguments that are used for formatting purpose.
        :param kwargs:
            Additional keywords that are used for formatting purpose.
        """
        self._log(DEBUG, msg, *args, **kwargs)

    def log_exception(self, msg, *args, **kwargs):
        """Use to call an exception message.

        :param str msg:
            Message to log.
        :param args:
            Additional arguments that are used for formatting purpose.
        :param kwargs:
            Additional keywords that are used for formatting purpose.
        """
        self._log(EXCEPTION, msg, *args, **kwargs)

    def log_info(self, msg, *args, **kwargs):
        """Use to call a basic info message.

        :param str msg:
            Message to log.
        :param args:
            Additional arguments that are used for formatting purpose.
        :param kwargs:
            Additional keywords that are used for formatting purpose.
        """
        self._log(INFO, msg, *args, **kwargs)

    def log_warning(self, msg, *args, **kwargs):
        """Use to call a warning message.

        :param str msg:
            Message to log.
        :param args:
            Additional arguments that are used for formatting purpose.
        :param kwargs:
            Additional keywords that are used for formatting purpose.
        """
        self._log(WARNING, msg, *args, **kwargs)

    def log_message(self, msg, *args, **kwargs):
        """Use to call a message that should always print.

        :param str msg:
            Message to log.
        :param args:
            Additional arguments that are used for formatting purpose.
        :param kwargs:
            Additional keywords that are used for formatting purpose.
        """
        self._log(MESSAGE, msg, *args, **kwargs)

    def log(self, level, msg, *args, **kwargs):
        """Use to call a message with the given logging level.

        :param int level:
            Level of the message to log.
        :param str msg:
            Message to log.
        :param args:
            Additional arguments that are used for formatting purpose.
        :param kwargs:
            Additional keywords that are used for formatting purpose.
        """
        # Get the value of the given level
        level = self._get_level_value(level)

        # Call the main logging method
        self._log(level, msg, *args, **kwargs)

    def _log(self, level, msg, *args, **kwargs):
        """Main logging method."""
        # Does the message need logged?
        if self.level > level:

            # If not, simply return
            return

        # Get the areas to be used
        areas = self.areas

        # Get wether we should prepend prefix
        prepend_prefix = kwargs.pop('prepend_prefix', self.prefix is not None)

        # Print to main log file?
        if MAIN_LOG & areas:

            # Import engine_server
            # This is done here to fix an ImportError
            from engines.server import engine_server

            # Create the record
            record = self.logger.makeRecord(
                self.logger.name, level,
                '(unknown file)', 0, msg, args, None)

            # Get the message to send
            message = _main_log_formatter.format(record)

            # Prepend prefix
            if prepend_prefix:
                message = self.prefix + message

            # Print to the main log
            engine_server.log_print(message + '\n')

        # Print to the console?
        if CONSOLE & areas:

            # If not, print to the console
            # If <engine>.log_print is called with logging being on,
            #   the console is already echoed with the message.
            from core import echo_console

            # Prepend prefix
            if prepend_prefix:
                msg = self.prefix + msg

            echo_console(msg)

        # Print to the script's log file?
        if SCRIPT_LOG & areas and self.root != _sp_logger:

            # Print message to the log file
            self.logger.log(level, msg, *args, **kwargs)

        # Print to the main SP log file?
        if SP_LOG & areas:

            # Get the given extra dictionary
            extra = kwargs.setdefault('extra', dict())

            # Set the logger name
            extra.setdefault('logger_name', self.logger.name)

            # Print to the SP log file
            _sp_logger.logger.log(level, msg, *args, **kwargs)

    @staticmethod
    def _get_level_value(level):
        """Return a level value used by the logging package.

        :rtype: int
        """
        return 50 - (10 * level)

    @property
    def root(self):
        """Return the root logger.

        :rtype: _LogInstance
        """
        # Store the current instance
        instance = self

        # Use a while statement to get the parent until root is found
        while instance.parent is not None:

            # Set the current instance to the parent
            instance = instance.parent

        # Return the root instance
        return instance

    @property
    def areas(self):
        """Return the root's areas value.

        :rtype: int
        """
        return self.root.areas

    @property
    def level(self):
        """Return the root's level value.

        :rtype: int
        """
        return self.root.level

    @property
    def formatter(self):
        """Return the root's formatter.

        :rtype: Formatter
        """
        return self.root._formatter

    @property
    def logger(self):
        """Return the instance's logger.

        :rtype: logging.Logger
        """
        return self._logger


class LogManager(AutoUnload, _LogInstance):
    """Main log class used as a root to create children instances."""

    def __init__(
            self, name, level, areas, filepath=None,
            log_format=None, date_format=None, encoding='utf-8',
            prefix=None):
        """Store the base values and creates the logger.

        :param str name:
            Name of the base logger.
        :param ConVar level:
            A console variable that defines required level to log a message.
        :param ConVar areas:
            A console variable that defines the areas where messages are
            getting logged.
        :param str filepath:
            A file where messages are getting logged.
        :param str log_format:
            A custom log format that defines the appearance of the messages.
        :param str date_format:
            A custom date format that defines how the date is printed.
        """
        # Initialize the dictionary
        super().__init__(prefix=prefix)

        # Store the base formatter
        self._formatter = Formatter(log_format, date_format)

        # Store the base attributes
        self._level = level
        self._areas = areas

        # Create the logger
        self._logger = getLogger(name)

        # Was a filepath given?
        if filepath is not None:

            # Does the given path end with the extension?
            if filepath.endswith('.log'):

                # Remove the extension
                filepath = filepath[:~3]

            # Get the path to the log file
            log_path = LOG_PATH / filepath + '.log'

            # Does the parent directory exist?
            if not log_path.parent.isdir():

                # Create the parent directory
                log_path.parent.makedirs()

            # Create the handler an add it to the logger
            self._handler = DailyRotatingFileHandler(log_path, when='D',
                backupCount=30, encoding=encoding)
            self._handler.setFormatter(self.formatter)
            self.logger.addHandler(self._handler)

    @property
    def level(self):
        """Return the needed level value.

        :rtype: int
        """
        return 50 - (self._level.get_int() * 10)

    @property
    def areas(self):
        """Return the areas to print messages to.

        :rtype: int
        """
        return self._areas.get_int()

    def _unload_instance(self):
        """Remove the logger from logging manager and close it."""
        self.clear()
        self.logger.manager.loggerDict.pop(self.logger.name, None)

        for handler in self.logger.handlers[:]:
            self.logger.removeHandler(handler)
            handler.close()


class DailyRotatingFileHandler(TimedRotatingFileHandler):
    """Source.Python's logging file handler."""

    file_name_format = 'source-python.%Y-%m-%d.log'

    def rotation_filename(self, default_name):
        """Returns the name of today's log file."""
        return date.today().strftime(self.file_name_format)

    def getFilesToDelete(self):
        """Returns the files to delete."""
        files = list()
        for f in Path(self.baseFilename).parent.files():
            try:
                delta = date.today() - datetime.strptime(
                    f.name, self.file_name_format).date()
                if delta.days < self.backupCount:
                    continue
                files.append(f)
            except ValueError:
                continue
        return files


# Set the core ConVars
_level = ConVar(
    'sp_logging_level', '0', 'The Source.Python base logging level')
_areas = ConVar(
    'sp_logging_areas', '1', 'The Source.Python base logging areas')

# Get the Source.Python main LogManager instance
_sp_logger = LogManager(
    'sp', _level, _areas,
    'source-python.{0}'.format(date.today().strftime('%Y-%m-%d')),
    '%(asctime)s - %(logger_name)s\t-\t%(levelname)s\t%(message)s',
    '%Y-%m-%d %H:%M:%S',
    prefix=SP_LOG_PREFIX)

# Set the parent logger level to allow all message types
_sp_logger.logger.parent.level = DEBUG
