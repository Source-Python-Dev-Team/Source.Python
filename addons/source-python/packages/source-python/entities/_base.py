# ../entities/_base.py

"""Provides a base class to interact with a specific entity."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Python Imports
#   Collections
from collections import defaultdict
#   Contextlib
from contextlib import suppress

# Source.Python Imports
#   Core
from core import GAME_NAME
#   Entities
from entities.constants import INVALID_ENTITY_INDEX
#   Engines
from engines.precache import Model
from engines.sound import Attenuation
from engines.sound import engine_sound
from engines.sound import Channel
from engines.sound import Pitch
from engines.sound import Sound
from engines.sound import SoundFlags
from engines.sound import StreamSound
from engines.sound import VOL_NORM
from engines.trace import engine_trace
from engines.trace import ContentMasks
from engines.trace import GameTrace
from engines.trace import Ray
from engines.trace import TraceFilterSimple
#   Entities
from _entities._entity import BaseEntity
from entities import BaseEntityGenerator
from entities import TakeDamageInfo
from entities.classes import server_classes
from entities.constants import DamageTypes
from entities.constants import RenderMode
from entities.helpers import index_from_inthandle
from entities.helpers import index_from_pointer
from entities.helpers import wrap_entity_mem_func
#   Filters
from filters.weapons import WeaponClassIter
#   Listeners
from listeners import OnEntityDeleted
from listeners.tick import Delay
#   Mathlib
from mathlib import NULL_VECTOR
#   Memory
from memory import make_object
#   Players
from players.constants import HitGroup
#   Studio
from studio.cache import model_cache
from studio.constants import INVALID_ATTACHMENT_INDEX


# =============================================================================
# >> GLOBAL VARIABLES
# =============================================================================
# Get a list of projectiles for the game
_projectile_weapons = [weapon.name for weapon in WeaponClassIter('grenade')]

# Get a dictionary to store the delays
_entity_delays = defaultdict(set)


# =============================================================================
# >> CLASSES
# =============================================================================
class Entity(BaseEntity):
    """Class used to interact directly with entities.

    Beside the standard way of doing stuff via methods and properties this
    class also provides dynamic attributes that depend on the entity that is
    being accessed with this class. You can print all dynamic properties by
    iterating over the following generators:

    1. :attr:`properties`
    2. :attr:`inputs`
    3. :attr:`outputs`
    4. :attr:`keyvalues`
    """

    def __init__(self, index):
        """Initialize the Entity instance.

        :param int index:
            The entity index to wrap.
        """
        # Initialize the object
        super().__init__(index)

        # Set the entity's base attributes
        object.__setattr__(self, '_index', index)

    def __hash__(self):
        """Return a hash value based on the entity index."""
        # Required for sets, because we have implemented __eq__
        return hash(self.index)

    def __getattr__(self, attr):
        """Find if the attribute is valid and returns the appropriate value."""
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
        """Create a new networked entity with the given classname.

        :param str classname:
            Classname of the entity to create.
        :raise ValueError:
            Raised if the given classname is not a networked entity.
        """
        entity = BaseEntity.create(classname)
        if entity.is_networked():
            return cls(entity.index)

        entity.remove()
        raise ValueError('"{}" is not a networked entity.'.format(classname))

    @classmethod
    def find(cls, classname):
        """Try to find an entity with the given classname.

        If not entity has been found, None will be returned.

        :param str classname:
            The classname of the entity.
        :return:
            Return the found entity.
        :rtype: Entity
        """
        entity = BaseEntity.find(classname)
        if entity is not None and entity.is_networked():
            return cls(entity.index)

        return None

    @classmethod
    def find_or_create(cls, classname):
        """Try to find an entity with the given classname.

        If no entity has been found, it will be created.

        :param str classname:
            The classname of the entity.
        :return:
            Return the found or created entity.
        :rtype: Entity
        """
        entity = cls.find(classname)
        if entity is None:
            entity = cls.create(classname)

        return entity

    @classmethod
    def _obj(cls, ptr):
        """Return an entity instance of the given pointer."""
        return cls(index_from_pointer(ptr))

    @property
    def index(self):
        """Return the entity's index.

        :rtype: int
        """
        return self._index

    @property
    def owner(self):
        """Return the entity's owner.

        :return:
            None if the entity has no owner.
        :rtype: Entity
        """
        try:
            return Entity(index_from_inthandle(self.owner_handle))
        except (ValueError, OverflowError):
            return None

    @property
    def server_classes(self):
        """Yield all server classes for the entity."""
        yield from server_classes.get_entity_server_classes(self)

    @property
    def properties(self):
        """Iterate over all descriptors available for the entity."""
        for server_class in self.server_classes:
            yield from server_class.properties

    @property
    def inputs(self):
        """Iterate over all inputs available for the entity."""
        for server_class in self.server_classes:
            yield from server_class.inputs

    @property
    def outputs(self):
        """Iterate over all outputs available for the entity."""
        for server_class in self.server_classes:
            yield from server_class.outputs

    @property
    def keyvalues(self):
        """Iterate over all entity keyvalues available for the entity.

        .. note::

            An entity might also have hardcoded keyvalues that can't be listed
            with this property.
        """
        for server_class in self.server_classes:
            yield from server_class.keyvalues

    def get_model(self):
        """Return the entity's model.

        :return:
            ``None`` if the entity has no model.
        :rtype: Model
        """
        if not self.model_name:
            return None

        return Model(self.model_name)

    def set_model(self, model):
        """Set the entity's model to the given model.

        :param Model model:
            The model to set.
        """
        self.model_index = model.index
        self.model_name = model.path

    model = property(
        get_model, set_model,
        doc="""Property to get/set the entity's model.

        .. seealso:: :meth:`get_model` and :meth:`set_model`""")

    def get_parent(self):
        """Return the entity's parent.

        :rtype: Entity
        """
        # Get the parent handle...
        parent_inthandle = self.parent_inthandle

        # Does the entity have no parent?
        if parent_inthandle == -1:
            return None

        # Return the parent of the entity...
        return Entity(index_from_inthandle(parent_inthandle))

    def _set_parent(self, parent):
        """Set the parent of the entity.

        :param Entity parent:
            The parent of the entity. If None, actual parent will be cleared.
        """
        # Clear the actual parent if None was given...
        if parent is None:
            self.clear_parent()

        # Set the parent of the entity...
        self.set_parent(parent)

    parent = property(
        get_parent, _set_parent,
        doc="""Property to get/set the parent of the entity.

        .. seealso:: :meth:`get_parent` and :meth:`set_parent`""")

    def get_property_bool(self, name):
        """Return the boolean property.

        :param str name:
            Name of the property to retrieve.
        :rtype: bool
        """
        return self._get_property(name, 'bool')

    def get_property_color(self, name):
        """Return the Color property.

        :param str name:
            Name of the property to retrieve.
        :rtype: Color
        """
        return self._get_property(name, 'Color')

    def get_property_edict(self, name):
        """Return the Edict property.

        :param str name:
            Name of the property to retrieve.
        :rtype: Edict
        """
        return self._get_property(name, 'Edict')

    def get_property_float(self, name):
        """Return the float property.

        :param str name:
            Name of the property to retrieve.
        :rtype: float
        """
        return self._get_property(name, 'float')

    def get_property_int(self, name):
        """Return the integer property.

        :param str name:
            Name of the property to retrieve.
        :rtype: int
        """
        return self._get_property(name, 'int')

    def get_property_interval(self, name):
        """Return the Interval property.

        :param str name:
            Name of the property to retrieve.
        :rtype: Interval
        """
        return self._get_property(name, 'Interval')

    def get_property_pointer(self, name):
        """Return the pointer property.

        :param str name:
            Name of the property to retrieve.
        :rtype: Pointer
        """
        return self._get_property(name, 'pointer')

    def get_property_quaternion(self, name):
        """Return the Quaternion property.

        :param str name:
            Name of the property to retrieve.
        :rtype: Quaternion
        """
        return self._get_property(name, 'Quaternion')

    def get_property_short(self, name):
        """Return the short property.

        :param str name:
            Name of the property to retrieve.
        :rtype: int
        """
        return self._get_property(name, 'short')

    def get_property_ushort(self, name):
        """Return the ushort property.

        :param str name:
            Name of the property to retrieve.
        :rtype: int
        """
        return self._get_property(name, 'ushort')

    def get_property_string(self, name):
        """Return the string property.

        :param str name:
            Name of the property to retrieve.
        :rtype: str
        """
        return self._get_property(name, 'string_array')

    def get_property_string_pointer(self, name):
        """Return the string property.

        :param str name:
            Name of the property to retrieve.
        :rtype: str
        """
        return self._get_property(name, 'string_pointer')

    def get_property_char(self, name):
        """Return the char property.

        :param str name:
            Name of the property to retrieve.
        :rtype: str
        """
        return self._get_property(name, 'char')

    def get_property_uchar(self, name):
        """Return the uchar property.

        :param str name:
            Name of the property to retrieve.
        :rtype: int
        """
        return self._get_property(name, 'uchar')

    def get_property_uint(self, name):
        """Return the uint property.

        :param str name:
            Name of the property to retrieve.
        :rtype: int
        """
        return self._get_property(name, 'uint')

    def get_property_vector(self, name):
        """Return the Vector property.

        :param str name:
            Name of the property to retrieve.
        :rtype: Vector
        """
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
        """Set the boolean property.

        :param str name:
            Name of the property to set.
        :param bool value:
            The value to set.
        """
        self._set_property(name, 'bool', value)

    def set_property_color(self, name, value):
        """Set the Color property.

        :param str name:
            Name of the property to set.
        :param Color value:
            The value to set.
        """
        self._set_property(name, 'Color', value)

    def set_property_edict(self, name, value):
        """Set the Edict property.

        :param str name:
            Name of the property to set.
        :param Edict value:
            The value to set.
        """
        self._set_property(name, 'Edict', value)

    def set_property_float(self, name, value):
        """Set the float property.

        :param str name:
            Name of the property to set.
        :param float value:
            The value to set.
        """
        self._set_property(name, 'float', value)

    def set_property_int(self, name, value):
        """Set the integer property.

        :param str name:
            Name of the property to set.
        :param int value:
            The value to set.
        """
        self._set_property(name, 'int', value)

    def set_property_interval(self, name, value):
        """Set the Interval property.

        :param str name:
            Name of the property to set.
        :param Interval value:
            The value to set.
        """
        self._set_property(name, 'Interval', value)

    def set_property_pointer(self, name, value):
        """Set the pointer property.

        :param str name:
            Name of the property to set.
        :param Pointer value:
            The value to set.
        """
        self._set_property(name, 'pointer', value)

    def set_property_quaternion(self, name, value):
        """Set the Quaternion property.

        :param str name:
            Name of the property to set.
        :param Quaternion value:
            The value to set.
        """
        self._set_property(name, 'Quaternion', value)

    def set_property_short(self, name, value):
        """Set the short property.

        :param str name:
            Name of the property to set.
        :param int value:
            The value to set.
        """
        self._set_property(name, 'short', value)

    def set_property_ushort(self, name, value):
        """Set the ushort property.

        :param str name:
            Name of the property to set.
        :param int value:
            The value to set.
        """
        self._set_property(name, 'ushort', value)

    def set_property_string(self, name, value):
        """Set the string property.

        :param str name:
            Name of the property to set.
        :param str value:
            The value to set.
        """
        self._set_property(name, 'string_array', value)

    def set_property_string_pointer(self, name, value):
        """Set the string property.

        :param str name:
            Name of the property to set.
        :param str value:
            The value to set.
        """
        self._set_property(name, 'string_pointer', value)

    def set_property_char(self, name, value):
        """Set the char property.

        :param str name:
            Name of the property to set.
        :param str value:
            The value to set.
        """
        self._set_property(name, 'char', value)

    def set_property_uchar(self, name, value):
        """Set the uchar property.

        :param str name:
            Name of the property to set.
        :param int value:
            The value to set.
        """
        self._set_property(name, 'uchar', value)

    def set_property_uint(self, name, value):
        """Set the uint property.

        :param str name:
            Name of the property to set.
        :param int value:
            The value to set.
        """
        self._set_property(name, 'uint', value)

    def set_property_vector(self, name, value):
        """Set the Vector property.

        :param str name:
            Name of the property to set.
        :param Vector value:
            The value to set.
        """
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

    def delay(
            self, delay, callback, args=(), kwargs=None,
            cancel_on_level_end=False):
        """Execute a callback after the given delay.

        :param float delay:
            The delay in seconds.
        :param callback:
            A callable object that should be called after the delay expired.
        :param tuple args:
            Arguments that should be passed to the callback.
        :param dict kwargs:
            Keyword arguments that should be passed to the callback.
        :param bool cancel_on_level_end:
            Whether or not to cancel the delay at the end of the map.
        :raise ValueError:
            If the given callback is not callable.
        :return:
            The delay instance.
        :rtype: Delay
        """
        # TODO: Ideally, we want to subclass Delay and cleanup on cancel() too
        #   in case the caller manually cancel the returned Delay.
        def _callback(*args, **kwargs):
            """Called when the delay is executed."""
            # Remove the delay from the global dictionary...
            _entity_delays[self.index].remove(delay)

            # Was this the last pending delay for the entity?
            if not _entity_delays[self.index]:

                # Remove the entity from the dictionary...
                del _entity_delays[self.index]

            # Call the callback...
            callback(*args, **kwargs)

        # Get the delay instance...
        delay = Delay(delay, _callback, args, kwargs, cancel_on_level_end)

        # Add the delay to the dictionary...
        _entity_delays[self.index].add(delay)

        # Return the delay instance...
        return delay

    def get_input(self, name):
        """Return the input function matching the given name.

        :parma str name:
            Name of the input function.
        :rtype: InputFunction
        :raise ValueError:
            Raised if the input function wasn't found.
        """
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
        """Call the input function matching the given name.

        :param str name:
            Name of the input function.
        :param args:
            Optional arguments that should be passed to the input function.
        :param kwargs:
            Optional keyword arguments that should be passed to the input
            function.
        :raise ValueError:
            Raised if the input function wasn't found.
        """
        self.get_input(name)(*args, **kwargs)

    def emit_sound(
            self, sample, recipients=(), volume=VOL_NORM,
            attenuation=Attenuation.NONE, channel=Channel.AUTO,
            flags=SoundFlags.NO_FLAGS, pitch=Pitch.NORMAL, origin=NULL_VECTOR,
            direction=NULL_VECTOR, origins=(), update_positions=True,
            sound_time=0.0, speaker_entity=INVALID_ENTITY_INDEX,
            download=False, stream=False):
        """Emit a sound from this entity.

        :param str sample:
            Sound file relative to the ``sounds`` directory.
        :param RecipientFilter recipients:
            Recipients to emit the sound to.
        :param int index:
            Index of the entity to emit the sound from.
        :param float volume:
            Volume of the sound.
        :param Attenuation attenuation:
            How far the sound should reaches.
        :param int channel:
            Channel to emit the sound with.
        :param SoundFlags flags:
            Flags of the sound.
        :param Pitch pitch:
            Pitch of the sound.
        :param Vector origin:
            Origin of the sound.
        :param Vector direction:
            Direction of the sound.
        :param tuple origins:
            Origins of the sound.
        :param bool update_positions:
            Whether or not the positions should be updated.
        :param float sound_time:
            Time to play the sound for.
        :param int speaker_entity:
            Index of the speaker entity.
        :param bool download:
            Whether or not the sample should be added to the downloadables.
        :param bool stream:
            Whether or not the sound should be streamed.
        """
        # Get the correct Sound class...
        if not stream:
            sound_class = Sound
        else:
            sound_class = StreamSound

        # Get the sound...
        sound = sound_class(sample, self.index, volume, attenuation, channel,
            flags, pitch, origin, direction, origins, update_positions,
            sound_time, speaker_entity, download)

        # Make sure we have a tuple as recipients...
        if not isinstance(recipients, tuple):
            recipients = (recipients,)

        # Emit the sound to the given recipients...
        sound.play(*recipients)

    def is_in_solid(
            self, mask=ContentMasks.ALL, generator=BaseEntityGenerator):
        """Return whether or not the entity is in solid.

        :param ContentMasks mask:
            Contents the ray can possibly collide with.
        :param generator:
            A callable that returns an iterable which contains
            :class:`BaseEntity` instances that are ignored by the ray.
        :rtype: bool
        """
        # Get a Ray object of the entity physic box
        ray = Ray(self.origin, self.origin, self.mins, self.maxs)

        # Get a new GameTrace instance
        trace = GameTrace()

        # Do the trace
        engine_trace.trace_ray(ray, mask, TraceFilterSimple(
            generator()), trace)

        # Return whether or not the trace did hit
        return trace.did_hit()

    def take_damage(
            self, damage, damage_type=DamageTypes.GENERIC, attacker_index=None,
            weapon_index=None, hitgroup=HitGroup.GENERIC, skip_hooks=False,
            **kwargs):
        """Deal damage to the entity.

        :param int damage:
            Amount of damage to deal.
        :param DamageTypes damage_type:
            Type of the dealed damage.
        :param int attacker_index:
            If not None, the index will be used as the attacker.
        :param int weapon_index:
            If not None, the index will be used as the weapon. This method
            also tries to retrieve the attacker from the weapon, if
            ``attacker_index`` wasn't set.
        :param HitGroup hitgroup:
            The hitgroup where the damage should be applied.
        :param bool skip_hooks:
            If True, the damage will be dealed directly by skipping any
            registered hooks.
        """
        # Import Entity classes
        # Doing this in the global scope causes cross import errors
        from weapons.entity import Weapon

        # Is the game supported?
        if not hasattr(self, 'on_take_damage'):

            # Raise an error if not supported
            raise NotImplementedError(
                '"take_damage" is not implemented for {0}'.format(GAME_NAME))

        # Store values for later use
        attacker = None
        weapon = None

        # Was an attacker given?
        if attacker_index is not None:

            # Try to get the Entity instance of the attacker
            with suppress(ValueError):
                attacker = Entity(attacker_index)

        # Was a weapon given?
        if weapon_index is not None:

            # Try to get the Weapon instance of the weapon
            with suppress(ValueError):
                weapon = Weapon(weapon_index)

        # Is there a weapon but no attacker?
        if attacker is None and weapon is not None:

            # Try to get the attacker based off of the weapon's owner
            with suppress(ValueError, OverflowError):
                attacker_index = index_from_inthandle(weapon.owner_handle)
                attacker = Entity(attacker_index)

        # Is there an attacker but no weapon?
        if attacker is not None and weapon is None:

            # Try to use the attacker's active weapon
            with suppress(AttributeError):
                weapon = attacker.active_weapon

        # Try to set the hitgroup
        with suppress(AttributeError):
            self.hitgroup = hitgroup

        # Get a TakeDamageInfo instance
        take_damage_info = TakeDamageInfo()

        # Is there a valid weapon?
        if weapon is not None:

            # Is the weapon a projectile?
            if weapon.classname in _projectile_weapons:

                # Set the inflictor to the weapon's index
                take_damage_info.inflictor = weapon.index

            # Is the weapon not a projectile and the attacker is valid?
            elif attacker_index is not None:

                # Set the inflictor to the attacker's index
                take_damage_info.inflictor = attacker_index

            # Set the weapon to the weapon's index
            take_damage_info.weapon = weapon.index

        # Is the attacker valid?
        if attacker_index is not None:

            # Set the attacker to the attacker's index
            take_damage_info.attacker = attacker_index

        # Set the damage amount
        take_damage_info.damage = damage

        # Set the damage type value
        take_damage_info.type = damage_type

        # Loop through the given keywords
        for item in kwargs:

            # Set the offset's value
            setattr(take_damage_info, item, kwargs[item])

        if skip_hooks:
            self.on_take_damage.skip_hooks(take_damage_info)
        else:
            self.on_take_damage(take_damage_info)

    @wrap_entity_mem_func
    def teleport(self, origin=None, angle=None, velocity=None):
        """Change the origin, angle and/or velocity of the entity.

        :param Vector origin:
            New location of the entity.
        :param QAngle angle:
            New angle of the entity.
        :param Vector velocity:
            New velocity of the entity.
        """
        return [origin, angle, velocity]

    @wrap_entity_mem_func
    def set_parent(self, parent, attachment=INVALID_ATTACHMENT_INDEX):
        """Set the parent of the entity.

        :param Pointer parent:
            The parent.
        :param str attachment:
            The attachment name/index.
        """
        if not isinstance(attachment, int):
            attachment = self.lookup_attachment(attachment)

        return [parent, attachment]


# =============================================================================
# >> LISTENERS
# =============================================================================
@OnEntityDeleted
def _on_entity_deleted(base_entity):
    """Called when an entity is removed.

    :param BaseEntity base_entity:
        The removed entity.
    """
    # Make sure the entity is networkable...
    if not base_entity.is_networked():
        return

    # Get the index of the entity...
    index = base_entity.index

    # Was no delay registered for this entity?
    if index not in _entity_delays:
        return

    # Loop through all delays...
    for delay in _entity_delays[index]:

        # Make sure the delay is still running...
        if not delay.running:
            continue

        # Cancel the delay...
        delay.cancel()

    # Remove the entity from the dictionary...
    del _entity_delays[index]
