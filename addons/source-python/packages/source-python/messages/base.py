# ../messages/base.py

# ============================================================================
# >> IMPORTS
# ============================================================================
# Python Imports
#   Collections
from collections import ChainMap
from collections import defaultdict
from collections import OrderedDict
#   ConfigObj
from configobj import ConfigObj
from configobj import Section
#   Imports
from imp import load_source
#   Path
from path import path

# Source.Python Imports
from engine_c import EngineServer
from core import echo_console
from excepthooks import ExceptHooks
#   UserMessage
from usermessage_c import UserMessage
#   Filters
from filters.recipients import RecipientFilter
#   Translations
from translations.strings import TranslationStrings


# ============================================================================
# >> GLOBAL VARIABLES
# ============================================================================
# Store the fieldtype converters
_fieldtypes = dict(bool=bool, char=str, byte=int, short=int, long=int,
    float=float, buffer=object, string=str)


# ============================================================================
# >> CLASSES
# ============================================================================
class _UserMessages(dict):
    '''Class used to store the message classes'''

    def __init__(self, file_path, *args, **kwargs):
        '''Parse the given files and store the parsed message classes'''

        # Parse the given file
        parsed_messages = ConfigObj(file_path)

        # Loop through all given arguments
        for file_path in args:

            # Parse and merge the current file
            parsed_messages.merge(ConfigObj(file_path))

        # Get the ../messages/types/ base path
        base_path = path(__file__).parent.joinpath('types')

        # Loop through all messages
        for message_name, message_data in parsed_messages.items():

            # Get the current message class data
            class_data = message_data.get('MESSAGE_CLASS', None)

            # Any class to import?
            if class_data is not None:

                # Import the current message class
                message_class = getattr(load_source(message_name,
                    base_path.joinpath(class_data[0])), class_data[1])

                # Delete the class data as we are done with it
                del message_data['MESSAGE_CLASS']

            # Otherwise
            else:

                # Use the base class
                message_class = BaseMessage

            # Get an ordered dictionnary to store the parameters
            required_parameters = OrderedDict()

            # Get the required parameters
            message_parameters = message_data.get('REQUIRED_PARAMETERS',
                None)

            # Get a set to store the translatable parameters
            translatable_parameters = set()

            # Any required parameters?
            if message_parameters is not None:

                # Delete the required parameters from the message data
                del message_data['REQUIRED_PARAMETERS']

                # Loop through all required parameters
                for parameter_name in message_parameters:

                    # Get the current parameter data
                    parameter_data = message_data.get(parameter_name)

                    # Delete the current parameter data
                    del message_data[parameter_name]

                    # Get the current parameter length
                    parameter_length = int(parameter_data.get('length', '1'))

                    # Is the current parameter larger than one value?
                    if parameter_length > 1:

                        # Get the the current parameter values
                        default_values = parameter_data['default_values']

                        # Loop through all required values
                        for parameter_index, parameter_type in zip(range(
                            parameter_length), parameter_data['types']):

                            # Convert the current value
                            default_values[parameter_index] = _fieldtypes[
                                parameter_type](default_values[
                                    parameter_index])

                        # Store the current parameter data
                        required_parameters[parameter_name] = dict(
                            default_values=default_values,
                                types=parameter_data['types'])

                    # Otherwise
                    else:

                        # Get the current parameter type
                        parameter_type = parameter_data['type']

                        # Is the current parameter translatable?
                        if parameter_type == 'string':

                            # Add the current parameter to the translatables
                            translatable_parameters.add(parameter_name)

                        # Store the current parameter data
                        required_parameters[parameter_name] = dict(
                            default_value=_fieldtypes[parameter_type](
                                parameter_data['default_value']),
                                    type=parameter_type)

                    # Store more data
                    required_parameters[parameter_name].update(dict(
                        length=parameter_length,
                            field_name=parameter_data.get('field_name', '')))

            # Get a dictionnary to store the special parameters
            special_parameters = dict()

            # Get the special parameters
            message_parameters = message_data.get('SPECIAL_PARAMETERS', None)

            # Any special parameters?
            if message_parameters is not None:

                # Delete the special parameters from the message data
                del message_data['SPECIAL_PARAMETERS']

                # Loop through all special parameters
                for parameter_name in message_parameters:

                    # Set the current parameter value
                    special_parameters[parameter_name] = message_data.get(
                        parameter_name, None)

                    # Is the current parameter value None?
                    if special_parameters[parameter_name] is None:

                        # No need to go further
                        continue

                    # Delete the current parameter data
                    del message_data[parameter_name]

            # Store the current message class
            self[message_name] = type(message_name, (message_class,), dict(
                _message_name=message_name,
                    _required_parameters=required_parameters,
                        _translatable_parameters=translatable_parameters,
                            _special_parameters=special_parameters,
                                **message_data))


class BaseMessage(dict):
    '''Base message class'''

    def __init__(self, *args, **kwargs):
        '''Parse and store the given parameters'''

        # Get a list of the given arguments
        arguments = list(args)

        # Loop through all required parameters
        for parameter_name in self._required_parameters:

            # Get the current parameter data
            parameter_data = self._required_parameters[parameter_name]

            # Get the current parameter length
            parameter_length = parameter_data['length']

            # Is the current parameter larger than one value?
            if parameter_length > 1:

                # Get the current parameter default values
                default_values = parameter_data['default_values']

                # Is the current parameter given as keyword?
                if parameter_name in kwargs:

                    # Get the given values
                    parameter_values = kwargs[parameter_name]

                    # Delete the current parameter from the keywords
                    del kwargs[parameter_name]

                # Otherwise, any arguments?
                elif arguments:

                    # Get the given values
                    parameter_values = arguments[:parameter_length]

                    # Get the given parameters length
                    values_length = len(parameter_values)

                    # Remove the values from the given arguments
                    arguments = arguments[values_length:]

                    # Make sure we have enough values
                    parameter_values += default_values[values_length:]

                # Otherwise
                else:

                    # use the default values
                    parameter_values = default_values

                # Set the current parameter value
                self[parameter_name] = parameter_values

            # Otherwise
            else:

                # Is the current parameter given as keyword?
                if parameter_name in kwargs:

                    # Get the current parameter value
                    parameter_value = kwargs[parameter_name]

                    # Delete the keyword as we are done with it
                    del kwargs[parameter_name]

                # Otherwise, any arguments?
                elif arguments:

                    # Get the given value
                    parameter_value = arguments.pop()

                # Otherwise
                else:

                    # Use the default value
                    parameter_value = parameter_data['default_value']

                # Set the current parameter value
                self[parameter_name] = parameter_value

        # Get a set to store the given users
        users = set()

        # Is the users given as keyword?
        if 'users' in kwargs:

            # Get the given users
            users = kwargs['users']

            # Delete the users from the keywords
            del kwargs['users']

        # Otherwise, any arguments?
        elif arguments:

            # Get the given users
            users = arguments

        # Set the given users
        super(BaseMessage, self).__setattr__('users', users)

        # Loop through all special parameters
        for parameter_name in self._special_parameters:

            # Get a variable to store the current parameter value
            parameter_value = self._special_parameters[parameter_name]

            # Is the current parameter not given?
            if parameter_name in kwargs:

                # Get the given value
                parameter_value = kwargs[parameter_name]

                # Delete the current parameter from the keywords
                del kwargs[parameter_name]

            # Store the given value
            self[parameter_name] = parameter_value

        # Set the given tokens
        super(BaseMessage, self).__setattr__('tokens', kwargs)


    def __getattr__(self, attribute):
        '''Return the given parameter value'''

        # Try to return from an attribute first
        try:

            # Return the given attribute value
            return super(BaseMessage, self).__getattr__(attribute)

        # Was the given attribute not valid?
        except AttributeError:

            # Return the given parameter value
            return self[attribute]


    def __setattr__(self, attribute, value):
        '''Set the given parameter value'''

        # Is the given attribute valid?
        if attribute in self.__dict__:

            # Set the given attribute value
            super(BaseMessage, self).__setattr__(attribute, value)

        # Otherwise
        else:

            # Set the given parameter value
            self[attribute] = value

    def __getitem__(self, item):
        '''Return teh given parameter value'''

        # Is the given item a valid parameter?
        if item in self or item in self._special_parameters:

            # Return the value of the given parameter
            return super(BaseMessage, self).__getitem__(item)

        # Otherwise, is the given item matching a token?
        elif item in self.tokens:

            # Return the value of the given token
            return self.tokens[item]

        # Raise an error
        raise KeyError('"{0}" is not a valid "{1}" '
            'parameter.'.format(item, self._message_name))

    def __setitem__(self, item, value):
        '''Set the given parameter to the given value'''

        # Is the given item a valid parameter?
        if (item in self._required_parameters or
            item in self._special_parameters):

            # Set the given parameter to the given value
            super(BaseMessage, self).__setitem__(item, value)

        # Otherwise
        else:

            # Assume it is a token
            self.tokens[item] = value

    def _prepare_parameter(self, parameter_name, parameter_value):
        '''Prepare the given parameter value'''

        # Get the given parameter data
        parameter_data = self._required_parameters[parameter_name]

        # Get the given parameter length
        parameter_length = parameter_data['length']

        # Is the current parameter larger than one value?
        if parameter_length > 1:

            # Is the given value not iterable?
            if not hasattr(parameter_value, '__iter__'):

                # Convert the given parametervalue to a tuple
                parameter_value = (parameter_value,)

            # Get a list of the given values
            parameter_values = list(parameter_value)

            # Get the length of the values
            values_length = len(parameter_values)

            # Not enough values?
            if values_length < parameter_length:

                # Make sure we have enough value
                parameter_values += parameter_data['default_values'][
                    values_length:]

            # Loop through all values
            for parameter_index, parameter_type, parameter_value in zip(
                range(parameter_length), parameter_data['types'],
                    parameter_values):

                # Convert the current value
                parameter_values[parameter_index] = _fieldtypes[
                    parameter_type](parameter_value)

            # Set the return value
            return_value = parameter_values

        # Otherwise
        else:

            # Convert the given value
            return_value = _fieldtypes[parameter_data['type']](
                parameter_value)

        # Return the prepared value
        return return_value

    def _write_field_value(self, parameter_name, usermsg, field_type,
        field_name, field_value, field_index=-1):
        '''Write the given field value to the given message'''
        getattr(usermsg, 'set_' + field_type)(field_name, field_value,
            field_index)

    def _send_message(self, recipient, **kwargs):
        '''Send the message to the given recipient filter'''

        # Get a UserMessage instance
        usermsg = UserMessage(recipient, self._message_name)

        # Loop through all required parameters
        for parameter_name in self._required_parameters:

            # Get the current parameter data
            parameter_data = self._required_parameters[parameter_name]

            # Get the current parameter length
            parameter_length = parameter_data['length']

            # Is the current parameter larger than one value?
            if parameter_length > 1:

                # Try to prepare the current parameter values
                try:

                    # Prepare the given values
                    parameter_values = self._prepare_parameter(parameter_name,
                        kwargs[parameter_name])

                # I'm not really fan of this but, to prevent crashes, we need
                #   to hook any exceptions that may occurs...
                except:

                    # Print the exception to the console
                    ExceptHooks.print_exception()

                    # Print a debugging message
                    echo_console(
                        '"{0}" is not a valid value for "{1}.{2}"'.format(
                            kwargs[parameter_name],
                            self._message_name,
                            parameter_name))

                    # Use the default values
                    parameter_values = self._prepare_parameter(parameter_name,
                        parameter_data['default_values'])

                # Get the current parameter field name
                field_name = parameter_data['field_name']

                # Loop through all values
                for parameter_index, parameter_type, parameter_value in zip(
                    range(parameter_length), parameter_data['types'],
                        parameter_values):

                    # Write the current parameter
                    self._write_field_value(parameter_name, usermsg,
                        parameter_type, field_name, parameter_value,
                            parameter_index)

            # Otherwise
            else:

                # Try to convert the given value
                try:

                    # Prepare the current parameter
                    parameter_value = self._prepare_parameter(parameter_name,
                        kwargs[parameter_name])

                # I'm not really fan of this but, to prevent crashes, we need
                #   to hook any exceptions that may occurs...
                except:

                    # Print the exception to the console
                    ExceptHooks.print_exception()

                    # Print a debugging message
                    echo_console(
                        '"{0}" is not a valid value for "{1}.{2}"'.format(
                            kwargs[parameter_name],
                            self._message_name,
                            parameter_name))

                    # Use the default value
                    parameter_value = self._prepare_parameter(parameter_name,
                        parameter_data['default_value'])

                # Write the current parameter
                self._write_field_value(parameter_name, usermsg,
                    parameter_data['type'], parameter_data['field_name'],
                        parameter_value)

        # Send the message
        usermsg.send_message()

    def send(self, *args, **kwargs):
        '''Send the message to the given users'''

        # Get a recipient filter of the given users
        recipient = RecipientFilter(*(args or self.users))

        # Any parameter to translate?
        if self._translatable_parameters:

            # Get a default dictionnary to store the players
            languages = defaultdict(set)

            # Get a mapping of the given tokens
            tokens = ChainMap(kwargs, self.tokens)

            # Loop through all indexes
            for index in recipient:

                # Add the current index
                languages[EngineServer.get_client_convar_value(index,
                    'cl_language')].add(index)

            # Loop through all languages
            for language, users in languages.items():

                # Get a dictionnary to store the translated strings
                translated_parameters = dict()

                # Loop through all translatable parameter
                for parameter_name in self._translatable_parameters:

                    # Get the current parameter value
                    parameter_value = self[parameter_name]

                    # Is the current parameter not translatable?
                    if not isinstance(parameter_value, TranslationStrings):

                        # No need to go further
                        continue

                    # Translate the current parameter
                    translated_parameters[parameter_name
                        ] = parameter_value.get_string(language, **tokens)

                # Update the recipient filter
                recipient.update(*users)

                # Send the message
                self._send_message(recipient, **ChainMap(
                    translated_parameters, self))

        # Otherwise
        else:

            # Send the message
            self._send_message(recipient, **self)
