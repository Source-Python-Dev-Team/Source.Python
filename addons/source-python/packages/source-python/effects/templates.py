# ../effects/templates.py

"""Base templates for effect classes."""

# ============================================================================
# >> IMPORTS
# ============================================================================
# Source.Python Imports
#   Core
from core import GameConfigObj
#   Effects
from _effects._base import BaseTempEntity
#   Entities
from entities.classes import _supported_property_types
from entities.classes import server_classes
from entities.props import SendPropType
#   Memory
from memory import TYPE_SIZES
from memory import get_size
from memory.helpers import Type
from memory.hooks import HookType
from memory.manager import manager
#   Paths
from paths import SP_DATA_PATH

# Site-Packages Imports
#   ConfigObj
from configobj import Section


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('TempEntityTemplate',
           'TempEntityTemplates',
           'temp_entity_templates',
           )


# ============================================================================
# >> CLASSES
# ============================================================================
class TempEntityTemplate(BaseTempEntity):
    """A temp entity template."""

    def __init__(self, temp_entity):
        """Initialize the instance.

        :param BaseTempEntity temp_entity:
            The base entity instance to copy the base from.
        """
        # Store the size of the temp entity...
        self._size = get_size(BaseTempEntity)

        # Get the aliases...
        self._aliases = GameConfigObj(
            SP_DATA_PATH / 'effects' / temp_entity.server_class.name + '.ini')

        # Get a dictionary to store the properties...
        self._properties = dict()

        # Add the properties to the dictionary...
        self._add_properties(temp_entity.server_class.table)

        # Loop through all properties...
        for prop in temp_entity.server_class.table:

            # Is the current prop a base class?
            if prop.name != 'baseclass':

                # Pass to the next property...
                continue

            # Add the current table to the properties...
            self._add_properties(prop.data_table)

        # Get a list to store our hooks...
        self._hooks = list()

        # Initialize the base class...
        super()._copy_base(temp_entity, self.size)

    def _add_properties(self, send_table):
        """Parse the given send table and add all properties to the dictionary.

        :param SendTable send_table:
            The send table instance to parse.
        """
        # Loop through all properties...
        for name, prop, offset in server_classes._find_properties(send_table):

            # Is the current prop an array?
            if prop.type == SendPropType.ARRAY:

                # Loop through all aliases...
                for alias, data in self.aliases.items():

                    # Is the current alias matching the prop name?
                    if isinstance(data, Section) and data['name'] == name:

                        # Set the name of the type...
                        type_name = data['type']

                        # Get the size of the array...
                        size = self._get_type_size(data['type']) * prop.length

                        # No need to continue looping...
                        break

                # Was no size retrieved?
                else:

                    # We don't want to add that property...
                    continue

            # Otherwise, is the type supported?
            elif prop.type in _supported_property_types:

                # Set the name of the type...
                type_name = _supported_property_types[
                    prop.type]

                # Get the size of the type...
                size = self._get_type_size(type_name)

            # Otherwise...
            else:

                # We don't want to add that property...
                continue

            # Add the offset to the size...
            size += offset

            # Is the size larger than our actual stored size?
            if size > self.size:

                # Update the size...
                self._size = size

            # Add the property...
            self._properties[name] = (prop, offset, type_name)

    @staticmethod
    def _get_type_size(type_name):
        """Helper method returning the size of the given type.

        :param str type_name:
            The name of the type.
        """
        # Is the type native?
        if Type.is_native(type_name):

            # Return the size of the type...
            return TYPE_SIZES[type_name.upper()]

        # Otherwise...
        else:

            # Get the size of the type...
            return get_size(manager.get_class(type_name))

        # Raise an exception...
        raise ValueError('"{}" is not a supported type.'.format(type_name))

    def add_hook(self, callback):
        """Register a hook for this temp entity.

        :param function callback:
            The callback function to register.
        """
        # Is the given callback not callable?
        if not callable(callback):
            raise TypeError('The given callback is not callable.')

        # Is the callback already registered?
        if callback in self.hooks:
            raise ValueError('The given callback is already registered.')

        # Register the hook...
        self.hooks.append(callback)

    def remove_hook(self, callback):
        """Unregister a hook for this temp entity.

        :param function callback:
            The callback function to unregister.
        """
        # Raise an exception if the given callback isn't registered...
        if callback not in self.hooks:
            raise ValueError('The given callback is not registered.')

        # Unregister the hook...
        self.hooks.remove(callback)

    def handle_hook(self, temp_entity, recipient_filter):
        """Call the registered callbacks.

        :param TempEntity temp_entity:
            The TempEntity instance.
        :param RecipientFilter recipient_filter:
            The RecipientFilter instance.

        :rtype: bool
        """
        # Set the default return value to None...
        return_value = None

        # Loop through all registered hooks for this temp entity...
        for callback in self.hooks:

            # Call the callback and store the value it returned...
            returned_value = callback(temp_entity, recipient_filter)

            # Did the callback return anything?
            if returned_value is not None:

                # Yes, so override the return value...
                return_value = returned_value

        # Return the return value...
        return return_value

    @property
    def aliases(self):
        """Return the aliases of the temp entity.

        :rtype: GameConfigObj
        """
        return self._aliases

    @property
    def hooks(self):
        """Return the registered hooks for this temp entity.

        :rtype: dict
        """
        return self._hooks

    @property
    def properties(self):
        """Return the properties data of the temp entity.

        :rtype: dict
        """
        return self._properties

    @property
    def size(self):
        """Return the size of the temp entity instance.

        :rtype: int
        """
        return self._size


class TempEntityTemplates(dict):
    """Container class used to store the temp entity templates instances."""

    def __missing__(self, temp_entity_name):
        """Called when a temp entity template is requested but missing.

        :param str temp_entity_name:
            The name of the temp entity template requested.
        :raise NameError:
            Raised if the given name is not a valid temp entity name.
        """
        # Import this here to fix a cyclic import
        from effects import _first_temp_entity

        # Get the first temp entity in the chain...
        temp_entity = _first_temp_entity

        # Loop as long as we have a valid instance...
        while temp_entity:

            # Are the names matching?
            if temp_entity.name == temp_entity_name:

                # No need to continue searching...
                break

            # Pass to the next temp entity in the chain...
            temp_entity = temp_entity.next

        # Did we reach the last temp entity in the chain?
        if temp_entity is None:

            # If so, raise an exception....
            raise NameError('"{}" is not a valid temp entity name.'.format(
                temp_entity_name))

        # Get the temp entity template...
        temp_entity_template = self[temp_entity_name] = TempEntityTemplate(
            temp_entity)

        # Return the temp entity template...
        return temp_entity_template


# ============================================================================
# >> GLOBAL VARIABLES
# ============================================================================
# Get a dictionary to store the temp entity templates...
temp_entity_templates = TempEntityTemplates()
