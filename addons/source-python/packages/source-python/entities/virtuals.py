# ../entities/virtuals.py

# =============================================================================
# >> IMPORTS
# =============================================================================
# Site-Packages Imports
#   Configobj
from configobj import ConfigObj

# Source.Python Imports
from core import GAME_NAME
from core import PLATFORM
from paths import SP_DATA_PATH
#   Entities
from entities.constants import DATA_DESC_MAP_OFFSET
from entities.datamaps import DataMap
from entities.helpers import pointer_from_edict
#   Memory
from memory import Argument
from memory import Convention
from memory import Function
from memory import Return
from memory import make_object
from memory.helpers import Key
from memory.helpers import NO_DEFAULT


# =============================================================================
# >> GLOBAL VARIABLES
# =============================================================================
# Store the raw data types
_ini_data = (
    (Key.OFFSET, int, NO_DEFAULT),
    (Key.CONVENTION, Key.as_convention, Convention.CDECL),
    (Key.ARGS, Key.as_args_tuple, ()),
    (Key.RETURN_TYPE, Key.as_return_type, Return.VOID),
)

# Store the base paths to the virtual files
_virtuals_path = SP_DATA_PATH.joinpath('entities', 'virtuals', GAME_NAME)
_virtuals_base_path = SP_DATA_PATH.joinpath('virtuals', GAME_NAME)


# =============================================================================
# >> CLASSES
# =============================================================================
class _EntityVirtualFunctions(dict):
    """Dictionary that stores all virtual functions by server class name"""

    def get_virtual_functions(self, edict):
        """Retrieves all functions for the given edict's server class"""

        # Is the GetDataDescMap offset set?
        if DATA_DESC_MAP_OFFSET is None:
            return {}

        # Get the edict's classname
        server_class = edict.get_networkable().get_server_class().name

        # Has the entity's server class already been added to the dictionary?
        if not server_class in self:

            # Get the pointer of the edict
            pointer = pointer_from_edict(edict)

            # Create the function for the entity's GetDataDescMap
            function = pointer.make_virtual_function(
                DATA_DESC_MAP_OFFSET, Convention.THISCALL,
                (Argument.POINTER, ), Return.POINTER)

            # Create a DataMap object
            datamap = make_object(DataMap, function(pointer))

            # Set the new server_class name to the datamap's name.
            # This is done to fix a KeyError that occurs when the
            # server_class is still not in the dictionary later on.
            server_class = datamap.class_name

            # Is the current server_class value not in the dictionary?
            if not server_class in self:

                # Use "while" to get all datamaps for the entity
                while datamap:

                    # Store the class name as a _Virtuals dictionary object
                    self[datamap.class_name] = _Virtuals(datamap.class_name)

                    # Move up a level and store the base of the current datamap
                    self[datamap.class_name].base = datamap = datamap.base

        # Create a dictionary of values to return
        values = dict()

        # Use a "while" statement to get all values for the entity
        while server_class in self:

            # Loop through each value in the current server_class
            for value in self[server_class]:

                # Is the value not yet in the return dictionary?
                if not value in values:

                    # Add the item to the return dictionary
                    values[value] = self[server_class][value]

            # Use try/except in case .base returns None
            try:

                # Move up one level
                server_class = self[server_class].base.class_name

            # Was an exception encountered?
            except AttributeError:

                # Break the while loop
                break

        # Return the values
        return values

# Get the _EntityVirtualFunctions instance
EntityVirtualFunctions = _EntityVirtualFunctions()


class _Virtuals(dict):
    """Dictionary used to store virtual functions"""

    def __init__(self, class_name):
        """Retrieves all virtual functions for the given
            class name and stores them in the dictionary"""

        # Get the paths to the game's virtual functions
        file_path = _virtuals_path.joinpath(class_name + '.ini')
        base_path = _virtuals_base_path.joinpath(class_name + '.ini')

        # Do both files exist?
        if not (file_path.isfile() and base_path.isfile()):
            return

        # Get the contents of both files
        file_contents = ConfigObj(file_path)
        base_contents = ConfigObj(base_path, unrepr=True)

        # Loop through the supported file's items
        for item in file_contents:

            # Does the current item's "name" exist in the database?
            if not file_contents[item]['name'] in base_contents:
                continue

            # Set the "offset" value for the current item
            file_contents[item]['offset'] = base_contents[
                file_contents[item]['name']][PLATFORM]

            # Add the item to the dictionary as a _VirtualStager instance
            self[item] = _VirtualStager(file_contents[item], self, item)

    def __setitem__(self, item, value):
        """Disallows setting an item to anything other than a
            _VirtualStager or _VirtualFunction instance"""
        if not isinstance(value, (_VirtualStager, _VirtualFunction)):
            raise TypeError(
                'Setting _Virtuals item to anything other than a ' +
                '_VirtualStager or VirtualFunction instance is not allowed')
        super(_Virtuals, self).__setitem__(item, value)


class _VirtualStager(object):
    """Class used a "stager" so that the Function instance
        is only retrieved if the functionality is used"""

    def __init__(self, items, host, name):
        """Store the items, host dictionary, and item name"""
        self.items = items
        self.host = host
        self.name = name

    def __setattr__(self, attr, value):
        """Sets the host dictionary's value for the item to a _VirtualFunction
            instance the first time it is about to be called"""

        # Is the given attribute anything other than "current_pointer"
        if attr != 'current_pointer':

            # Allow setting all other attributes
            super(_VirtualStager, self).__setattr__(attr, value)
            return

        # Get the _VirtualFunction instance for the stager
        new_value = _VirtualFunction(self.items, value)

        # Set the host dictionary's value to the _VirtualFunction instance
        self.host[self.name] = new_value


class _VirtualFunction(Function):
    """Class used to create and call a virtual function for entities"""

    def __init__(self, items, current_pointer):
        """Creates the virtual function"""

        # Create a list to store the arguments for <>.make_virtual_function()
        data = list()

        # Loop through the data checkers
        for key, converter, default in _ini_data:

            # Get the virtual function's value for the current data
            value = items.get(key, default)

            # Is the value invalid?
            if value is NO_DEFAULT:
                raise KeyError(
                    'Missing information for key "{0}".'.format(key))

            # Add the current value to the data list
            data.append(value if value is default else converter(value))

        # Get the Function instance
        function = current_pointer.make_virtual_function(*data)

        # Initialize the Function instance
        super(_VirtualFunction, self).__init__(function)

        # Set the current pointer to the given one
        self.current_pointer = current_pointer

        # Store the index to add the pointer
        self.pointer_index = int(items.get('pointer_index', 0))

    def __call__(self, *args):
        """Adds the entity's pointer as the <pointer_index>
            argument when calling the virtual function"""

        # Add the pointer to the arguments
        args = (
            args[:self.pointer_index] +
            (self.current_pointer, ) +
            args[self.pointer_index:])

        # Call the virtual function
        super(_VirtualFunction, self).__call__(*args)

        # Reset the current pointer
        self.current_pointer = None
