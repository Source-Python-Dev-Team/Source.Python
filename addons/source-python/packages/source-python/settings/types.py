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
from menus import PagedMenu, SimpleMenu
from menus import PagedOption, SimpleOption
from messages import SayText
from players.entity import Player
from players.helpers import uniqueid_from_index
from settings import _settings_strings
from settings.storage import _player_settings_storage
from translations.strings import TranslationStrings


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

        # Return the instance
        return self

    def __init__(self, name, default, text=None):
        """Store the base attributes and create the menu."""
        self.name = name
        self.default = default
        self.text = text
        self._create_menu()

    def _create_menu(self):
        self.menu = PagedMenu(
            select_callback=self._chosen_value,
            build_callback=self._menu_build,
            title=self.name if self.text is None else self.text,
            description=_settings_strings['Description']
        )

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
        if Player(index).is_fake_client():
            return self._get_default_value()

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
        return self._get_default_value()

    def _get_default_value(self):
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
        # Set the player's setting
        uniqueid = uniqueid_from_index(index)
        _player_settings_storage[uniqueid][self.convar] = option.value
        self._send_chosen_message(index, option.value)

    def _send_chosen_message(self, index, value):
        """Send message about choice made for setting change."""
        _message.send(index, convar=self.convar, value=value)


class IntegerSetting(SettingsType):
    """Class used to store integer value settings."""

    _type = int

    def __init__(
        self, name, default, text=None, min_value=None, max_value=None
    ):
        """Store the base attributes on instantiation."""
        super().__init__(name, default, text)
        if min_value is not None and max_value is not None:
            if min_value >= max_value:
                raise ValueError(
                    'min_value ({min_value}) must be less than max_value '
                    '({max_value}).'.format(
                        min_value=min_value,
                        max_value=max_value,
                    )
                )
        self.min = self._type(min_value) if min_value is not None else None
        self.max = self._type(max_value) if max_value is not None else None
        self.current_values = {}

    def _create_menu(self):
        self.menu = SimpleMenu(
            select_callback=self._chosen_value,
            build_callback=self._menu_build,
        )

    def _menu_build(self, menu, index):
        """Build the menu."""
        self.menu.clear()
        self.menu.append(self.name if self.text is None else self.text)

        # Get the player's information
        player = Player(index)
        uniqueid = player.uniqueid
        current_value = self.get_setting(index)

        description = _settings_strings['Description'].get_string(
            language=player.language,
            value=current_value,
        )
        self.menu.append(description)
        self.menu.append('-' * len(description))
        counter = 1
        if self.min is None or self.max is None or self.max - self.min > 100:
            for value in (100, -100):
                self._add_option(choice_index=counter, value=value)
                counter += 1

        if self.min is None or self.max is None or self.max - self.min > 10:
            for value in (10, -10):
                self._add_option(choice_index=counter, value=value)
                counter += 1

        for value in (1, -1):
            self._add_option(choice_index=counter, value=value)
            counter += 1

        if uniqueid not in self.current_values:
            self.current_values[uniqueid] = current_value

        self.menu.append(
            SimpleOption(
                choice_index=counter,
                text='Save ({current_value})'.format(
                    current_value=self.current_values[uniqueid],
                ),
                value='Save',
            )
        )

    def _add_option(self, choice_index, value):
        """Add the value to the menu."""
        self.menu.append(
            SimpleOption(
                choice_index=choice_index,
                text='{:+d}'.format(value),
                value=value,
            )
        )

    def _chosen_value(self, menu, index, option):
        """Store the player's chosen value for the setting."""
        uniqueid = uniqueid_from_index(index)

        if option.value == 'Save':
            value = self.current_values[uniqueid]

            # Set the player's setting
            _player_settings_storage[uniqueid][self.convar] = value
            self._send_chosen_message(index, value)
            del self.current_values[uniqueid]
            return

        new_value = self.current_values[uniqueid] + option.value
        if self.min is not None:
            new_value = max(new_value, self.min)
        if self.max is not None:
            new_value = min(new_value, self.max)
        self.current_values[uniqueid] = new_value
        self.menu.send()

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


class BoolSetting(SettingsType):
    """Class used to store boolean value settings."""

    _type = bool

    def __init__(self, name, default, text=None):
        super().__init__(name, default, text)
        for item in [True, False]:
            self.menu.append(
                PagedOption(
                    text=item,
                    value=item,
                )
            )

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
        super().__init__(name, default, text)
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
            text=name if text is None else text,
            value=name,
        )

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
        self.menu.remove(self.options[name])
        del self.options[name]

    def _is_valid_setting(self, value):
        """Return whether the given value is a valid value for the setting."""
        # Is the given value in the setting's options?
        if value in self.options:

            # If so, return True
            return True

        # If the given value is not in the options, return False
        return False

    def _send_chosen_message(self, index, value):
        """Send message about choice made for setting change."""
        value = self.options[value].text
        if isinstance(value, TranslationStrings):
            player = Player(index)
            value = value.get_string(player.language)
        _message.send(index, convar=self.convar, value=value)
