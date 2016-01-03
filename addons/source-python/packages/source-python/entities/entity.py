# ../entities/entity.py

"""Provides a base class to interact with a specific entity."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
#   Memory
import memory
#   Engines
from engines.precache import Model
from engines.trace import engine_trace
from engines.trace import ContentMasks
from engines.trace import GameTrace
from engines.trace import Ray
from engines.trace import TraceFilterSimple
#   Entities
from entities import BaseEntityGenerator
from entities.classes import server_classes
from entities.constants import RenderMode
from entities.factories import factory_dictionary
from entities.helpers import create_entity
from entities.helpers import edict_from_index
from entities.helpers import index_from_pointer
from entities.helpers import spawn_entity
from entities.specials import _EntitySpecials
#   Memory
from memory import make_object
#   Studio
from studio.cache import model_cache
from studio.constants import INVALID_ATTACHMENT_INDEX


# =============================================================================
# >> FORWARD IMPORTS
# =============================================================================
# Source.Python Imports
#   Entities
from _entities._entity import BaseEntity


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
# Add all the global variables to __all__
__all__ = ('BaseEntity',
           'Entity',
           )


# =============================================================================
# >> CLASSES
# =============================================================================
class Entity(BaseEntity, _EntitySpecials):
    """Class used to interact directly with entities."""

    def __init__(self, index):
        """Initialize the Entity object."""
        # Initialize the object
        super().__init__(index)

        # Set the entity's base attributes
        object.__setattr__(self, '_index', index)
        object.__setattr__(self, '_edict', None)
        object.__setattr__(self, '_pointer', None)

    def __getattr__(self, attr):
        """Find if the attribute is valid and returns the appropriate value."""
        # Loop through all instances (used to get edict/IPlayerInfo attributes)
        for instance in self.instances:

            # Does the current instance contain the given attribute?
            if hasattr(instance, attr):

                # Return the instance's value for the given attribute
                return getattr(instance, attr)

        # Loop through all of the entity's server classes
        for server_class in self.server_classes:

            # Does the current server class contain the given attribute?
            if hasattr(server_class, attr):

                # Return the attribute's value
                return getattr(make_object(server_class, self.pointer), attr)

        # If the attribute is not found, raise an error
        raise AttributeError('Attribute "{0}" not found'.format(attr))

    def __setattr__(self, attr, value):
        """Find if the attribute is value and sets its value."""
        # Is the given attribute private?
        if attr.startswith('_'):

            # Get the name of the private attribute
            name = attr[1:]

            # Is the attribute a property?
            if (name in super().__dir__() and isinstance(
                    getattr(self.__class__, name, None), property)):

                # Set the private attribute's value
                object.__setattr__(self, attr, value)

                # No need to go further
                return

        # Is the given attribute a property?
        if (attr in super().__dir__() and isinstance(
                getattr(self.__class__, attr, None), property)):

            # Set the property's value
            object.__setattr__(self, attr, value)

            # No need to go further
            return

        # Loop through all of the entity's server classes
        for server_class in self.server_classes:

            # Does the current server class contain the given attribute?
            if hasattr(server_class, attr):

                # Set the attribute's value
                setattr(server_class(self.pointer, wrap=True), attr, value)

                # No need to go further
                return

        # If the attribute is not found, just set the attribute
        super().__setattr__(attr, value)

    def __dir__(self):
        """Return an alphabetized list of attributes for the instance."""
        # Get the base attributes
        attributes = set(super().__dir__())

        # Loop through all instances for the entity
        for instance in self.instances:

            # Loop through all of the attributes of the current instance
            for attr in dir(instance):

                # Add the attribute if it is not private
                if not attr.startswith('_'):
                    attributes.add(attr)

        # Loop through all server classes for the entity
        for server_class in self.server_classes:

            # Loop through all of the server class' attributes
            for attr in dir(server_class):

                # Add the attribute if it is not private
                if not attr.startswith('_'):
                    attributes.add(attr)

        # Return a sorted list of attributes
        return sorted(attributes)

    @classmethod
    def create(cls, classname):
        """Create a new entity with the given classname."""
        return cls(create_entity(classname))

    @staticmethod
    def find(classname):
        """Try to find an entity with the given classname.

        If not entity has been found, None will be returned.

        :param str classname: The classname of the entity.
        :return: Return the found entity.
        :rtype: Entity
        """
        # Import this here to fix a circular import
        from filters.entities import EntityIter

        for entity in EntityIter(classname):
            return entity

        return None

    @classmethod
    def find_or_create(cls, classname):
        """Try to find an entity with the given classname.

        If no entity has been found, it will be created.

        :param str classname: The classname of the entity.
        :return: Return the found or created entity.
        :rtype: Entity
        """
        entity = cls.find(classname)
        if entity is None:
            entity = cls.create(classname)

        return entity

    def spawn(self):
        """Spawn the entity."""
        spawn_entity(self.index)

    @classmethod
    def _obj(cls, ptr):
        """Return an entity instance of the given pointer."""
        return cls(index_from_pointer(ptr))

    @property
    def _size(self):
        """Return the entity's size."""
        return self.factory.size

    @property
    def factory(self):
        """Return the entity's factory."""
        return factory_dictionary.find_factory(self.classname)

    @property
    def index(self):
        """Return the entity's index."""
        return self._index

    @property
    def edict(self):
        """Return the entity's :class:`entities.Edict` instance."""
        if self._edict is None:
            edict = edict_from_index(self.index)
            object.__setattr__(self, '_edict', edict)
        return self._edict

    @property
    def pointer(self):
        """Return the entity's :class:`memory.Pointer`."""
        if self._pointer is None:
            pointer = memory.get_object_pointer(self)
            object.__setattr__(self, '_pointer', pointer)
        return self._pointer

    @property
    def instances(self):
        """Yield the entity's base instances.

        Values yielded are the entity's :class:`entities.Edict`
        and :class:`memory.Pointer` objects.
        """
        yield self.edict
        yield self.pointer

    @property
    def inthandle(self):
        """Return the entity's integer handle."""
        return self.basehandle.to_int()

    @property
    def server_class(self):
        """Return the entity's server class."""
        return self.edict.networkable.get_server_class()

    @property
    def server_classes(self):
        """Yield all server classes for the entity."""
        # Loop through all server classes for the entity
        for server_class in server_classes.get_entity_server_classes(self):

            # Yield the server class
            yield server_class

    @property
    def properties(self):
        """Iterate over all descriptors available for the entity.

        This property is a helper for scripters
            to know what descriptors an entity has available.
        """
        # Loop through each server class for the entity
        for server_class in self.server_classes:

            # Loop through the server class' descriptors
            for name in server_class.properties:

                # Yield the descriptor
                yield name

    @property
    def inputs(self):
        """Iterate over all inputs available for the entity.

        This property is a helper for scripters
            to know what inputs an entity has available.
        """
        # Loop through each server class for the entity
        for server_class in self.server_classes:

            # Loop through the server class' inputs
            for name in server_class.inputs:

                # Yield the input
                yield name

    @property
    def outputs(self):
        """Iterate over all outputs available for the entity.

        This property is a helper for scripters
            to know what outputs an entity has available.
        """
        # Loop through each server class for the entity
        for server_class in self.server_classes:

            # Loop through the server class' outputs
            for name in server_class.outputs:

                # Yield the input
                yield name

    @property
    def keyvalues(self):
        """Iterate over all entity keyvalues available for the entity.

        This property is a helper for scripters
            to know what keyvalues an entity has available.
        """
        # Loop through each server class for the entity
        for server_class in self.server_classes:

            # Loop through the server class' keyvalues
            for keyvalue in server_class.keyvalues:

                # Yield the keyvalue
                yield keyvalue

    def get_color(self):
        """Return the entity's color as a Color instance."""
        return self.render_color

    def set_color(self, color):
        """Set the entity's color."""
        # Set the entity's render mode
        self.render_mode |= RenderMode.TRANS_COLOR

        # Set the entity's color
        self.render_color = color

        # Set the entity's alpha
        self.render_amt = color.a

    # Set the "color" property for Entity
    color = property(
        get_color, set_color,
        doc="""Property to get/set the entity's color values.""")

    def get_model(self):
        """Return the entity's model."""
        return Model(self.get_model_name())

    def set_model(self, model):
        """Set the entity's model to the given model."""
        self.set_model_index(model.index)
        self.set_key_value_string('model', model.path)

    model = property(
        get_model, set_model,
        doc="""Property to get/set the entity's model.""")

    @property
    def model_header(self):
        """Return a ModelHeader instance of the current entity's model."""
        return model_cache.get_model_header(model_cache.find_model(
            self.model.path))

    def get_property_bool(self, name):
        """Return the boolean property."""
        return self._get_property(name, 'bool')

    def get_property_color(self, name):
        """Return the Color property."""
        return self._get_property(name, 'Color')

    def get_property_edict(self, name):
        """Return the Edict property."""
        return self._get_property(name, 'Edict')

    def get_property_float(self, name):
        """Return the float property."""
        return self._get_property(name, 'float')

    def get_property_int(self, name):
        """Return the integer property."""
        return self._get_property(name, 'int')

    def get_property_interval(self, name):
        """Return the Interval property."""
        return self._get_property(name, 'Interval')

    def get_property_pointer(self, name):
        """Return the pointer property."""
        return self._get_property(name, 'pointer')

    def get_property_quaternion(self, name):
        """Return the Quaternion property."""
        return self._get_property(name, 'Quaternion')

    def get_property_short(self, name):
        """Return the short property."""
        return self._get_property(name, 'short')

    def get_property_string(self, name):
        """Return the string property."""
        return self._get_property(name, 'string_array')

    def get_property_string_pointer(self, name):
        """Return the string property."""
        return self._get_property(name, 'string_pointer')

    def get_property_uchar(self, name):
        """Return the uchar property."""
        return self._get_property(name, 'uchar')

    def get_property_uint(self, name):
        """Return the uint property."""
        return self._get_property(name, 'uint')

    def get_property_vector(self, name):
        """Return the Vector property."""
        return self._get_property(name, 'Vector')

    def _get_property(self, name, prop_type):
        """Verify the type and return the property."""
        # Loop through all entity server classes
        for server_class in self.server_classes:

            # Is the name a member of the current server class?
            if name not in server_class.properties:
                continue

            # Is the type the correct type?
            if prop_type != server_class.properties[name].prop_type:
                raise TypeError('Property "{0}" is of type {1} not {2}'.format(
                    name, server_class.properties[name].prop_type, prop_type))

            # Return the property for the entity
            return getattr(
                make_object(server_class._properties, self.pointer), name)

        # Raise an error if the property name was not found
        raise ValueError(
            'Property "{0}" not found for entity type "{1}"'.format(
                name, self.classname))

    def set_property_bool(self, name, value):
        """Set the boolean property."""
        self._set_property(name, 'bool', value)

    def set_property_color(self, name, value):
        """Set the Color property."""
        self._set_property(name, 'Color', value)

    def set_property_edict(self, name, value):
        """Set the Edict property."""
        self._set_property(name, 'Edict', value)

    def set_property_float(self, name, value):
        """Set the float property."""
        self._set_property(name, 'float', value)

    def set_property_int(self, name, value):
        """Set the integer property."""
        self._set_property(name, 'int', value)

    def set_property_interval(self, name, value):
        """Set the Interval property."""
        self._set_property(name, 'Interval', value)

    def set_property_pointer(self, name, value):
        """Set the pointer property."""
        self._set_property(name, 'pointer', value)

    def set_property_quaternion(self, name, value):
        """Set the Quaternion property."""
        self._set_property(name, 'Quaternion', value)

    def set_property_short(self, name, value):
        """Set the short property."""
        self._set_property(name, 'short', value)

    def set_property_string(self, name, value):
        """Set the string property."""
        self._set_property(name, 'string_array', value)

    def set_property_string_pointer(self, name, value):
        """Set the string property."""
        self._set_property(name, 'string_pointer', value)

    def set_property_uchar(self, name, value):
        """Set the uchar property."""
        self._set_property(name, 'uchar', value)

    def set_property_uint(self, name, value):
        """Set the uint property."""
        self._set_property(name, 'uint', value)

    def set_property_vector(self, name, value):
        """Set the Vector property."""
        self._set_property(name, 'Vector', value)

    def _set_property(self, name, prop_type, value):
        """Verify the type and set the property."""
        # Loop through all entity server classes
        for server_class in self.server_classes:

            # Is the name a member of the current server class?
            if name not in server_class.properties:
                continue

            # Is the type the correct type?
            if prop_type != server_class.properties[name].prop_type:
                raise TypeError('Property "{0}" is of type {1} not {2}'.format(
                    name, server_class.properties[name].prop_type, prop_type))

            # Set the property for the entity
            setattr(make_object(
                server_class._properties, self.pointer), name, value)

            # Is the property networked?
            if server_class.properties[name].networked:

                # Notify the change of state
                self.edict.state_changed()

            # No need to go further
            return

        # Raise an error if the property name was not found
        raise ValueError(
            'Property "{0}" not found for entity type "{1}"'.format(
                name, self.classname))

    def get_input(self, name):
        """Return the InputFunction instance for the given name."""
        # Loop through each server class for the entity
        for server_class in self.server_classes:

            # Does the current server class contain the input?
            if name in server_class.inputs:

                # Return the InputFunction instance for the given input name
                return getattr(
                    make_object(server_class._inputs, self.pointer), name)

        # If no server class contains the input, raise an error
        raise ValueError(
            'Unknown input "{0}" for entity type "{1}".'.format(
                name, self.classname))

    def call_input(self, name, *args, **kwargs):
        """Call the InputFunction instance for the given name."""
        self.get_input(name)(*args, **kwargs)

    def lookup_attachment(self, name):
        """Return the attachment index matching the given name."""
        # Get the ModelHeader instance of the entity
        model_header = self.model_header

        # Loop through all attachments
        for index in range(model_header.attachments_count):

            # Are the names matching?
            if name == model_header.get_attachment(index).name:

                # Return the current index
                return index

        # No attachment found
        return INVALID_ATTACHMENT_INDEX

    def is_in_solid(
            self, mask=ContentMasks.ALL, generator=BaseEntityGenerator):
        """Return whether or not the entity is in solid."""
        # Get a Ray object of the entity physic box
        ray = Ray(self.origin, self.origin, self.mins, self.maxs)

        # Get a new GameTrace instance
        trace = GameTrace()

        # Do the trace
        engine_trace.trace_ray(ray, mask, TraceFilterSimple(
            [entity.index for entity in generator()]), trace)

        # Return whether or not the trace did hit
        return trace.did_hit()
