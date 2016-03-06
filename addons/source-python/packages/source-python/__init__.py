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
# >> IMPORTS
# =============================================================================
# Source.Python Imports
#   Loggers
from loggers import _sp_logger  # It's save to import this here


# =============================================================================
# >> LOAD & UNLOAD
# =============================================================================
def load():
    """Load Source.Python's Python side."""
    setup_logging()
    setup_hooks()
    setup_translations()
    setup_global_pointers()
    setup_sp_command()
    setup_auth()
    setup_user_settings()
    setup_entities_listener()
    setup_versioning()
    setup_sqlite()


def unload():
    """Unload Source.Python's Python side."""
    unload_plugins()
    remove_entities_listener()


# =============================================================================
# >> HOOKS
# =============================================================================
def setup_hooks():
    """Set up hooks."""
    _sp_logger.log_debug('Setting up hooks...')

    from hooks.exceptions import except_hooks
    from hooks.warnings import warning_hooks


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
        server.server = manager.get_global_pointer('Server')


# =============================================================================
# >> SP COMMAND
# =============================================================================
def setup_sp_command():
    """Set up the 'sp' command."""
    _sp_logger.log_debug('Setting up the "sp" command...')

    from core.command import _core_command


# =============================================================================
# >> AUTH
# =============================================================================
def setup_auth():
    """Set up authentification."""
    _sp_logger.log_debug('Setting up auth...')

    from core.command import _core_command
    from core.settings import _core_settings

    # Get the auth providers that should be loaded
    auth_providers = _core_settings['AUTH_SETTINGS']['providers'].split()

    # Should any providers be loaded?
    if auth_providers:

        # Load the auth providers
        _core_command['auth'].call_command(['load'] + auth_providers)


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

    from _core import _sp_plugin
    from memory.manager import manager

    manager.get_global_pointer('GlobalEntityList').add_entity_listener(
        _sp_plugin)


def remove_entities_listener():
    """Remove entities listener."""
    _sp_logger.log_debug('Removing entities listener...')

    from _core import _sp_plugin
    from memory.manager import manager

    manager.get_global_pointer('GlobalEntityList').remove_entity_listener(
        _sp_plugin)


# =============================================================================
# >> PLUGINS
# =============================================================================
def unload_plugins():
    """Unload all plugins."""
    _sp_logger.log_debug('Unloading plugins...')

    from core.manager import core_plugin_manager
    from core.command import _core_command

    for plugin_name in core_plugin_manager:
        _core_command.unload_plugin(plugin_name)


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
