# ../settings/player.py

# =============================================================================
# >> IMPORTS
# =============================================================================
# Python Imports
#   Collections
from collections import OrderedDict

# Source.Python Imports
#   Settings
from settings.types import _SettingsType
from settings.types import _FloatSetting
from settings.types import _IntegerSetting
from settings.types import _StringSetting


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
# Add all the global variables to __all__
__all__ = [
    'PlayerSettings',
]


# =============================================================================
# >> GLOBAL VARIABLES
# =============================================================================
# Create a dictionary to store all of the user settings
_MainSettingsDictionary = dict()


# =============================================================================
# >> CLASSES
# =============================================================================
class _SettingsDictionary(OrderedDict):
    '''Class used to store user settings'''

    def __init__(self, name, text=''):
        '''
            Verifies the name value on instantiation and stores base attributes
        '''

        # Is the given name a proper value for a convar?
        if not name.replace('_', '').replace(' ', '').isalpha():

            # Raise an error
            raise ValueError(
                'Given name "{0}" is not valid'.format(name))

        # Set the base attributes
        self._name = name
        self._text = text

        # Call the super class' __init__ to initialize the OrderedDict
        super(_SettingsDictionary, self).__init__()

    def __setitem__(self, item, value):
        '''Override __setitem__ to verify the type of value given
            and that the item is not already in the dictionary'''

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
        super(_SettingsDictionary, self).__setitem__(item, value)

        # Set the item's prefix
        self[item]._prefix = self.prefix + '_'

        # Does the section's name need added to the prefix?
        if not isinstance(self, PlayerSettings):

            # Add the section's name to the prefix
            self[item]._prefix += self.name.lower().replace(' ', '_') + '_'

    @property
    def name(self):
        '''Returns the name of the _SettingsDictionary instance'''
        return self._name

    @property
    def prefix(self):
        '''Returns the prefix of the _SettingsDictionary instance'''
        return self._prefix

    def add_float_setting(
            self, name, default, text='', min_value=None, max_value=None):
        '''Adds a new float setting to the dictionary'''

        # Add the new float setting to the dictionary
        self[name] = _FloatSetting(name, default, text, min_value, max_value)

        # Return the setting
        return self[name]

    def add_int_setting(
            self, name, default, text='', min_value=None, max_value=None):
        '''Adds a new integer setting to the dictionary'''

        # Add the new integer setting to the dictionary
        self[name] = _IntegerSetting(name, default, text, min_value, max_value)

        # Return the setting
        return self[name]

    def add_string_setting(self, name, default, text=''):
        '''Adds a new string setting to the dictionary'''

        # Add the new string setting to the dictionary
        self[name] = _StringSetting(name, default, text)

        # Return the setting
        return self[name]

    def add_section(self, name, text=''):
        '''Adds a new section to the dictionary'''

        # Add the new section to the dictionary
        self[name] = _SettingsDictionary(name, text)

        # Return the section
        return self[name]


class PlayerSettings(_SettingsDictionary):
    '''Class used to register user settings'''

    def __init__(self, name, prefix, text=''):
        '''Verifies the given values, creates the instance,
            and stores the instance in the main dictionary'''

        # Is the given name already registered?
        if name in _MainSettingsDictionary:

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
        super(PlayerSettings, self).__init__(name, text)

        # Set the prefix for the settings
        self._prefix = prefix.lower()

        # Add the instance to the main dictionary
        _MainSettingsDictionary[name] = self

    def unregister_settings(self):
        '''Unregisters the given settings from the dictionary'''
        del _MainSettingsDictionary[self.name]
