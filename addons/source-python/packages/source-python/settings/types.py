# ../settings/types.py

"""Contains user setting types."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Python Imports
#   Collections
from collections import OrderedDict
#   Contextlib
from contextlib import suppress

# Source.Python Imports
#   Engines
from engines.server import engine_server
#   Menus
from menus import PagedMenu
from menus import PagedOption
#   Messages
from messages import SayText
#   Players
from players.helpers import playerinfo_from_index
from players.helpers import uniqueid_from_playerinfo
#   Settings
from settings import _settings_strings
from settings.storage import _player_settings_storage


# =============================================================================
# >> GLOBAL VARIABLES
# =============================================================================
_message = SayText(message=_settings_strings['Chosen'])


# =============================================================================
# >> CLASSES
# =============================================================================
class _SettingsType(object):
    """Class used to store settings with possible values."""

    def __new__(cls, name, default, text=None, *args):
        """Verify the name and default value before getting the new object."""
        # Was a valid value passed as the name?
        if not name.replace('_', '').replace(' ', '').isalpha():

            # Raise an error
            raise ValueError(
                'Given name "{0}" is not valid'.format(name))

        # Is the given default value of the correct type?
        if not isinstance(default, cls._type):

            # Raise an error
            raise ValueError(
                'Given value must be of "{0}" type, not "{1}"'.format(
                    cls._type.__name__, type(default).__name__))

        # Get the new object instance
        self = object.__new__(cls)

        # Store the base attributes
        self._name = name
        self._default = default
        self._text = text

        # Store a menu for the object
        self._menu = PagedMenu(
            select_callback=self._chosen_value,
            build_callback=self._menu_build,
            title=name if text is None else text,
            description=_settings_strings['Description'])

        # Return the instance
        return self

    @property
    def name(self):
        """Return the name of the setting."""
        return self._name

    @property
    def default(self):
        """Return the default value of the setting."""
        return self._default

    @property
    def text(self):
        """Return the setting's text."""
        return self._text

    @property
    def prefix(self):
        """Return the prefix of the setting."""
        return self._prefix

    @property
    def convar(self):
        """Return the convar name of the setting."""
        return self.prefix + self.name.lower().replace(' ', '_')

    @property
    def menu(self):
        """Return the setting's menu."""
        return self._menu

    def get_setting(self, index):
        """Return the setting value for the given player index."""
        # Get the client's convar value
        value = engine_server.get_client_convar_value(index, self.convar)

        # Try to typecast the value, suppressing ValueErrors
        with suppress(ValueError):

            # Typecast the given value
            value = self._type(value)

            # Is the given value a proper one for the convar?
            if self._is_valid_setting(value):

                # If so, return the value
                return value

        # Get the client's uniqueid
        uniqueid = uniqueid_from_playerinfo(playerinfo_from_index(index))

        # Is the uniqueid in the setting's storage dictionary?
        if uniqueid in _player_settings_storage:

            # Is the convar in the clients's dictionary?
            if self.convar in _player_settings_storage[uniqueid]:

                # Get the client's value for the convar
                value = _player_settings_storage[uniqueid][self.convar]

                # Try to typecast the value, suppressing ValueErrors
                with suppress(ValueError):

                    # Typecast the given value
                    value = self._type(value)

                    # Is the given value a proper one for the convar?
                    if self._is_valid_setting(value):

                        # Return the value
                        return value

        # Return the default value
        return self.default

    def _menu_build(self, menu, index):
        """Set the default value in the menu description."""
        self.menu.description.tokens = {'value': self.get_setting(index)}

    def _chosen_value(self, menu, index, option):
        """Store the player's chosen value for the setting."""
        # Get the client's uniqueid
        uniqueid = uniqueid_from_playerinfo(playerinfo_from_index(index))

        # Set the player's setting
        _player_settings_storage[uniqueid][self.convar] = option.value

        # Send the player a message about their changed setting
        _message.send(index, convar=self.convar, value=option.value)


class _NumericalSetting(_SettingsType):
    """Class used to store integer/float settings with min/max values."""

    def __init__(
            self, name, default, text=None, min_value=None, max_value=None):
        """Store the base attributes on instantiation."""
        self._min = self._type(min_value) if min_value is not None else None
        self._max = self._type(max_value) if max_value is not None else None

    @property
    def min(self):
        """Return the min value for the setting."""
        return self._min

    @property
    def max(self):
        """Return the max value for the setting."""
        return self._max

    def _is_valid_setting(self, value):
        """Return whether the given value is a valid value for the setting."""
        # Is the given value less than the setting's min value?
        if self.min is not None and value < self.min:

            # If too low, return False
            return False

        # Is the given value more than the setting's max value?
        if self.max is not None and value > self.max:

            # If too high, return False
            return False

        # Return True if the value is within the proper range
        return True


class _FloatSetting(_NumericalSetting):
    """Class used to store float value settings."""

    _type = float


class _IntegerSetting(_NumericalSetting):
    """Class used to store integer value settings."""

    _type = int


class _StringSetting(_SettingsType):
    """Class used to store string value settings with available options."""

    _type = str

    def __init__(self, name, default, text=None):
        """Store the base attributes on instatiation."""
        self._options = OrderedDict()

    @property
    def options(self):
        """Return the options dictionary for the setting."""
        return self._options

    def add_option(self, name, text=None):
        """Add an option to the setting's dictionary."""
        # Is the given option already registered?
        if name in self.options:

            # Raise an error
            raise ValueError(
                'Given name "{0}" is already an option'.format(name))

        # Store the option
        option = self.options[name] = PagedOption(
            name if text is None else text, name)

        # Add the option to the menu
        self.menu.append(option)

    def remove_option(self, name):
        """Remove an option from the settings."""
        # Is the option registered?
        if name not in self.options:

            # Raise an error
            raise ValueError(
                'Given name "{0}" is not an option'.format(name))

        # Delete the option
        del self.options[name]

    def _is_valid_setting(self, value):
        """Return whether the given value is a valid value for the setting."""
        # Is the given value in the setting's options?
        if value in self.options:

            # If so, return True
            return True

        # If the given value is not in the options, return False
        return False
