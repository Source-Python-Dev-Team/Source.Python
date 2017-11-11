# ../__init__.py

"""This is the main file that loads/unloads the Python part of the plugin."""

# =============================================================================
# Source Python
# Copyright (C) 2012-2015 Source Python Development Team.  All rights reserved.
# =============================================================================

# This program is free software; you can redistribute it and/or modify it
# under the terms of the GNU General Public License, version 3.0, as
# published by the Free Software Foundation.

# This program is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
# FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
# more details.

# You should have received a copy of the GNU General Public License along
# with this program.  If not, see <http://www.gnu.org/licenses/>.

# As a special exception, the Source.Python Team gives you permission
# to link the code of this program (as well as its derivative works) to
# "Half-Life 2," the "Source Engine," and any Game MODs that run on software
# by the Valve Corporation.  You must obey the GNU General Public License in
# all respects for all other code used.  Additionally, the Source.Python
# Development Team grants this exception to all derivative works.


# =============================================================================
# >> FILE ACCESS LOGGER
# =============================================================================
# If True, all calls to open() with a path to a Source.Python data file will be
# logged in ../logs/file_access.log. The log entry will contain the file that
# is being accessed and a full stack trace. The logger will be removed as soon
# as setup_data_update() is called.
# This is a debug option to ensure that no data files are being accessed before
# the data has been updated. Release builds should have this option set to
# False.
LOG_FILE_OPERATIONS = False

if LOG_FILE_OPERATIONS:
    import builtins
    import traceback
    from paths import SP_DATA_PATH
    from paths import LOG_PATH

    LOG_FILE = LOG_PATH / 'file_access.log'

    # Clear log file
    LOG_FILE.open('w').close()

    old_open = builtins.open

    def new_open(f, *args, **kwargs):
        if isinstance(f, str) and f.startswith(SP_DATA_PATH):
            print(f)
            with LOG_FILE.open('a') as log_f:
                log_f.write('File access: {}\n'.format(f))
                traceback.print_stack(file=log_f)
                log_f.write('\n\n')

        return old_open(f, *args, **kwargs)

    builtins.open = new_open


# =============================================================================
# >> IMPORTS
# =============================================================================
# Python Imports
#   Context
from contextlib import suppress

# Source.Python Imports
#   Loggers
from loggers import _sp_logger  # It's save to import this here


# =============================================================================
# >> LOAD & UNLOAD
# =============================================================================
def load():
    """Load Source.Python's Python side."""
    setup_stdout_redirect()
    setup_core_settings()
    setup_logging()
    setup_exception_hooks()
    setup_data_update()
    setup_translations()
    setup_data()
    setup_global_pointers()
    setup_sp_command()
    setup_auth()
    setup_user_settings()
    setup_entities_listener()
    setup_run_command_listener()
    setup_versioning()
    setup_sqlite()


def unload():
    """Unload Source.Python's Python side."""
    unload_plugins()
    remove_entities_listener()
    unload_auth()


# =============================================================================
# >> DATA UPDATE
# =============================================================================
def setup_data_update():
    """Setup data update."""
    _sp_logger.log_debug('Setting up data update...')

    if LOG_FILE_OPERATIONS:
        builtins.open = old_open

    from core.settings import _core_settings

    if not _core_settings.auto_data_update:
        _sp_logger.log_debug('Automatic data updates are disable.')
        return

    _sp_logger.log_info('Checking for data updates...')

    from core.update import is_new_data_available, update_data
    from translations.manager import language_manager

    try:
        if is_new_data_available():
            _sp_logger.log_info('New data is available. Downloading...')
            update_data()

            # languages.ini is loaded before the data has been updated. Thus,
            # we need to reload the file.
            language_manager.reload()
        else:
            _sp_logger.log_info('No new data is available.')
    except:
        _sp_logger.log_exception(
            'An error occured during the data update.', exc_info=True)

def setup_data():
    """Setup data."""
    _sp_logger.log_debug('Setting up data...')

    from core import GameConfigObj
    from memory.manager import manager
    from paths import SP_DATA_PATH

    import players
    players.BaseClient = manager.create_type_from_dict(
        'BaseClient',
        GameConfigObj(SP_DATA_PATH / 'client' / 'CBaseClient.ini'))

    import listeners
    listeners.BaseEntityOutput = manager.create_type_from_dict(
        'BaseEntityOutput',
        GameConfigObj(SP_DATA_PATH / 'entity_output' / 'CBaseEntityOutput.ini'))

    try:
        _fire_output = listeners.BaseEntityOutput.fire_output
    except AttributeError:
        from warnings import warn
        warn(
            'BaseEntityOutput.fire_output not found. '
            'OnEntityOutput listener will not fire.'
        )
    else:
        _fire_output.add_pre_hook(listeners._pre_fire_output)


# =============================================================================
# >> CORE SETTINGS
# =============================================================================
def setup_core_settings():
    """Setup core settings."""
    _sp_logger.log_debug('Setting up core settings...')

    from core.settings import _core_settings
    _core_settings.load()


# =============================================================================
# >> LOGGING
# =============================================================================
def setup_logging():
    """Set up logging."""
    _sp_logger.log_debug('Setting up logging...')

    from configobj import ConfigObjError
    from cvars import ConVar

    # Use try/except in case the logging values are not integers
    try:

        # Import the core settings dictionary
        from core.settings import _core_settings

        # Set the logging level
        ConVar('sp_logging_level').set_int(
            int(_core_settings['LOG_SETTINGS']['level']))

        # Set the logging areas
        ConVar('sp_logging_areas').set_int(
            int(_core_settings['LOG_SETTINGS']['areas']))

    # Was an exception raised?
    except (ValueError, ConfigObjError):

        # Set the logging level to max (5)
        ConVar('sp_logging_level').set_int(5)

        # Set the logging area to include console, SP logs, and main log
        ConVar('sp_logging_areas').set_int(7)

        # Log a message about the value
        _sp_logger.log_message(
            '[Source.Python] Plugin did not load properly ' +
            'due to the following error:')

        # Re-raise the error
        raise


# =============================================================================
# >> HOOKS
# =============================================================================
def setup_exception_hooks():
    """Set up hooks."""
    _sp_logger.log_debug('Setting up exception hooks...')

    from hooks.exceptions import except_hooks
    from hooks.warnings import warning_hooks


# =============================================================================
# >> TRANSLATIONS
# =============================================================================
def setup_translations():
    """Set up translations."""
    _sp_logger.log_debug('Setting up translations...')

    # Import the Language Manager
    from translations.manager import language_manager
    from core.settings import _core_settings

    # Set the default language
    language_manager._register_default_language(
        _core_settings['BASE_SETTINGS']['language'])


# =============================================================================
# >> GLOBAL POINTERS
# =============================================================================
def setup_global_pointers():
    """Set up global pointers."""
    _sp_logger.log_debug('Setting up global pointers...')

    import sys
    from warnings import warn
    from core import GameConfigObj
    from memory.manager import manager
    from paths import SP_DATA_PATH

    manager.create_global_pointers_from_file(GameConfigObj(
        SP_DATA_PATH / 'memory' / 'global_pointers.ini'))

    _sp_logger.log_debug('Setting up global "server" variables...')
    from engines import server
    try:
        server.server = server.engine_server.server
    except NotImplementedError:
        try:
            server.server = manager.get_global_pointer('Server')
        except NameError:
            warn(str(sys.exc_info()[1]))

    _sp_logger.log_debug('Setting up global "factory_dictionary" variables...')
    from entities import factories
    try:
        from _entities._factories import factory_dictionary
    except ImportError:
        try:
            factory_dictionary = manager.get_global_pointer(
                'EntityFactoryDictionary'
            )
        except NameError:
            warn(str(sys.exc_info()[1]))
            return

    factories.factory_dictionary = factory_dictionary


# =============================================================================
# >> SP COMMAND
# =============================================================================
def setup_sp_command():
    """Set up the 'sp' command."""
    _sp_logger.log_debug('Setting up the "sp" command...')

    from core.command import auth, docs, dump, plugin


# =============================================================================
# >> AUTH
# =============================================================================
def setup_auth():
    """Set up authentification."""
    _sp_logger.log_debug('Setting up auth...')

    from auth.manager import auth_manager
    auth_manager.load()

def unload_auth():
    """Unload authentification."""
    _sp_logger.log_debug('Unloading auth...')

    from auth.manager import auth_manager
    auth_manager.unload()


# =============================================================================
# >> USER_SETTINGS
# =============================================================================
def setup_user_settings():
    """Set up user settings."""
    _sp_logger.log_debug('Setting up user settings...')

    from commands.client import client_command_manager
    from commands.say import say_command_manager
    from settings.menu import _player_settings
    from core.settings import _core_settings

    # Are there any private user settings say commands?
    if _core_settings['USER_SETTINGS']['private_say_commands']:

        # Register the private user settings say commands
        say_command_manager.register_commands(_core_settings[
            'USER_SETTINGS']['private_say_commands'].split(
            ','), _player_settings._private_send_menu)

    # Are there any public user settings say commands?
    if _core_settings['USER_SETTINGS']['public_say_commands']:

        # Register the public user settings say commands
        say_command_manager.register_commands(_core_settings[
            'USER_SETTINGS']['public_say_commands'].split(
            ','), _player_settings._send_menu)

    # Are there any client user settings commands?
    if _core_settings['USER_SETTINGS']['client_commands']:

        # Register the client user settings commands
        client_command_manager.register_commands(_core_settings[
            'USER_SETTINGS']['client_commands'].split(
            ','), _player_settings._send_menu)


# =============================================================================
# >> ENTITIES LISTENER
# =============================================================================
def setup_entities_listener():
    """Set up entities listener."""
    _sp_logger.log_debug('Setting up entities listener...')

    import sys
    from warnings import warn
    from _core import _sp_plugin
    from memory.manager import manager

    try:
        manager.get_global_pointer('GlobalEntityList').add_entity_listener(
            _sp_plugin
        )
    except NameError:
        warn(str(sys.exc_info()[1]))


def remove_entities_listener():
    """Remove entities listener."""
    _sp_logger.log_debug('Removing entities listener...')

    from _core import _sp_plugin
    from memory.manager import manager

    with suppress(NameError):
        manager.get_global_pointer('GlobalEntityList').remove_entity_listener(
            _sp_plugin)


# =============================================================================
# >> RUN COMMAND LISTENER
# =============================================================================
def setup_run_command_listener():
    """Set up the run command listener."""
    _sp_logger.log_debug('Setting up run command listener...')

    # This is done here to fix a cyclic import
    import listeners._run_command


# =============================================================================
# >> PLUGINS
# =============================================================================
def unload_plugins():
    """Unload all plugins."""
    _sp_logger.log_debug('Unloading plugins...')

    from plugins.manager import plugin_manager

    for plugin in plugin_manager.loaded_plugins:
        plugin.unload()


# =============================================================================
# >> VERSION
# =============================================================================
def setup_versioning():
    """Setup versioning."""
    _sp_logger.log_debug('Setting up versioning...')

    from core import version


# =============================================================================
# >> SQLITE3
# =============================================================================
def setup_sqlite():
    """Pre-load libsqlite3.so.0 on Linux."""
    from core import PLATFORM
    if PLATFORM != 'linux':
        return

    _sp_logger.log_debug('Pre-loading libsqlite3.so.0...')

    import ctypes
    from paths import BASE_PATH

    # This is required, because some systems don't have the required sqlite
    # version installed. This fixes the issue by loading the library into the
    # memory using its absolute path.
    # Using RPATH might be a better solution, but I don't get it working...
    ctypes.cdll.LoadLibrary(BASE_PATH / 'Python3/plat-linux/libsqlite3.so.0')


# =============================================================================
# >> STDOUT
# =============================================================================
def setup_stdout_redirect():
    """Setup sys.stdout redirect."""
    import sys

    # The idea is to always redirect sys.stdout, because this allows us to use
    # print(), which will also print the output to the client console, if it
    # has been triggered via RCON or on a listen server. However, the downside
    # of this is that we can't flush the console anymore, which is quite
    # useful in some cases (e.g. generating the wiki).
    # Thus, we only redirect sys.stdout if it's None for now, which only seems
    # to happen on Windows 10. Otherwise, print() wouldn't output anything.
    # See also:
    # https://github.com/Source-Python-Dev-Team/Source.Python/issues/151
    # https://github.com/Source-Python-Dev-Team/Source.Python/issues/175
    # https://github.com/Source-Python-Dev-Team/Source.Python/issues/193
    if sys.stdout is not None and sys.stderr is not None:
        return

    _sp_logger.log_debug('Setting up sys.stdout/sys.stderr redirect...')

    from core import console_message

    class OutputRedirect(object):
        def write(self, data):
            console_message(data)
            return len(data)

        def flush(self):
            # We can't flush anymore...
            pass

    if sys.stdout is None:
        sys.stdout = OutputRedirect()

    if sys.stderr is None:
        sys.stderr = OutputRedirect()

    from engines.server import engine_server

    if not engine_server.is_dedicated_server():
        # Return here for listen servers, because we only want to see the
        # warning if reconnecting the output streams failed, which is only
        # done on dedicated servers. For listen servers creating OutputRedirect
        # instances is the proper fix.
        return

    from warnings import warn
    warn(
        'sys.stdout and/or sys.stderr is None. All data will be redirected through '
        'core.console_message() instead. If you receive this warning, please '
        'notify us and tell us your operating system, game and Source.Python '
        'version. The information can be posted here: '
        'https://github.com/Source-Python-Dev-Team/Source.Python/issues/175. '
        'Source.Python should continue working, but we would like to figure '
        'out in which situations sys.stdout is None to be able to fix this '
        'issue instead of applying a workaround.')
