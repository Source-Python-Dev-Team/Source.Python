# ../settings/types.py

"""Contains user setting types."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Python
from collections import OrderedDict
from contextlib import suppress

# Source.Python
from cvars import ConVar
from engines.server import engine_server
from menus import PagedMenu
from menus import PagedOption
from messages import SayText
from players.helpers import uniqueid_from_index
from settings import _settings_strings
from settings.storage import _player_settings_storage


# =============================================================================
# >> GLOBAL VARIABLES
# =============================================================================
_message = SayText(message=_settings_strings['Chosen'])


# =============================================================================
# >> CLASSES
# =============================================================================
class SettingsType(object):
    """Class used to store settings with possible values."""

    prefix = ''

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
        self.name = name
        self.default = default
        self.text = text

        # Store a menu for the object
        self.menu = PagedMenu(
            select_callback=self._chosen_value,
            build_callback=self._menu_build,
            title=name if text is None else text,
            description=_settings_strings['Description'])

        # Return the instance
        return self

    @property
    def convar(self):
        """Return the convar name of the setting.

        :rtype: str
        """
        return self.prefix + self.name.lower().replace(' ', '_')

    @property
    def _type(self):
        """The type of object to cast values to for the class."""
        raise NotImplementedError('No _type defined for class.')

    def _is_valid_setting(self, value):
        """Return whether or not the given value is valid."""
        raise NotImplementedError('No _is_valid_setting defined for class.')

    def get_setting(self, index):
        """Return the setting value for the given player index."""
        # Get the client's convar value
        value = engine_server.get_client_convar_value(index, self.convar)

        # Try to typecast the value, suppressing ValueErrors
        with suppress(ValueError):

            # Typecast the given value
            value = self._typecast_value(value)

            # Is the given value a proper one for the convar?
            if self._is_valid_setting(value):

                # If so, return the value
                return value

        # Get the client's uniqueid
        uniqueid = uniqueid_from_index(index)

        # Is the uniqueid in the setting's storage dictionary?
        if uniqueid in _player_settings_storage:

            # Is the convar in the clients's dictionary?
            if self.convar in _player_settings_storage[uniqueid]:

                # Get the client's value for the convar
                value = _player_settings_storage[uniqueid][self.convar]

                # Try to typecast the value, suppressing ValueErrors
                with suppress(ValueError):

                    # Typecast the given value
                    value = self._typecast_value(value)

                    # Is the given value a proper one for the convar?
                    if self._is_valid_setting(value):

                        # Return the value
                        return value

        # Return the default value
        if isinstance(self.default, ConVar):
            return self._typecast_default_convar()
        return self.default

    def _typecast_value(self, value):
        """Cast the value to the proper type."""
        return self._type(value)

    def _typecast_default_convar(self):
        """Return the type-casted value of the default ConVar."""
        return getattr(
            self.default,
            'get_{value_type}'.format(
                value_type=str(self._type)
            )
        )()

    def _menu_build(self, menu, index):
        """Set the default value in the menu description."""
        self.menu.description.tokens = {'value': self.get_setting(index)}

    def _chosen_value(self, menu, index, option):
        """Store the player's chosen value for the setting."""
        # Get the client's uniqueid
        uniqueid = uniqueid_from_index(index)

        # Set the player's setting
        _player_settings_storage[uniqueid][self.convar] = option.value

        # Send the player a message about their changed setting
        _message.send(index, convar=self.convar, value=option.value)


class _NumericalSetting(SettingsType):
    """Class used to store integer/float settings with min/max values."""

    def __init__(
            self, name, default, text=None, min_value=None, max_value=None):
        """Store the base attributes on instantiation."""
        self.min = self._type(min_value) if min_value is not None else None
        self.max = self._type(max_value) if max_value is not None else None

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


class FloatSetting(_NumericalSetting):
    """Class used to store float value settings."""

    _type = float


class IntegerSetting(_NumericalSetting):
    """Class used to store integer value settings."""

    _type = int


class BoolSetting(SettingsType):
    """Class used to store boolean value settings."""

    _type = bool

    def _typecast_value(self, value):
        """Cast the given value to a boolean."""
        try:
            value = int(value)
            return bool(value)
        except ValueError:
            return ''

    def _is_valid_setting(self, value):
        """Return whether the given value is a valid boolean value."""
        return isinstance(value, self._type)


class StringSetting(SettingsType):
    """Class used to store string value settings with available options."""

    _type = str

    def __init__(self, name, default, text=None):
        """Store the base attributes on instatiation."""
        self.options = OrderedDict()

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
