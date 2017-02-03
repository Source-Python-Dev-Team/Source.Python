# ../effects/base.py

"""Provides base effect classes."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
#   Colors
from colors import Color
#   Effects
from effects.templates import temp_entity_templates
#   Engines
from engines.precache import Decal
from engines.precache import Model
#   Entities
from entities.classes import _supported_property_types
from entities.entity import Entity
from entities.props import SendPropType
#   Filters
from filters.recipients import RecipientFilter
#   Memory
from memory import get_object_pointer
from memory import get_size
from memory import make_object
from memory import Pointer
from memory.helpers import Array
from memory.helpers import Type
from memory.manager import manager
#   Players
from players.entity import Player
#   String Tables
from stringtables import string_tables

# Site-Packages Imports
#   ConfigObj
from configobj import Section


# =============================================================================
# >> FORWARD IMPORTS
# =============================================================================
# Source.Python Imports
#   Effects
from _effects._base import BaseTempEntity


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('BaseTempEntity',
           'TempEntity',
           )


# ============================================================================
# >> CLASSES
# ============================================================================
class TempEntity(BaseTempEntity):
    """Class used to interact with a specific temp entity."""

    def __init__(self, temp_entity, **aliases):
        """Initialize the temp entity instance.

        :param str/Pointer temp_entity:
            The name of the temp entity to initialize or the pointer to wrap.
        :param aliases:
            Any alias to set on initialization.
        """
        # Is the given name a Pointer instance?
        if isinstance(temp_entity, Pointer):

            # Wrap the given pointer...
            super()._wrap_ptr(temp_entity)

        # Otherwise...
        else:

            # Get the template of the temp entity...
            template = temp_entity_templates[temp_entity]

            # Initialize the temp entity...
            super()._copy_base(template, template.size)

        # Loop through all given aliases...
        for alias, value in aliases.items():

            # Set the alias to the given value...
            setattr(self, alias, value)

    @classmethod
    def _obj(cls, ptr):
        """Wrap the given pointer.

        :param Pointer ptr:
            The pointer to wrap.
        """
        return cls(ptr)

    @property
    def _size(self):
        """Return the size of the temp entity.

        :rtype: int
        """
        return self.template.size

    def __getattr__(self, name):
        """Return the value of the given alias.

        :param str name:
            The alias name.
        :rtype: object
        """
        # Get the name of the prop...
        prop_name = self.template.aliases.get(name, None)

        # Was the given name a valid alias?
        if prop_name is not None:

            # Is the name a section?
            if isinstance(prop_name, Section):

                # Is the alias a decal?
                if prop_name['type'] == Decal.__name__:

                    # Return the decal instance...
                    return Decal(string_tables[Decal.precache_table][
                        getattr(self, prop_name['name'])])

                # Otherwise, is the alias an entity?
                elif prop_name['type'] == Entity.__name__:

                    # Return the entity instance...
                    return Entity(getattr(self, prop_name['name']))

                # Otherwise, is the alias a player?
                elif prop_name['type'] == Player.__name__:

                    # Return the player instance...
                    return Player(getattr(self, prop_name['name']))

                # Otherwise, is the alias a model?
                elif prop_name['type'] == Model.__name__:

                    # Get the name of the model...
                    model_name = string_tables[Model.precache_table][
                        getattr(self, prop_name['name'])]

                    # Was the model not precached?
                    if not model_name:

                        # Return an error model...
                        return Model('models/error.mdl')

                    # Return the model instance...
                    return Model(model_name)

                # Otherwise, is the alias a color?
                elif prop_name['type'] == Color.__name__:

                    # Get a tuple to store the RGBA values...
                    values = tuple()

                    # Loop through all aliases...
                    for alias in prop_name['name']:

                        # Add the current value to the tuple...
                        values += (getattr(self, alias),)

                    # Return the color instance...
                    return Color(*values)

                # Otherwise...
                else:

                    # Get the data of the property...
                    prop, offset, type_name = self.template.properties[
                        prop_name['name']]

                    # Return the value of the property...
                    return make_object(
                        manager.get_class(prop_name['type']),
                        get_object_pointer(self) + offset)

            # Get the data of the property...
            prop, offset, type_name = self.template.properties[prop_name]

            # Return the value of the property...
            return self._get_property(prop_name, prop.type)

        # Return the value of the given attribute...
        return super().__getattribute__(name)

    def __setattr__(self, name, value):
        """Set the value of the given alias.

        :param str name:
            The alias name.
        :param object value:
            The value to set.
        """
        # Get the name of the prop...
        prop_name = self.template.aliases.get(name, None)

        # Was the given name a valid alias?
        if prop_name is not None:

            # Is the name a section?
            if isinstance(prop_name, Section):

                # Is the alias a decal?
                if prop_name['type'] == Decal.__name__:

                    # Is the given value an invalid decal instance?
                    if not isinstance(value, Decal):

                        # Raise an exception...
                        raise ValueError(
                            '"{}" is not a valid Decal instance.'.format(
                                value))

                    # Set the model index...
                    setattr(self, prop_name['name'], value.index)

                # Is the alias an entity?
                elif prop_name['type'] == Entity.__name__:

                    # Is the given value an invalid entity instance?
                    if not isinstance(value, Entity):

                        # Raise an exception...
                        raise ValueError(
                            '"{}" is not a valid Entity instance.'.format(
                                value))

                    # Set the alias value...
                    setattr(self, prop_name['name'], value.index)

                # Otherwise, is the alias a player?
                elif prop_name['type'] == Player.__name__:

                    # Set the player instance...
                    setattr(self, prop_name['name'], value.index)

                # Otherwise, is the alias a model?
                elif prop_name['type'] == Model.__name__:

                    # Is the given value an invalid model instance?
                    if not isinstance(value, Model):

                        # Raise an exception...
                        raise ValueError(
                            '"{}" is not a valid Model instance.'.format(
                                value))

                    # Set the model index...
                    setattr(self, prop_name['name'], value.index)

                # Otherwise, is the alias a color?
                elif prop_name['type'] == Color.__name__:

                    # Is the given value an invalid color instance?
                    if not isinstance(value, Color):

                        # Raise an exception...
                        raise ValueError(
                            '"{}" is not a valid Color instance.'.format(
                                value))

                    # Loop through all aliases...
                    for index, alias in enumerate(prop_name['name']):

                        # Set the current alias...
                        setattr(self, alias, value[index])

                # Otherwise...
                else:

                    # Get the data of the property...
                    prop, offset, type_name = self.template.properties[
                        prop_name['name']]

                    # Get the class of the type...
                    cls = manager.get_class(prop_name['type'])

                    # Is the given value not valid?
                    if not isinstance(value, cls):

                        # Raise an exception...
                        raise TypeError(
                            'The given value is not of type "{}".'.format(
                                type_name))

                    # Set the value of the property...
                    get_object_pointer(value).copy(
                        get_object_pointer(self) + offset,
                        self.template._get_type_size(prop_name['type']))

                # No need to go further...
                return

            # Get the data of the property...
            prop, offset, type_name = self.template.properties[prop_name]

            # Set the value of the property...
            self._set_property(prop_name, prop.type, value)

            # No need to go further...
            return

        # Set the value of the given attribute...
        super().__setattr__(name, value)

    def __dir__(self):
        """Return an alphabetized list of attributes for the instance."""
        # Get the base attributes
        attributes = set(super().__dir__())

        # Add all aliases to the attributes
        attributes.update(set(self.template.aliases))

        # Return a sorted list of attributes
        return sorted(attributes)

    def _get_property(self, prop_name, prop_type):
        """Return the value of the given property name.

        :param str prop_name:
            The name of the property.
        :param SendPropType prop_type:
            The type of the property.
        """
        # Is the given property not valid?
        if prop_name not in self.template.properties:

            # Raise an exception...
            raise NameError(
                '"{}" is not a valid property for temp entity "{}".'.format(
                    prop_name, self.name))

        # Get the property data...
        prop, offset, type_name = self.template.properties[prop_name]

        # Are the prop types matching?
        if prop.type != prop_type:

            # Raise an exception...
            raise TypeError('"{}" is not of type "{}".'.format(
                prop_name, prop_type))

        # Is the property an array?
        if prop_type == SendPropType.ARRAY:

            # Return an array instance...
            return Array(manager, False, type_name, get_object_pointer(
                self) + offset, prop.length)

        # Is the given type not supported?
        if prop_type not in _supported_property_types:

            # Raise an exception...
            raise TypeError('"{}" is not supported.'.format(prop_type))

        # Is the type native?
        if Type.is_native(type_name):

            # Return the value...
            return getattr(
                get_object_pointer(self), 'get_' + type_name)(offset)

        # Otherwise
        else:

            # Make the object and return it...
            return make_object(
                manager.get_class(type_name),
                get_object_pointer(self) + offset)

        # Raise an exception...
        raise ValueError('Unable to get the value of "{}".'.format(prop_name))

    def get_property_array(self, prop_name):
        """Return the value of the given property as an Array instance.

        :param str prop_name:
            The name of the property.
        :rtype: Array
        """
        return self._get_property(prop_name, SendPropType.ARRAY)

    def get_property_bool(self, prop_name):
        """Return the value of the given property as a boolean.

        :param str prop_name:
            The name of the property.
        :rtype: bool
        """
        return bool(self._get_property(prop_name, SendPropType.INT))

    def get_property_float(self, prop_name):
        """Return the value of the given property as a float.

        :param str prop_name:
            The name of the property.
        :rtype: float
        """
        return self._get_property(prop_name, SendPropType.FLOAT)

    def get_property_int(self, prop_name):
        """Return the value of the given property as an integer.

        :param str prop_name:
            The name of the property.
        :rtype: int
        """
        return self._get_property(prop_name, SendPropType.INT)

    def get_property_string(self, prop_name):
        """Return the value of the given property as a string.

        :param str prop_name:
            The name of the property.
        :rtype: str
        """
        return self._get_property(prop_name, SendPropType.STRING)

    def get_property_vector(self, prop_name):
        """Return the value of the given property as a string.

        :param str prop_name:
            The name of the property.
        :rtype: Vector
        """
        return self._get_property(prop_name, SendPropType.VECTOR)

    def _set_property(self, prop_name, prop_type, value):
        """Set the given property to the given value.

        :param str prop_name:
            The name of the property.
        :param SendPropType prop_type:
            The type of the property.
        :param value object:
            To value to set to the given property.
        """
        # Is the given property not valid?
        if prop_name not in self.template.properties:

            # Raise an exception...
            raise NameError(
                '"{}" is not a valid property for temp entity "{}".'.format(
                    prop_name, self.name))

        # Get the property data...
        prop, offset, type_name = self.template.properties[prop_name]

        # Are the prop types matching?
        if prop.type != prop_type:

            # Raise an exception...
            raise TypeError('"{}" is not of type "{}".'.format(
                prop_name, prop_type))

        # Is the property an array?
        if prop_type == SendPropType.ARRAY:

            # Is the given value not an Array instance?
            if not isinstance(value, Array):

                # Raise an exception...
                raise TypeError('Given value is not an Array instance.')

            # Is the length not matching?
            if value._length != prop.length:

                # Raise an exception...
                raise ValueError('Given array is not of length "{}".'.format(
                    prop.length))

            # Copy the values...
            value.copy(get_object_pointer(self), self._get_type_size(
                type_name) * prop.length)

            # No need to go further...
            return

        # Otherwise, is the type native?
        elif Type.is_native(type_name):

            # Set the value of the property...
            getattr(get_object_pointer(self), 'set_{}'.format(type_name))(
                value, offset)

            # No need to go further...
            return

        # Otherwise...
        else:

            # Get the class...
            cls = manager.get_class(type_name)

            # Is the given value valid?
            if not isinstance(value, cls):

                # Raise an exception...
                raise TypeError('"{}" is not a valid "{}" value.'.format(
                    value, type_name))

            # Set the value of the property...
            get_object_pointer(value).copy(
                get_object_pointer(self) + offset, get_size(cls))

            # No need to go further...
            return

        # Raise an exception...
        raise NameError('Unable to set "{}" for the temp entity "{}".'.format(
            prop_name, self.name))

    def set_property_array(self, prop_name, value):
        """Set the value of the given property as an Array instance.

        :param str prop_name:
            The name of the property.
        :param Array value:
            The value to set.
        """
        self._set_property(prop_name, SendPropType.ARRAY, value)

    def set_property_bool(self, prop_name, value):
        """Set the value of the given property as a boolean.

        :param str prop_name:
            The name of the property.
        :param bool value:
            The value to set.
        """
        self._set_property(prop_name, SendPropType.INT, int(value))

    def set_property_float(self, prop_name, value):
        """Set the value of the given property as a float.

        :param str prop_name:
            The name of the property.
        :param float value:
            The value to set.
        """
        self._set_property(prop_name, SendPropType.FLOAT, value)

    def set_property_int(self, prop_name, value):
        """Set the value of the given property as an integer.

        :param str prop_name:
            The name of the property.
        :param int value:
            The value to set.
        """
        self._set_property(prop_name, SendPropType.INT, value)

    def set_property_string(self, prop_name, value):
        """Set the value of the given property as a string.

        :param str prop_name:
            The name of the property.
        :param str value:
            The value to set.
        """
        self._set_property(prop_name, SendPropType.STRING, value)

    def set_property_vector(self, prop_name, value):
        """Set the value of the given property as a string.

        :param str prop_name:
            The name of the property.
        :param Vector value:
            The value to set.
        """
        self._set_property(prop_name, SendPropType.VECTOR, value)

    def create(self, *recipients, delay=0.0, **aliases):
        """Create the temp entity effect.

        :param RecipientFilter recipients:
            The recipient filter listing the players to send the effect to.
        :param float delay:
            The delay before creating the effect.
        :param dict aliases:
            Any aliases to set before creating the temp entity effect.
        """
        # Get a recipient filter matching the given players...
        recipients = RecipientFilter(*recipients)

        # Loop trhough all given aliases...
        for alias, value in aliases.items():

            # Set the alias to the given value...
            setattr(self, alias, value)

        # Create the temp entity effect...
        super().create(recipients, delay)

    @property
    def template(self):
        """Return the template of the temp entity.

        :rtype: TempEntityTemplate
        """
        return temp_entity_templates[self.name]
