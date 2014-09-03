# ../entities/entity.py

"""Provides a base class to interact with a specific entity."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
#   Entities
from entities.classes import _server_classes
from entities.helpers import edict_from_index
from entities.helpers import pointer_from_edict
from entities.specials import _EntitySpecials
#   Memory
from memory import make_object


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
# Add all the global variables to __all__
__all__ = ('BaseEntity',
           )


# =============================================================================
# >> CLASSES
# =============================================================================
class BaseEntity(_EntitySpecials):

    """Class used to interact directly with entities."""

    def __new__(cls, index):
        """Verify the given index is valid and store base attributes."""
        # Get the given indexes edict
        edict = edict_from_index(index)

        # Is the edict valid?
        if not edict or edict.is_free():

            # If not raise an error
            raise ValueError(
                'Index "{0}" is not a proper entity index'.format(index))

        # Create the object
        self = object.__new__(cls)

        # Set the entity's base attributes
        self._index = index
        self._edict = edict
        self._pointer = pointer_from_edict(edict)

        # Return the instance
        return self

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
                return getattr(server_class(self.pointer, wrap=True), attr)

        # If the attribute is not found, raise an error
        raise AttributeError('Attribute "{0}" not found'.format(attr))

    def __setattr__(self, attr, value):
        """Find if the attribute is value and sets its value."""
        # Is the given attribute private?
        if attr.startswith('_'):

            # Get the name of the private attribute
            name = attr[1:]

            # Is the attribute a property?
            if (name in super(BaseEntity, self).__dir__() and isinstance(
                    getattr(self.__class__, name), property)):

                # Set the private attribute's value
                super(BaseEntity, self).__setattr__(attr, value)

                # No need to go further
                return

            # If not a property, do not allow the private attribute
            raise ValueError(
                'Invalid private attribute "{0}" given.'.format(attr))

        # Loop through all of the entity's server classes
        for server_class in self.server_classes:

            # Does the current server class contain the given attribute?
            if hasattr(server_class, attr):

                # Set the attribute's value
                setattr(server_class(self.pointer, wrap=True), attr, value)

        # If the attribute is not found, just set the attribute
        super(BaseEntity, self).__setattr__(attr, value)

    def __dir__(self):
        """Return an alphabetized list of attributes for the instance."""
        # Get the base attributes
        attributes = set(super(BaseEntity, self).__dir__())

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

    @property
    def index(self):
        """Return the entity's index."""
        return self._index

    @property
    def edict(self):
        """Return the entity's edict instance."""
        return self._edict

    @property
    def instances(self):
        """Yield the entity's base instances."""
        yield self.edict
        yield self.pointer

    @property
    def basehandle(self):
        """Return the entity's BaseEntityHandle instance."""
        return self.get_networkable().get_entity_handle().get_ref_ehandle()

    @property
    def inthandle(self):
        """Return the entity's integer handle."""
        return self.basehandle.to_int()

    @property
    def classname(self):
        """Return the classname of the entity."""
        return self.edict.get_class_name()

    @property
    def pointer(self):
        """Return the entity's pointer."""
        return self._pointer

    @property
    def server_class(self):
        """Return the entity's server class."""
        return self.edict.get_networkable().get_server_class()

    @property
    def server_classes(self):
        """Loop through all server classes for the entity."""
        # Get the first server class for the entity.
        server_class = _server_classes.get_start_server_class(
            self.server_class, self.pointer)

        # Use the current server class to get its base
        #   until there are no more base classes to return.
        while server_class in _server_classes:
            yield _server_classes[server_class]
            try:
                server_class = _server_classes[
                    server_class]._base_class.class_name
            except AttributeError:
                break

    def get_color(self):
        """Return a 4 part tuple (RGBA) for the entity's color."""
        # Get the render value
        value = self.render

        # Return a tuple with the RGBA values
        return (
            value & 0xff, (value & 0xff00) >> 8,
            (value & 0xff0000) >> 16, (value & 0xff000000) >> 24)

    def set_color(self, args):
        """Set the entity's color to the given RGBA values."""
        # Are the the correct number of arguments?
        if not len(args) in (3, 4):

            # Raise an error
            raise TypeError(
                'set_color() requires 3 or 4 ' +
                'arguments, {0} were given'.format(len(args)))

        # Get the RGB values
        red, green, blue = args[:3]

        # Get the value using the RGB values
        value = red + (green << 8) + (blue << 16)

        # Was an alpha value passed?
        if len(args) == 3:

            # Get the current alpha value
            alpha = (self.render & 0xff000000)

        # Otherwise
        else:

            # Get the 4th value passed
            alpha = args[3]

        # Add the alpha value to the RGB value
        value += alpha << 24

        # Set the rendermode
        self.rendermode = self.rendermode | 1

        # Set the renderfx
        self.renderfx = self.renderfx | 256

        # Set the entity's color
        self.render = value

    # Set the "color" property for BaseEntity
    color = property(get_color, set_color)

    def get_descriptor(self, name):
        """Get the entity's value of the given descriptor."""
        # Loop through each server class for the entity
        for server_class in self.server_classes:

            # Does the current server class contain the descriptor?
            if hasattr(server_class._descriptors, name):

                # Return the descriptor's value for the entity
                return getattr(
                    make_object(server_class._descriptors, self.pointer), name)

        # If no server class contains the descriptor, raise an error
        raise ValueError(
            'Unknown descriptor "{0}" for entity type "{1}".'.format(
                name, self.classname))

    def set_descriptor(self, name, value):
        """Set the entity's value for the given descriptor."""
        # Loop through each server class for the entity
        for server_class in self.server_classes:

            # Does the current server class contain the descriptor?
            if hasattr(server_class._descriptors, name):

                # Set the descriptor's value for the entity and return
                setattr(make_object(
                    server_class._descriptors, self.pointer), name, value)
                return

        # If no server class contains the descriptor, raise an error
        raise ValueError(
            'Unknown descriptor "{0}" for entity type "{1}".'.format(
                name, self.classname))

    def get_input(self, name):
        """Return the InputFunction instance for the given name."""
        # Loop through each server class for the entity
        for server_class in self.server_classes:

            # Does the current server class contain the input?
            if hasattr(server_class._inputs, name):

                # Return the InputFunction instance for the given input name
                return getattr(
                    make_object(server_class._inputs, self.pointer), name)

        # If no server class contains the input, raise an error
        raise ValueError(
            'Unknown input "{0}" for entity type "{1}".'.format(
                name, self.classname))

    def get_output(self, name):
        """Return the OutputFunction instance for the given name."""
        # Loop through each server class for the entity
        for server_class in self.server_classes:

            # Does the current server class contain the output?
            if hasattr(server_class._outputs, name):

                # Return the OutputFunction instance for the given output name
                return getattr(
                    make_object(server_class._outputs, self.pointer), name)

        # If no server class contains the output, raise an error
        raise ValueError(
            'Unknown output "{0}" for entity type "{1}".'.format(
                name, self.classname))
