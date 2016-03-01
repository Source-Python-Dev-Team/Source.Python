# ../commands/__init__.py

"""Provides server, client, and say command functionality."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Python Imports
import argparse
# Source.Python Imports
#   Loggers
from loggers import _sp_logger


# =============================================================================
# >> FORWARD IMPORTS
# =============================================================================
# Source.Python Imports
#   Commands
from _commands import Command
from _commands import CommandReturn
from _commands import ConCommandBase


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('Command',
           'CommandParser',
           'CommandReturn',
           'ConCommandBase',
           'ParserError',
           )


# =============================================================================
# >> GLOBAL VARIABLES
# =============================================================================
# Get the sp.commands logger
commands_logger = _sp_logger.commands


# =============================================================================
# >> CLASSES
# =============================================================================
class ParserError(Exception):
    """Exception raised when parsing a command's arguments."""

    def __init__(self, message):
        """Store the message."""
        super().__init__(message)
        self.message = message.rstrip() if message else ''


class CommandParser(argparse.ArgumentParser):
    """A class to parse Command objects."""

    def __init__(self, command_name, *args, **kwargs):
        """Initialize the parser.

        :param str command_name: The name of the command that will be parsed.
            This is required for proper help/error messages.

        See :class:`argparse.ArgumentParser` for more information.
        """
        super().__init__(command_name, *args, **kwargs)

    def _print_message(self, message, file=None):
        """Redirect messages that should be printed into a ParserError."""
        raise ParserError(message)

    def error(self, message):
        """Redirect error messages into a ParserError."""
        raise ParserError(message)

    def exit(self, status=0, message=None):
        """Redirect exit messages into a ParserError."""
        raise ParserError(message)

    def parse_command(self, command, namespace=None):
        """Parse a Command object.

        :param Command command: The command that should be parsed.

        See :meth:`~.argparse.ArgumentParser.parse_args` for more information.
        """
        return self.parse_args(
            [command[x] for x in range(1, len(command))], namespace)
