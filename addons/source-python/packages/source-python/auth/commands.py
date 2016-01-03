# TODO Add commands to add/remove permissions once command filters are added

import re

from players.entity import Player

from commands.client import ClientCommand
from commands.server import ServerCommand

from messages import HudDestination
from messages import TextMsg

from . import auth_logger
from .manager import auth_manager


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
# TODO: Fill __all__
__all__ = ()


# =============================================================================
# >> GLOBAL VARIABLES
# =============================================================================
argument_re = re.compile('<([^>]+)>')


# =============================================================================
# >> CLASSES
# =============================================================================
class AuthCommand(object):
    def __init__(self, command, permission=None):
        self.permission = permission
        self.command = command
        self.arguments = []
        for arg in command.split(' '):
            if argument_re.match(arg):
                self.arguments.append(None)
            else:
                self.arguments.append(arg)
        self.callback = None

    def __call__(self, callback, *args, **kwargs):
        self.callback = callback
        return callback

    def test(self, index, command):
        if command.get_arg_count() - 1 != len(self.arguments):
            return
        if self.permission is not None:
            if index is not None and self.permission not in Player(index).permissions:
                return
        args = []
        for i in range(0, len(self.arguments)):
            if self.arguments[i] is not None and self.arguments[i] != command[i + 1]:
                return
            elif self.arguments[i] is None:
                args.append(command[i + 1])
        if self.callback is not None:
            self.callback(index, *args)


class AuthCommandManager(list):
    def register_command(self, command, permission=None):
        authcommand = AuthCommand(command, permission)
        self.append(authcommand)
        return authcommand


auth_commands = AuthCommandManager()


@ClientCommand('auth')
@ServerCommand('auth')
def _auth_command(command, index=None):
    for authcommand in auth_commands:
        authcommand.test(index, command)


@auth_commands.register_command('load <backend>', 'sp.auth.commands.load')
def load_backend_command(index, backend):
    if auth_manager.load_backend(backend):
        message = '[Auth] Loaded backend {} succesfully.'.format(backend)
    else:
        message = '[Auth] Failed to load backend {}.'.format(backend)
    if index is None:
        auth_logger.log_message(message)
    else:
        TextMsg(message, destination=HudDestination.CONSOLE).send(index)
