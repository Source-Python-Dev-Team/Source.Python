# ../commands/manager.py

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
#   Commands
from commands import CommandReturn
#   Hooks
from hooks.exceptions import ExceptHooks


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
# Set all to an empty list
__all__ = []


# =============================================================================
# >> CLASSES
# =============================================================================
class _BaseCommandManager(dict):
    '''Class used to (un)register commands'''

    # Store the base attributes
    _use_args = True
    _callback_manager = None

    def register_commands(self, names, callback, *args, **kwargs):
        '''Registers the given commands to the given callback'''

        # Was a single command name given?
        if isinstance(names, str):

            # Store the command as a list
            names = [names]

        # Are the names of a proper iterable type?
        if type(names) not in (list, tuple):

            # Raise an error
            raise TypeError(
                '{0} commands must be passed as a list, tuple, or string,'
                ' not "{1}"'.format(type(self).__name__, type(names).__name__))

        # Is there a specified callback manager for this class?
        if self._callback_manager is not None:

            # Get the callback manager's instance for the given callback
            callback = self._callback_manager(callback, *args, **kwargs)

        # Loop through the given names
        for name in names:

            # Is the command already registered?
            if name not in self:

                # Are the arguments supposed to be used
                # when getting the command instance?
                if self._use_args:

                    # Get the command using the arguments
                    command = self._get_command(name, *args)

                # Are the arguments not supposed to be used?
                else:

                    # Get the command without the arguments
                    command = self._get_command(name)

                # Add the command to the dictionary
                self[name] = _CallbackList(command)

            # Add the callback to the command's list of callbacks
            self[name].append(callback)

    def unregister_commands(self, names, callback):
        '''Unregisters the given commands from the given callback'''

        # Was a single command name given?
        if isinstance(names, str):

            # Store the command as a list
            names = [names]

        # Are the names of a proper iterable type?
        if type(names) not in (list, tuple):

            # Raise an error
            raise TypeError(
                '{0} commands must be passed as a list, tuple, or string,'
                ' not "{1}"'.format(type(self).__name__, type(names).__name__))

        # Loop through all given names
        for name in names:

            # Is the command registered?
            if name not in self:

                # Raise an error
                raise KeyError('Command "{0}" not registered'.format(name))

            # Is there a specified callback manager for this class?
            if self._callback_manager is not None:

                # Loop through each callback in the command's list
                for registered_callback in self[name]:

                    # Is the current callback an
                    # instance for the given callback?
                    if registered_callback.callback == callback:

                        # Set the callback's instance to the current callback
                        callback = registered_callback

                        # Break the loop
                        break

            # Remove the callback from the command's list
            self[name].remove(callback)

            # Are there any more callbacks registered to the command?
            if not self[name]:

                # Remove the callback from the command's callback
                self[name].command.remove_callback(self[name])

                # Remove the command from the dictionary
                del self[name]


class _CallbackList(list):
    '''List class used to store'''

    def __init__(self, command):
        '''
            Stores the given command and registers the instance to the command
        '''

        # Store the command
        self.command = command

        # Add the instance to the command's callback list
        self.command.add_callback(self)

    def __call__(self, *args):
        '''Calls all callbacks for the command'''

        # Loop through each callback in the list
        for callback in self:

            # Use try/except to continue the loop in case of an error
            try:

                # Call the callback and get its return value
                return_value = callback(*args)

            # Was an error encountered?
            except:

                # Print the exception to the console
                ExceptHooks.print_exception()

            # Was no error encountered?
            else:

                # Does the command need blocked?
                if not (return_value is None or return_value):

                    # Block the command
                    return CommandReturn.BLOCK

        # Allow the command to continue
        return CommandReturn.CONTINUE

    def append(self, callback):
        '''Adds a callback to the list'''

        # Is the callback already in the list?
        if callback in self:

            # Raise an error
            raise ValueError('Callback already registered to command')

        # Add the callback to the list
        super(_CallbackList, self).append(callback)

    def remove(self, callback):
        '''Removes a callback from the list'''

        # Is the callback in the list?
        if callback not in self:

            # Raise an error
            raise ValueError('Callback not registered to command')

        # Remove the callback from the list
        super(_CallbackList, self).remove(callback)
