# ../__init__.py

'''
    ===========================================================================
    Source Python
    Copyright (C) 2012 Source Python Development Team.  All rights reserved.
    ===========================================================================

    This program is free software; you can redistribute it and/or modify it
    under the terms of the GNU General Public License, version 3.0, as
    published by the Free Software Foundation.

    This program is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
    more details.

    You should have received a copy of the GNU General Public License along
    with this program.  If not, see <http://www.gnu.org/licenses/>.

    As a special exception, the Source.Python Team gives you permission
    to link the code of this program (as well as its derivative works) to
    "Half-Life 2," the "Source Engine," and any Game MODs that run on software
    by the Valve Corporation.  You must obey the GNU General Public License in
    all respects for all other code used.  Additionally, the Source.Python
    Development Team grants this exception to all derivative works.
'''

# =============================================================================
# >> IMPORTS
# =============================================================================
# Python Imports
from configobj import ConfigObjError

# Source.Python Imports
from cvar_c import CConVar
#   ExceptHooks - this is done to register sys.excepthook
from excepthooks import ExceptHooks


# =============================================================================
# >> LOGGING SETUP
# =============================================================================
# Use try/except in case the logging values are not integers
try:

    # Import the core settings dictionary
    from _core.settings import _CoreSettingsInstance

    # Set the logging level
    CConVar.get('sp_logging_level').set_int(
        int(_CoreSettingsInstance['LOG_SETTINGS']['level']))

    # Set the logging areas
    CConVar.get('sp_logging_areas').set_int(
        int(_CoreSettingsInstance['LOG_SETTINGS']['areas']))

# Was an exception raised?
except (ValueError, ConfigObjError):

    # Set the logging level to max (5)
    CConVar.get('sp_logging_level').set_int(5)

    # Set the logging area to include console, SP logs, and main log
    CConVar.get('sp_logging_areas').set_int(7)

    # Import the _SPLogger
    from loggers import _SPLogger

    # Log a message about the value
    _SPLogger.log_message(
        '[Source.Python] Plugin did not load properly ' +
        'due to the following error:')

    # Re-raise the error
    raise


# =============================================================================
# >> TRANSLATIONS SETUP
# =============================================================================
# Import the Language Manager
from translations.manager import LanguageManager

# Set the default language
LanguageManager._register_default_language(
    _CoreSettingsInstance['BASE_SETTINGS']['language'])


# =============================================================================
# >> INITIALIZE SP COMMAND
# =============================================================================
from _core.command import SPSubCommandManager


# =============================================================================
# >> AUTH SETUP
# =============================================================================
# Get the auth providers that should be loaded
auth_providers = _CoreSettingsInstance['AUTH_SETTINGS']['providers'].split()

# Should any providers be loaded?
if auth_providers:

    # Load the auth providers
    SPSubCommandManager.call_command('auth', ['load'] + auth_providers)


# =============================================================================
# >> USER_SETTINGS SETUP
# =============================================================================
from settings.menu import _SettingsMenuInstance

# Are there any private user settings say commands?
if _CoreSettingsInstance['USER_SETTINGS']['private_say_commands']:

    # Register the private user settings say commands
    SayCommandManager.register_commands(_CoreSettingsInstance[
        'USER_SETTINGS']['private_say_commands'].split(
        ','), _SettingsMenuInstance._private_command)

# Are there any public user settings say commands?
if _CoreSettingsInstance['USER_SETTINGS']['public_say_commands']:

    # Register the public user settings say commands
    SayCommandManager.register_commands(_CoreSettingsInstance[
        'USER_SETTINGS']['public_say_commands'].split(
        ','), _SettingsMenuInstance._send_menu)

# Are there any client user settings commands?
if _CoreSettingsInstance['USER_SETTINGS']['client_commands']:

    # Register the client user settings commands
    ClientCommandManager.register_commands(_CoreSettingsInstance[
        'USER_SETTINGS']['client_commands'].split(
        ','), _SettingsMenuInstance._send_menu)
