# ../commands/typed.py

#: .. todo:: Add the ability to define prefixes for messages.
#: .. todo:: Add callback to the Node class. It could be called when a sub-command is required.

# =============================================================================
# >> IMPORTS
# =============================================================================
# Python Imports
#   Contextlib
from contextlib import suppress
#   Inspect
import inspect
from inspect import Parameter
import itertools
import textwrap

# Source.Python
#   Auth
from auth.manager import auth_manager
#   Core
from core import AutoUnload
#   Commands
from commands import commands_logger
from commands import CommandReturn
from commands.server import server_command_manager
from commands.client import client_command_manager
from commands.say import say_command_manager
#   Filters
from filters.players import parse_filter
#   Messages
from messages import SayText2
from messages import TextMsg
from messages import HudDestination
#   Translations
from translations.strings import TranslationStrings


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('PRIVATE_COMMAND_PREFIX',
           'SUPPORTED_KINDS',
           'ArgumentError',
           'ArgumentNumberMismatch',
           'CommandInfo',
           'CommandNode',
           'CommandParser',
           'filter_str',
           'InvalidArgumentValue',
           'Node',
           'Store',
           'SubCommandError',
           'SubCommandExpectedError',
           'SubCommandNotFound',
           'TypedClientCommand',
           'TypedSayCommand',
           'TypedServerCommand',
           'ValidationError'
           )


# =============================================================================
# >> CONSTANTS
# =============================================================================
SUPPORTED_KINDS = (
    # There is no syntax for this in Python, but a PEP. If it ever gets added,
    # this implementation will already support it (unless
    # <Parameter object>.name is empty).
    Parameter.POSITIONAL_ONLY,
    Parameter.POSITIONAL_OR_KEYWORD,
    Parameter.VAR_POSITIONAL,
)

PRIVATE_COMMAND_PREFIX = '/'


# =============================================================================
# >> GLOBAL VARIABLES
# =============================================================================
logger = commands_logger.typed


# =============================================================================
# >> EXCEPTIONS
# =============================================================================
class ValidationError(Exception):
    def __init__(self, message='', language=None, **tokens):
        self.message = message
        self.language = language
        self.tokens = tokens

class ArgumentError(ValidationError): pass
class ArgumentNumberMismatch(ArgumentError): pass
class InvalidArgumentValue(ArgumentError): pass

class SubCommandError(ValidationError): pass
class SubCommandNotFound(SubCommandError): pass
class SubCommandExpectedError(SubCommandError): pass


# =============================================================================
# >> CLASSES
# =============================================================================
class Node(object):
    def __init__(self, commands, description):
        self.description = description
        self.commands = commands

    @property
    def signature(self):
        """Return the signature of the node.

        :rtype: str
        """
        raise NotImplementedError('Must be implemented by a sub class.')


class Store(Node, dict):
    def __init__(self, commands=None, description=None):
        super().__init__(commands, description)

    @property
    def signature(self):
        """Return the signature of the node.

        :rtype: str
        """
        return ' '.join(self.commands) + ' <sub-command>'

    @property
    def help_text(self):
        """Return the help text of the node.

        :rtype: str
        """
        wrapper = textwrap.TextWrapper(
            40, subsequent_indent='  ', break_long_words=True)

        result = ''
        for node in sorted(self.values(), key=lambda node: node.signature):
            sig_lines = wrapper.wrap(node.signature)
            desc_lines = wrapper.wrap(node.description or '')
            for sig, desc in itertools.zip_longest(
                    sig_lines, desc_lines, fillvalue=''):
                result += '\n  ' + sig.ljust(40) + '  ' + desc

        return result


class CommandNode(Node):
    def __init__(self, commands, params, callback, description, permission,
            fail_callback, requires_registration):
        """Initialize the object.

        .. seealso:: :meth:`CommandParser.add_command`
        """
        super().__init__(commands, description)
        self.params = params
        self.callback = callback

        # Explicitly check against True, because <permission> can also be a
        # non-empty string
        if permission == True:
            permission = '.'.join(commands)

        self.permission = permission
        self.fail_callback = fail_callback
        self.requires_registration = requires_registration
        self.command_to_register = commands[0]

    @property
    def signature(self):
        """Return the full signature of a command.

        :rtype: str
        """
        result = ' '.join(self.commands)
        for param in self.params:
            result += ' ' + self._param_to_str(param)

        return result

    @staticmethod
    def _param_to_str(param):
        """Represent a parameter as a string.

        :rtype: str
        """
        result = '{open_char}{arg_name}{type_name}{default}{close_char}'
        if (param.kind == param.VAR_POSITIONAL or
                param.default is not param.empty):
            open_char = '['
            close_char = ']'
        else:
            open_char = '<'
            close_char = '>'

        if param.default is not param.empty:
            default = '=' + str(param.default)
        else:
            default = ''

        if param.annotation is not param.empty:
            type_name = ':' + param.annotation.__name__
        else:
            type_name = ''

        if param.kind == param.VAR_POSITIONAL:
            arg_name = '*' + param.name
        else:
            arg_name = param.name

        return result.format(open_char=open_char, arg_name=arg_name,
            type_name=type_name, default=default, close_char=close_char)


# TODO: This base class is not quite correct.
class CommandParser(Store):
    def add_command(self, commands, params, callback, description=None,
            permission=None, fail_callback=None):
        """Add a command to the parser.

        :param str/list/tuple commands:
            Command to register.
        :param iterable params:
            Parameters of the command.
        :param callable callback:
            The callback for the command.
        :param str description:
            Description of the command.
        :param str permission:
            Required permission to use the command.
        :param callable fail_callback:
            Callback that gets called if authorization failed.
        :rtype: CommandNode
        """
        commands = self._validate_commands(commands)
        for param in params:
            if param.kind not in SUPPORTED_KINDS:
                raise TypeError(
                    'Unsupported argument type "{}" for argument.'.format(
                        param.kind, param.name))

        command = CommandNode(tuple(commands), tuple(params), callback,
            description, permission, fail_callback, commands[0] not in self)

        parsed_commands = []
        store = self
        while commands:
            command_name = commands.pop(0).lower()
            parsed_commands.append(command_name)
            if command_name in store:
                store = store[command_name]
                if (isinstance(store, CommandNode) or
                        (not commands and isinstance(store, Store))):
                    # We can't support multiple callbacks, because they might
                    # have different signatures.
                    raise ValueError('Command already exists.')
            else:
                if commands:
                    # We need to split these two lines to prevent recursive
                    # dicts
                    new_store = store[command_name] = Store(
                        tuple(parsed_commands))
                    store = new_store
                else:
                    store[command_name] = command

        return command

    def remove_command(self, commands):
        """Remove a command.

        :param str/list/tuple:
            Command to remove.

        .. seealso:: :meth:`_remove_command`
        .. seealso:: :meth:`_validate_commands`
        """
        return self._remove_command(self._validate_commands(commands))

    def _remove_command(self, commands):
        """Remove a command.

        :param list commands:
            Command to remove.
        :raise ValueError:
            Raised if the node does not exist.
        :return:
            Return whether the base command needs to be unregistered.
        :rtype: bool
        """
        store = self
        for command_name in commands[:-1]:
            try:
                store = store[command_name]
            except KeyError:
                raise ValueError('Command does not exist.')

        del store[commands[-1]]
        if len(commands) > 1:
            if store:
                return False

            return self._remove_command(commands[:-1])

        return True

    def get_node(self, commands):
        """Return a node.

        :param str/list/tuple:
            Node to seach.
        :raise ValueError:
            Raised if the node does not exist.
        :rtype: Node

        .. seealso:: :meth:`_validate_command`
        """
        store = self
        for command_name in self._validate_commands(commands):
            try:
                store = store[command_name]
            except (TypeError, KeyError):
                # The TypeError happens if "store" is already a CommandNode
                raise ValueError('Node does not exist.')

        return store

    def set_node_description(self, commands, description):
        """Set the description of a node.

        :param str/list/tuple:
            Node to seach.
        :raise ValueError:
            Raised if the node does not exist.
        """
        self.get_node(commands).description = description

    def get_command(self, commands):
        """Return a command.

        :param str/list/tuple:
            Command to search.
        :rtype: CommandNode

        .. seealso:: :meth:`get_node`
        """
        command = self.get_node(commands)
        assert isinstance(command, CommandNode)
        return command

    def _validate_commands(self, commands):
        """Validate a string, list or tuple of commands.

        :param str/list/tuple:
            The command name or command path to validate.
        :raise TypeError:
            Raised if ``commands`` is not a str, list or tuple.
        :raise ValueError:
            Raised if ``commands`` is an empty list or tuple.

        .. seealso:: :meth:`_validate_command`
        """
        if isinstance(commands, str):
            self._validate_command(commands)
            commands = [commands]
        elif not isinstance(commands, (list, tuple)):
            raise TypeError('<commands> must be str, list or tuple.')
        elif not commands:
            raise ValueError('<commands> must not be empty.')
        else:
            for command_name in commands:
                self._validate_command(command_name)

        return list(commands)

    def _validate_command(self, command_name):
        """Validate a command name.

        :param str command_name:
            The command name to check.
        :raise ValueError:
            Raised if the command name is invalid.
        """
        if not command_name:
            raise ValueError('Command name cannot be empty.')

        if ' ' in command_name:
            raise ValueError('Command cannot contain spaces.')

    def clean_command(self, command, args):
        """Clean a command and its passed arguments.

        :param CommandNode command:
        :param iterable args:
            The arguments to clean.
        :rtype: list
        :raise InvalidArgumentValue:
            Raised if the value is invalid for an argument.
        :raise ArgumentNumberMismatch:
            Raised if too many/less arguments have been passed.
        """
        result = []
        params = list(command.params)
        param = None
        for arg in args:
            if param is None or param.kind is not param.VAR_POSITIONAL:
                try:
                    param = params.pop(0)
                except IndexError:
                    raise ArgumentNumberMismatch(
                        'Too many arguments:\n  {}'.format(command.signature))

            if param.annotation is not param.empty:
                try:
                    arg = param.annotation(arg)
                except ValidationError:
                    raise
                except:
                    raise InvalidArgumentValue(
                        '"{}" is an invalid value for "{}:{}".'.format(
                            arg, param.name, param.annotation.__name__))

            result.append(arg)

        for param in params:
            if param.kind is param.VAR_POSITIONAL:
                break

            if param.default is param.empty:
                raise ArgumentNumberMismatch(
                    'Not enough arguments:\n  {}'.format(command.signature))

            result.append(param.default)

        return result

    def parse_command(self, command):
        """Parse a :class:`Command` object.

        Splits the command into the actual command and its arguments.

        :param Command command:
            A command to parse.
        :raise SubCommandNotFound:
            Raised if a sub command was not found.
        :raise SubCommandExpected:
            Raised if a sub command was expected, but more arguments have been
            passed.
        :rtype: tuple
        """
        args = list(command)
        store = self
        while args and isinstance(store, Store):
            sub_command = args.pop(0).lower()
            try:
                store = store[sub_command]
            except KeyError:
                raise SubCommandNotFound(
                    'Sub command "{}" not found.'.format(sub_command))

        if isinstance(store, Store):
            raise SubCommandExpectedError(
                'A sub-command is required:{}'.format(store.help_text))

        return (store, args)


class CommandInfo(object):
    """Stores command information for typed commands."""

    def __init__(self, command, typed_command_cls, index=None, team_only=None):
        """Initializes the instance.

        :param Command command:
            The actual Command instance.
        :param _TypedCommand typed_command_cls:
            Command this instance belongs to.
        :param int index:
            The index of the player that issued the command. None, if it's a
            server command.
        :param bool team_only:
            Indicates whether the command was issued in team chat. None, if
            it's a server or client command.
        """
        self.command = command
        self.typed_command_cls = typed_command_cls
        self.index = index
        self.team_only = team_only

    def reply(self, msg, language=None, **tokens):
        """Reply to the command issuer.

        :param str/TranslationStrings msg:
            Message to send.
        :param str language:
            Language to be used.
        :param tokens:
            Translation tokens for message.
        """
        self.typed_command_cls.send_message(self, msg, language, **tokens)

    def is_private_command(self):
        """Return ``True`` if it's a private command.

        :rtype: bool
        """
        return self.command[0].startswith(PRIVATE_COMMAND_PREFIX)

    @property
    def auto_command_return(self):
        """Determine the probably most desired ``CommandReturn`` value.

        For server commands it will always return ``CommandReturn.CONTINUE``.
        Client commands will always use ``CommandReturn.BLOCK``. For say
        commands it's determined by checking the prefix. If the command starts
        with a slash (``/``) it's handled as a private command. Thus,
        ``CommandReturn.BLOCK`` is returned. Otherwise
        ``CommandReturn.CONTINUE`` is used.

        :rtype: CommandReturn
        """
        return self.typed_command_cls.get_auto_command_return(self)


# We can't integrate this into SayCommand, ServerCommand and ClientCommand,
# because multiple callbacks are not supported by this system (because of the
# possibility of different function signatures). But multiple callbacks are
# important e.g. for ClientCommand, because that can actually hook client
# commands and multiple plugins might want to hook a client command.
class _TypedCommand(AutoUnload):
    """Decorator class to create typed commands."""

    def __init__(self, commands, permission=None, fail_callback=None):
        """Register a typed command callback.

        :param str/list/tuple commands:
            (Sub-) command to register.
        :param str/bool permission:
            A permission that is required to execute the command. If True, the
            permission string will be generated from the given command.
        :param callable fail_callback:
            Called when the executer does not have the required permission.
        """
        self.commands = commands
        self.permission = permission
        self.fail_callback = fail_callback
        self.command = None

    def __call__(self, callback):
        """Finish registering a typed command callback.

        :param callable callback:
            A callback that get called when the command has been issued.
        :raise ValueError:
            Raised if the callback does not accept at least one argument
            (command info).
        :return:
            Return the passed callback.
        :rtype: callable
        """
        params = tuple(inspect.signature(callback).parameters.values())
        if not params:
            raise ValueError(
                'Callback must at least accept 1 argument (command_info).')

        self.command = self.parser.add_command(
            self.commands, params[1:], callback, inspect.getdoc(callback),
            self.permission, self.fail_callback)

        if self.command.requires_registration:
            self.manager.register_commands(
                self.command.command_to_register, self.on_command)

        return callback

    def _unload_instance(self):
        with suppress(ValueError):
            if self.parser.remove_command(self.commands):
                self.manager.unregister_commands(
                    self.command.command_to_register, self.on_command)

    @classmethod
    def on_command(cls, command, *args):
        """Called when a (base) command has been executed.

        Parse the command, clean its arguments and execute the callback.
        """
        info = CommandInfo(command, cls, *args)
        try:
            command_node, args = cls.parser.parse_command(info.command)
            result = cls.on_clean_command(info, command_node, args)
        except ValidationError as e:
            info.reply(e.message, e.language, **e.tokens)
        else:
            if result is None:
                return info.auto_command_return

            return result

        return CommandReturn.CONTINUE

    @classmethod
    def on_clean_command(cls, command_info, command_node, args):
        """Called when the arguments of the parsed command should be cleaned.

        :rtype: CommandReturn
        """
        cleaned_args = cls.parser.clean_command(command_node, args)
        return command_node.callback(command_info, *cleaned_args)

    @property
    def parser(self):
        """Return the parser to use.

        :rtype: CommandParser
        """
        raise NotImplementedError('Needs to be implemented by a sub class.')

    @property
    def manager(self):
        """Return the manager that registers the commands.

        :rtype: _BaseCommandManager
        """
        raise NotImplementedError('Needs to be implemented by a sub class.')

    @staticmethod
    def send_message(command_info, message, language=None, **tokens):
        """Send a message."""
        raise NotImplementedError('Needs to be implemented by a sub class.')

    @classmethod
    def get_auto_command_return(cls, info):
        """Return the most desired ``CommandReturn`` value.

        :rtype: CommandReturn
        """
        raise NotImplementedError('Needs to be implemented by a sub class.')


class TypedServerCommand(_TypedCommand):
    """Decorator class to create typed server commands."""

    parser = CommandParser()
    manager = server_command_manager

    @staticmethod
    def send_message(command_info, message, language=None, **tokens):
        # Translate the message if it's a :class:`TranslationStrings` object.
        if isinstance(message, TranslationStrings):
            message = message.get_string(language, **tokens)

        logger.log_message(message)

    @classmethod
    def get_auto_command_return(cls, info):
        return CommandReturn.CONTINUE


class _TypedPlayerCommand(_TypedCommand):
    """Decorator class to create typed player based commands."""

    @classmethod
    def on_clean_command(cls, command_info, command_node, args):
        if (command_node.permission is None or
                auth_manager.is_player_authorized(
                    command_info.index, command_node.permission)):
            return super().on_clean_command(command_info, command_node, args)

        return cls.handle_fail_callback(command_info, command_node, args)

    @classmethod
    def handle_fail_callback(cls, command_info, command_node, args):
        if command_node.fail_callback is not None:
            return command_node.fail_callback(command_info, args)

        # TODO: Send "Required permission: <permission>" or write it to the
        #       logs?
        cls.send_message(
            command_info,
            'You are not authorized to use this command.\n' +
                'Required permission: {}'.format(command_node.permission))
        return CommandReturn.CONTINUE


class TypedClientCommand(_TypedPlayerCommand):
    """Decorator class to create typed client commands."""

    parser = CommandParser()
    manager = client_command_manager

    @staticmethod
    def send_message(command_info, message, language=None, **tokens):
        TextMsg(message, HudDestination.CONSOLE).send(command_info.index, **tokens)

    @classmethod
    def get_auto_command_return(cls, info):
        return CommandReturn.BLOCK


class TypedSayCommand(_TypedPlayerCommand):
    """Decorator class to create typed say commands."""

    parser = CommandParser()
    manager = say_command_manager

    @staticmethod
    def send_message(command_info, message, language=None, **tokens):
        SayText2(message).send(command_info.index, **tokens)

    @classmethod
    def get_auto_command_return(cls, info):
        if info.is_private_command():
            return CommandReturn.BLOCK

        return CommandReturn.CONTINUE


# =============================================================================
# >> COMMAND ANNOTATIONS
# =============================================================================
def filter_str(expr):
    """.. seealso:: :func:`filters.players.parse_filter`"""
    # A simple wrapper for parse_filter for a better looking command signature
    return parse_filter(expr)
