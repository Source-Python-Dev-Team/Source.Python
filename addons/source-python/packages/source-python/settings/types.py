# ../settings/types.py

# =============================================================================
# >> IMPORTS
# =============================================================================
# Python Imports
#   Collections
from collections import OrderedDict

# Source.Python Imports
from engines import EngineServer
#   Players
from players.helpers import playerinfo_from_index
from players.helpers import uniqueid_from_playerinfo
#   Settings
from settings.storage import _PlayerSettingsStorage


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
# Set all to an empty list
__all__ = []


# =============================================================================
# >> CLASSES
# =============================================================================
class _SettingsType(object):
    '''Class used to store settings with possible values'''

    def __new__(cls, name, default, *args):
        '''Verify the name and default value before getting the new object'''

        # Was a valid value passed as the name?
        if not name.replace('_', '').replace(' ', '').isalpha():

            # Raise an error
            raise ValueError(
                'Given name "{0}" is not valid'.format(name))

        # Is the given default value of the correct type?
        if type(default) != cls._type:

            # Raise an error
            raise ValueError(
                'Given value must be of "{0}" type, not "{1}"'.format(
                    cls._type.__name__, type(default).__name__))

        # Get the new object instance
        self = object.__new__(cls)

        # Return the instance
        return self

    @property
    def name(self):
        '''Returns the name of the setting'''
        return self._name

    @property
    def default(self):
        '''Returns the default value of the setting'''
        return self._default

    @property
    def text(self):
        '''Returns the text (to be used in the settings menu) of the setting'''
        return self._text

    @property
    def prefix(self):
        '''Returns the prefix of the setting'''
        return self._prefix

    def get_setting(self, index):
        '''Returns the setting value for the given player index'''

        # Get the convar's value
        convar = self.prefix + self.name.lower().replace(' ', '_')

        # Get the client's convar value
        value = EngineServer.get_client_convar_value(index, convar)

        # Use try/except to typecast the value
        try:

            # Typecast the given value
            value = self._type(value)

        # Was an exception encountered?
        except ValueError:

            # Do nothing if the error occurred
            pass

        # Was no exception encountered?
        else:

            # Is the given value a proper one for the convar?
            if self._is_valid_setting(value):

                # If so, return the value
                return value

        # Get the client's uniqueid
        uniqueid = uniqueid_from_playerinfo(playerinfo_from_index(index))

        # Is the uniqueid in the setting's storage dictionary?
        if uniqueid in _PlayerSettingsStorage:

            # Is the convar in the clients's dictionary?
            if convar in _PlayerSettingsStorage[uniqueid]:

                # Get the client's value for the convar
                value = _PlayerSettingsStorage[uniqueid][convar]

                # Use try/except to typecast the value
                try:

                    # Typecast the given value
                    value = self._type(value)

                # Was an exception encountered?
                except ValueError:

                    # Do nothing if the error occurred
                    pass

                # Was no exception encountered?
                else:

                    # Is the given value a proper one for the convar?
                    if self._is_valid_setting(value):

                        # Return the value
                        return value

        # Return the default value
        return self.default


class _NumericalSetting(_SettingsType):
    '''Class used to store integer/float settings with min/max values'''

    def __init__(self, name, default, text='', min_value=None, max_value=None):
        '''Stores the base attributes on instantiation'''
        self._name = name
        self._default = default
        self._text = text
        self._min = self._type(min_value) if not min_value is None else None
        self._max = self._type(max_value) if not max_value is None else None

    @property
    def min(self):
        '''Returns the min value for the setting'''
        return self._min

    @property
    def max(self):
        '''Returns the max value for the setting'''
        return self._max

    def _is_valid_setting(self, value):
        '''Returns whether the given value is a valid value for the setting'''

        # Is the given value less than the setting's min value?
        if not self.min is None and value < self.min:

            # If too low, return False
            return False

        # Is the given value more than the setting's max value?
        if not self.max is None and value > self.max:

            # If too high, return False
            return False

        # Return True if the value is within the proper range
        return True


class _FloatSetting(_NumericalSetting):
    '''Class used to store float value settings'''
    _type = float


class _IntegerSetting(_NumericalSetting):
    '''Class used to store integer value settings'''
    _type = int


class _StringSetting(_SettingsType):
    '''Class used to store string value settings with available options'''
    _type = str

    def __init__(self, name, default, text=''):
        '''Stores the base attributes on instatiation'''
        self._name = name
        self._default = default
        self._text = text
        self._options = OrderedDict()

    @property
    def options(self):
        '''Returns the options dictionary for the setting'''
        return self._options

    def add_option(self, name, text=''):
        '''Adds an option to the setting's dictionary'''

        # Is the given option already registered?
        if name in self.options:

            # Raise an error
            raise ValueError(
                'Given name "{0}" is already an option'.format(name))

        # Store the option
        self.options[name] = text if text else name

    def remove_option(self, name):
        '''Removes an option from the settings'''

        # Is the option registered?
        if not name in self.options:

            # Raise an error
            raise ValueError(
                'Given name "{0}" is not an option'.format(name))

        # Delete the option
        del self.options[name]

    def _is_valid_setting(self, value):
        '''Returns whether the given value is a valid value for the setting'''

        # Is the given value in the setting's options?
        if value in self.options:

            # If so, return True
            return True

        # If the given value is not in the options, return False
        return False
