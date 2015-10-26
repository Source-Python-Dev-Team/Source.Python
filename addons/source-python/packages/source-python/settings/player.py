# ../settings/player.py

"""Provides access to user settings functionality."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Python Imports
#   Collections
from collections import OrderedDict

# Source.Python Imports
#   Core
from core import AutoUnload
#   Menus
from menus import PagedMenu
from menus import PagedOption
#   Settings
from settings.menu import _player_settings
from settings.types import _SettingsType
from settings.types import _FloatSetting
from settings.types import _IntegerSetting
from settings.types import _StringSetting


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('PlayerSettings',
           )


# =============================================================================
# >> CLASSES
# =============================================================================
class _SettingsDictionary(OrderedDict):
    """Class used to store user settings."""

    def __init__(self, name, text=None):
        """Verify the name value and stores base attributes."""
        # Is the given name a proper value for a convar?
        if not name.replace('_', '').replace(' ', '').isalpha():

            # Raise an error
            raise ValueError(
                'Given name "{0}" is not valid'.format(name))

        # Set the base attributes
        self._name = name
        self._text = text

        # Create the instance's menu
        self._menu = PagedMenu(
            select_callback=self._chosen_item,
            title=name if text is None else text)

        # Call the super class' __init__ to initialize the OrderedDict
        super().__init__()

    def __setitem__(self, item, value):
        """Validate the given value and its type before setting the item."""
        # Is the given value a proper type?
        if not isinstance(value, (_SettingsDictionary, _SettingsType)):

            # Raise an error
            raise ValueError(
                'Given value "{0}" is not valid'.format(value))

        # Is the item already in the dictionary?
        if item in self:

            # Raise an error
            raise ValueError(
                'Given item "{0}" is already registered'.format(item))

        # Set the item in the dictionary
        super().__setitem__(item, value)

        # Get the new object
        value = self[item]

        # Set the item's prefix
        value._prefix = self.prefix + '_'

        # Does the section's name need added to the prefix?
        if not isinstance(self, PlayerSettings):

            # Add the section's name to the prefix
            value._prefix += self.name.lower().replace(' ', '_') + '_'

        # Add the option to the menu
        self.menu.append(PagedOption(
            value.name if value.text is None else value.text, value))

    @property
    def name(self):
        """Return the name of the _SettingsDictionary instance."""
        return self._name

    @property
    def text(self):
        """Return the text of the _SettingsDictionary instance."""
        return self._text

    @property
    def prefix(self):
        """Return the prefix of the _SettingsDictionary instance."""
        return self._prefix

    @property
    def menu(self):
        """Return the instance's menu object."""
        return self._menu

    def add_float_setting(
            self, name, default, text=None, min_value=None, max_value=None):
        """Add a new float setting to the dictionary."""
        # Add the new float setting to the dictionary
        self[name] = _FloatSetting(name, default, text, min_value, max_value)

        # Return the setting
        return self[name]

    def add_int_setting(
            self, name, default, text=None, min_value=None, max_value=None):
        """Add a new integer setting to the dictionary."""
        # Add the new integer setting to the dictionary
        self[name] = _IntegerSetting(name, default, text, min_value, max_value)

        # Return the setting
        return self[name]

    def add_string_setting(self, name, default, text=None):
        """Add a new string setting to the dictionary."""
        # Add the new string setting to the dictionary
        self[name] = _StringSetting(name, default, text)

        # Return the setting
        return self[name]

    def add_section(self, name, text=None):
        """Add a new section to the dictionary."""
        # Add the new section to the dictionary
        self[name] = _SettingsDictionary(name, text)

        # Return the section
        return self[name]

    @staticmethod
    def _chosen_item(menu, index, option):
        """Called when an item is chosen from the instance's menu."""
        # Is the chosen value another branch of settings?
        if isinstance(option.value, _SettingsDictionary):

            # Send the new menu
            option.value.menu.send(index)

            # No need to go further
            return

        # TODO: Placeholder for sending setting specific menus
        option.value.menu.send(index)


class PlayerSettings(AutoUnload, _SettingsDictionary):
    """Class used to register user settings."""

    def __init__(self, name, prefix, text=None):
        """Verify the given values and store the instance."""
        # Is the given name already registered?
        if name in _player_settings:

            # Raise an error
            raise ValueError(
                'Given name "{0}" is already registered.'.format(name))

        # Is the prefix a proper value?
        if not prefix.replace('_', '').isalpha():

            # Raise an error
            raise ValueError(
                'Given prefix "{0}" is not valid'.format(prefix))

        # Call the super class' __init__ to verify
        # the name and store base attributes
        super().__init__(name, text)

        # Set the prefix for the settings
        self._prefix = prefix.lower()

        # Add the instance to the main dictionary
        _player_settings[name] = self

        # Add the settings instance to the main settings menu
        _player_settings.menu.append(
            PagedOption(name if text is None else text, self))

    def unregister_settings(self):
        """Unregister the given settings from the dictionary."""
        del _player_settings[self.name]

    def _unload_instance(self):
        """Unregister the setting on unload."""
        self.unregister_settings()
