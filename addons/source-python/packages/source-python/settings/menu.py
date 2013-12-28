# ../settings/menu.py

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
#   Players
from players.helpers import uniqueid_from_playerinfo
#   Settings
from settings.player import _MainSettingsDictionary
from settings.player import _SettingsDictionary


# =============================================================================
# >> CLASSES
# =============================================================================
class _SettingsMenu(object):
    ''''''

    def _private_command(self, *args):
        ''''''

        # Send the menu
        self._send_menu(*args)

        # Make the command private
        return False

    def _send_menu(self, playerinfo, *args):
        ''''''

        for name in _MainSettingsDictionary:
            for setting in self._get_settings(_MainSettingsDictionary[name]):
                print(setting)

    def _get_setting(self, settings):
        for setting in settings:
            if isinstance(setting, _SettingsDictionary):
                self._get_settings(setting)
            else:
                yield setting

_SettingsMenuInstance = _SettingsMenu()
