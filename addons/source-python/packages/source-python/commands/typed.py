#: .. todo:: Add the ability to define prefixes for messages.
#: .. todo:: Add callback to the Node class. It could be called when a sub-command is required.

# =============================================================================
# >> IMPORTS
# =============================================================================
# Python
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
from core import echo_console
#   Commands
from commands import CommandReturn
from commands.server import server_command_manager
from commands.client import client_command_manager
from commands.say import say_command_manager
#   Messages
from messages import SayText2
from messages import TextMsg
from messages import HudDestination


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


# =============================================================================
# >> EXCEPTIONS
# =============================================================================
# TODO:
# We probably need to update these exceptions if we want to add translations.
class ValidationError(Exception):
    def __init__(self, message=''):
        self.message = message

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
        raise NotImplementedError('Must be implemented by a sub class.')


class Store(Node, dict):
    def __init__(self, commands=None, description=None):
        super().__init__(commands, description)

    @property
    def signature(self):
        return ' '.join(self.commands) + ' <sub-command>'


class CommandNode(Node):
    def __init__(self, commands, params, callback, description, permission,
            fail_callback, requires_registration):
        """Initialize the object.

        .. see-also:: :meth:`CommandParser.add_command`
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

        :param str/list/tuple commands: Command to register.
        :param iterable params: Parameters of the command.
        :param callable callback: The callback for the command.
        :param str description: Description of the command.
        :param str permission: Required permission to use the command.
        :param callable fail_callback: Callback that gets called if
            authorization failed.
        :rtype: CommandNode
        """
        commands = self._validate_commands(commands)
        command = CommandNode(tuple(commands), tuple(params), callback,
            description, permission, fail_callback, commands[0] not in self)

        parsed_commands = []
        store = self
        while commands:
            command_name = commands.pop(0)
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
                    new_store = store[command_name] = Store(tuple(parsed_commands))
                    store = new_store
                else:
                    store[command_name] = command

        for param in params:
            if param.kind not in SUPPORTED_KINDS:
                raise TypeError(
                    'Unsupported argument type "{}" for argument.'.format(
                        param.kind, param.name))

        return command

    def remove_command(self, commands):
        """Remove a command.

        :param str/list/tuple: Command to remove.

        .. see-also:: _remove_command
        .. see-also:: _validate_commands
        """
        return self._remove_command(self._validate_commands(commands))

    def _remove_command(self, commands):
        """Remove a command.

        :param list commands: Command to remove.
        :return: Return whether the base command needs to be unregistered.
        :rtype: bool
        :raise ValueError: Raised if the node does not exist.
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

        :param str/list/tuple: Node to seach.
        :rtype: Node
        :raise ValueError: Raised if the node does not exist.

        .. see-also:: _validate_command
        """
        store = self
        for command_name in self._validate_commands(commands):
            try:
                store = store[command_name]
            except KeyError:
                raise ValueError('Node does not exist.')

        return store

    def get_command(self, commands):
        """Return a command.

        :param str/list/tuple: Command to search.
        :rtype: CommandNode

        .. see-also:: get_node
        """
        command = self.get_node(commands)
        assert isinstance(command, CommandNode)
        return command

    def _validate_commands(self, commands):
        """Validate a string, list or tuple of commands.

        :param str/list/tuple: The command name or command path to validate.
        :raise TypeError: Raised if ``commands`` is not a str, list or tuple.
        :raise ValueError: Raised if ``commands`` is an empty list or tuple.

        .. see-also:: _validate_command
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

        :param str command_name: The command name to check.
        :raise ValueError: Raised if the command name is invalid.
        """
        if not command_name:
            raise ValueError('Command name cannot be empty.')

        if ' ' in command_name:
            raise ValueError('Command cannot contain spaces.')

    def clean_command(self, command, args):
        """Clean a command and its passed arguments.

        :param CommandNode command:
        :param iterable args: The arguments to clean.
        :rtype: list
        :raise InvalidArgumentValue: Raised if the value is invalid for an
            argument.
        :raise ArgumentNumberMismatch: Raised if too many/less arguments have
            been passed.
        """
        result = []
        params = list(command.params)
        param = None
        for arg in args:
            if param is None or param.kind is not param.VAR_POSITIONAL:
                try:
                    param = params.pop(0)
                except IndexError:
                    raise ArgumentNumberMismatch('Too many arguments.')

            if param.annotation is not param.empty:
                try:
                    arg = param.annotation(arg)
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

        Splits the command into the actual command and its arguments

        :raise SubCommandNotFound: Raised if a sub command was not found.
        :raise SubCommandExpected: Raised if a sub command was expected, but
            more arguments have been passed.
        :rtype: tuple
        """
        args = list(command)
        store = self
        while args and isinstance(store, Store):
            sub_command = args.pop(0)
            try:
                store = store[sub_command]
            except KeyError:
                raise SubCommandNotFound(
                    'Sub command "{}" not found.'.format(sub_command))

        if isinstance(store, Store):
            raise SubCommandExpectedError(
                'A sub-command is required:{}'.format(
                    self._get_help_text(store)))

        return (store, args)

    @staticmethod
    def _get_help_text(store):
        wrapper = textwrap.TextWrapper(
            40, subsequent_indent='  ', break_long_words=True)

        result = ''
        for node in sorted(store.values(), key=lambda node: node.commands[1]):
            sig_lines = wrapper.wrap(node.signature)
            desc_lines = wrapper.wrap(node.description or '')
            temp = ''
            for sig, desc in itertools.zip_longest(
                    sig_lines, desc_lines, fillvalue=''):
                temp += '\n  ' + sig.ljust(40) + '  ' + desc

            result += temp

        return result


# We can't integrate this into SayCommand, ServerCommand and ClientCommand,
# because multiple callbacks are not supported by this system (because of the
# possibility of different function signatures). But multiple callbacks are
# important e.g. for ClientCommand, because that can actually hook client
# commands and multiple plugins might want to hook a client command.
class _TypedCommand(AutoUnload):
    """Decorator class to create typed commands."""

    def __init__(self, commands, permission=None, fail_callback=None):
        """Register a typed command callback.

        :param str/list/tuple commands: (Sub-) command to register.
        :param str/bool permission: A permission that is required to execute
            the command. If True, the permission string will be generated from
            the given command.
        :param callable fail_callback: Called when the executer does not have
            the required permission.
        """
        self.commands = commands
        self.permission = permission
        self.fail_callback = fail_callback
        self.command = None

    def __call__(self, callback):
        """Finish registering a typed command callback."""
        params = tuple(inspect.signature(callback).parameters.values())
        # TODO:
        # Do we want to force people to use names likes command, index and
        # team_only? This will make it easier to avoid issues!
        if len(params) < self.args_to_skip:
            raise ValueError(
                'Callback must at least accept {} arguments.'.format(
                    self.args_to_skip))

        self.command = self.parser.add_command(
            self.commands, params[self.args_to_skip:], callback,
            inspect.getdoc(callback), self.permission, self.fail_callback)

        if self.command.requires_registration:
            self.manager.register_commands(
                self.command.command_to_register, self.on_command)

        return callback

    def _unload_instance(self):
        if self.parser.remove_command(self.commands):
            self.manager.unregister_commands(
                self.command.command_to_register, self.on_command)

    @classmethod
    def on_command(cls, command, *additional_args):
        """Called when a (base) command has been executed.

        Parse the command, clean its arguments and execute the callback.
        """
        # TODO: Translations!
        try:
            command, args = cls.parser.parse_command(command)
            result = cls.on_clean_command(command, args, *additional_args)
        except ArgumentError as e:
            cls.send_message(e.message, *additional_args)
        except SubCommandError as e:
            cls.send_message(e.message, *additional_args)
        else:
            return result

        return CommandReturn.CONTINUE

    @classmethod
    def on_clean_command(cls, command, args, *additional_args):
        """Called when the arguments of the parsed command should be cleaned.

        :rtype: CommandReturn
        """
        cleaned_args = cls.parser.clean_command(command, args)
        return command.callback(*(list(additional_args) + cleaned_args))

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

    @property
    def args_to_skip(self):
        """Return the number of arguments to skip when getting the signature
        of a function.

        :rtype: int
        """
        raise NotImplementedError('Needs to be implemented by a sub class.')

    @staticmethod
    def send_message(message, *args):
        """Send a message.

        :param *args: Additional arguments like index and team_only.
        """
        raise NotImplementedError('Needs to be implemented by a sub class.')


class TypedServerCommand(_TypedCommand):
    """Decorator class to create typed server commands."""

    parser = CommandParser()
    manager = server_command_manager
    args_to_skip = 0

    @staticmethod
    def send_message(message):
        echo_console(message)


class _TypedPlayerCommand(_TypedCommand):
    """Decorator class to create typed player based commands."""

    @classmethod
    def on_clean_command(cls, command, args, index, *additional_args):
        if (command.permission is None or
                auth_manager.is_authorized(index, command.permission)):
            return super().on_clean_command(
                command, args, index, *additional_args)

        return cls.handle_fail_callback(
            command, args, index, *additional_args)

    @classmethod
    def handle_fail_callback(cls, command, args, index, *additional_args):
        if command.fail_callback is not None:
            return command.fail_callback(
                index, *(list(additional_args) + args))

        # TODO: Send "Required permission: <permission>" or write it to the
        #       logs?
        cls.send_message(
            'You are not authorized to use this command.\n' +
                'Required permission: {}'.format(command.permission),
            index, *additional_args)
        return CommandReturn.CONTINUE


class TypedClientCommand(_TypedPlayerCommand):
    """Decorator class to create typed client commands."""

    parser = CommandParser()
    manager = client_command_manager
    args_to_skip = 1

    @staticmethod
    def send_message(message, index):
        TextMsg(message, HudDestination.CONSOLE).send(index)


class TypedSayCommand(_TypedPlayerCommand):
    """Decorator class to create typed say commands."""

    parser = CommandParser()
    manager = say_command_manager
    args_to_skip = 2

    @staticmethod
    def send_message(message, index, team_only):
        SayText2(message).send(index)