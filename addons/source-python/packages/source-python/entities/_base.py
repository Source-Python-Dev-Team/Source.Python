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
#   Inspect
from inspect import signature
#   WeakRef
from weakref import WeakSet

# Source.Python Imports
#   Core
from core import GAME_NAME
from core import BoostPythonClass
from core.cache import cached_property
#   Entities
from entities.constants import INVALID_ENTITY_INDEX
#   Engines
from engines.precache import Model
from engines.sound import Attenuation
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
from entities import TakeDamageInfo
from entities.classes import server_classes
from entities.constants import WORLD_ENTITY_INDEX
from entities.constants import DamageTypes
from entities.helpers import index_from_inthandle
from entities.helpers import index_from_pointer
from entities.helpers import wrap_entity_mem_func
#   Filters
from filters.weapons import WeaponClassIter
#   Listeners
from listeners.tick import Delay
from listeners.tick import Repeat
from listeners.tick import RepeatStatus
#   Mathlib
from mathlib import NULL_VECTOR
#   Memory
from memory import make_object
from memory.helpers import MemberFunction
#   Players
from players.constants import HitGroup
#   Studio
from studio.constants import INVALID_ATTACHMENT_INDEX


# =============================================================================
# >> FORWARD IMPORTS
# =============================================================================
# Source.Python Imports
#   Entities
from _entities._entity import BaseEntity


# =============================================================================
# >> GLOBAL VARIABLES
# =============================================================================
# Get a list of projectiles for the game
_projectile_weapons = [weapon.name for weapon in WeaponClassIter('grenade')]

# Get a dictionary to store the delays
_entity_delays = defaultdict(set)

# Get a dictionary to store the repeats
_entity_repeats = defaultdict(set)

# Get a set to store the registered entity classes
_entity_classes = WeakSet()


# =============================================================================
# >> CLASSES
# =============================================================================
class _EntityCaching(BoostPythonClass):
    """Metaclass used to cache entity instances."""

    def __init__(cls, classname, bases, attributes):
        """Initializes the class."""
        # New instances of this class will be cached in that dictionary
        cls._cache = {}

        # Set whether or not this class is caching its instances by default
        try:
            cls._caching = bool(
                signature(
                    vars(cls)['__init__']
                ).parameters['caching'].default
            )
        except KeyError:
            cls._caching = bool(vars(cls).get('caching', False))

        # Add the class to the registered classes
        _entity_classes.add(cls)

    def __call__(cls, index, caching=None):
        """Called when a new instance of this class is requested.

        :param int index:
            The index of the entity instance requested.
        :param bool caching:
            Whether to lookup the cache for an existing instance or not.
        """
        # Get whether or not we should lookup for a cached instance
        if caching is None:
            caching = cls._caching

        # Let's first lookup for a cached instance
        if caching:
            try:
                return cls._cache[index]
            except KeyError:
                pass

        # Nothing in cache, let's create a new instance
        obj = super().__call__(index)

        # Let's cache the new instance we just created
        if caching:

            # Only cache entities that are not marked for deletion.
            # This is required, because if someone request an entity instance
            # after we invalidated our cache but before the engine processed
            # the deletion we would now have an invalid instance in the cache.
            if not obj.is_marked_for_deletion():
                cls._cache[index] = obj

        # We are done, let's return the instance
        return obj

    @property
    def caching(cls):
        """Returns whether this class is caching its instances by default.

        :rtype: bool
        """
        return cls._caching

    @property
    def cache(cls):
        """Returns the cached instances of this class.

        :rtype: dict
        """
        return cls._cache


class Entity(BaseEntity, metaclass=_EntityCaching):
    """Class used to interact directly with entities.

    Beside the standard way of doing stuff via methods and properties this
    class also provides dynamic attributes that depend on the entity that is
    being accessed with this class. You can print all dynamic properties by
    iterating over the following generators:

    1. :attr:`properties`
    2. :attr:`inputs`
    3. :attr:`outputs`
    4. :attr:`keyvalues`

    :var cache:
        A read-only attribute that returns a dictionary containing the cached
        instances of this class.

        .. note::
            This is not an instance property, so it can only be
            accessed through the class itself.

    :var caching:
        A read-only attribute that returns whether this class is caching its
        instances by default.
    """

    def __init__(self, index, caching=True):
        """Initialize the Entity instance.

        :param int index:
            The entity index to wrap.
        :param bool caching:
            Whether to lookup the cache for an existing instance or not.
        """
        # Initialize the object
        super().__init__(index)

        # Set the entity's base attributes
        type(self).index.set_cached_value(self, index)

    def __hash__(self):
        """Return a hash value based on the entity inthandle."""
        # Required for sets, because we have implemented __eq__
        return hash(self.inthandle)

    def __getattr__(self, attr):
        """Find if the attribute is valid and returns the appropriate value."""
        # Loop through all of the entity's server classes
        for instance in self.server_classes.values():

            try:
                # Get the attribute's value
                value = getattr(instance, attr)
            except AttributeError:
                continue

            # Is the value a dynamic function?
            if isinstance(value, MemberFunction):

                # Cache the value
                with suppress(AttributeError):
                    object.__setattr__(self, attr, value)

            # Return the attribute's value
            return value

        # If the attribute is not found, raise an error
        raise AttributeError('Attribute "{0}" not found'.format(attr))

    def __setattr__(self, attr, value):
        """Find if the attribute is valid and sets its value."""
        # Is the given attribute a property?
        if (attr in super().__dir__() and isinstance(
                getattr(self.__class__, attr, None), property)):

            # Set the property's value
            object.__setattr__(self, attr, value)

            # No need to go further
            return

        # Loop through all of the entity's server classes
        for server_class, instance in self.server_classes.items():

            # Does the current server class contain the given attribute?
            if hasattr(server_class, attr):

                # Set the attribute's value
                setattr(instance, attr, value)

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
    def from_inthandle(cls, inthandle, caching=None):
        """Create an entity instance from an inthandle.

        :param int inthandle:
            The inthandle.
        :param bool caching:
            Whether to lookup the cache for an existing instance or not.
        :rtype: Entity
        """
        return cls(index_from_inthandle(inthandle), caching=caching)

    @classmethod
    def _obj(cls, ptr):
        """Return an entity instance of the given pointer."""
        return cls(index_from_pointer(ptr))

    def is_networked(self):
        """Return True if the entity is networked.

        :rtype: bool
        """
        return True

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

    @cached_property
    def server_classes(self):
        """Yield all server classes for the entity."""
        return {
            server_class: make_object(server_class, self.pointer) for
            server_class in server_classes.get_entity_server_classes(self)
        }

    @cached_property
    def properties(self):
        """Iterate over all descriptors available for the entity."""
        properties = {}
        for server_class in self.server_classes:
            for prop, data in server_class.properties.items():
                properties[prop] = data
        return properties

    @cached_property
    def inputs(self):
        """Iterate over all inputs available for the entity."""
        inputs = {}
        for server_class in self.server_classes.values():
            for input in server_class.inputs:
                inputs[input] = getattr(
                    make_object(
                        server_class._inputs, self.pointer                 
                    ),
                    input
                )
        return inputs

    @cached_property
    def outputs(self):
        """Iterate over all outputs available for the entity."""
        outputs = {}
        for server_class in self.server_classes:
            for output in server_class.outputs:
                outputs[output] = super().get_output(output)
        return outputs

    @cached_property
    def keyvalues(self):
        """Iterate over all entity keyvalues available for the entity.

        .. note::

            An entity might also have hardcoded keyvalues that can't be listed
            with this property.
        """
        keyvalues = {}
        for server_class in self.server_classes:
            for keyvalue, data in server_class.keyvalues.items():
                keyvalues[keyvalue] = data
        return keyvalues

    def get_model(self):
        """Return the entity's model.

        :return:
            ``None`` if the entity has no model.
        :rtype: Model
        """
        model_name = self.model_name
        if not model_name:
            return None

        return Model(model_name)

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

    def delay(
            self, delay, callback, args=(), kwargs=None,
            cancel_on_level_end=False):
        """Create the delay which will be stopped after removing the entity.

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
        # Get the index of the entity
        index = self.index

        # TODO: Ideally, we want to subclass Delay and cleanup on cancel() too
        #   in case the caller manually cancel the returned Delay.
        def _callback(*args, **kwargs):
            """Called when the delay is executed."""
            # Remove the delay from the global dictionary...
            _entity_delays[index].remove(delay)

            # Was this the last pending delay for the entity?
            if not _entity_delays[index]:

                # Remove the entity from the dictionary...
                del _entity_delays[index]

            # Call the callback...
            callback(*args, **kwargs)

        # Get the delay instance...
        delay = Delay(delay, _callback, args, kwargs, cancel_on_level_end)

        # Add the delay to the dictionary...
        _entity_delays[index].add(delay)

        # Return the delay instance...
        return delay

    def repeat(
            self, callback, args=(), kwargs=None,
            cancel_on_level_end=False):
        """Create the repeat which will be stopped after removing the entity.
        :param callback:
            A callable object that should be called at the end of each loop.
        :param tuple args:
            Arguments that should be passed to the callback.
        :param dict kwargs:
            Keyword arguments that should be passed to the callback.
        :param bool cancel_on_level_end:
            Whether or not to cancel the delay at the end of the map.
        :raise ValueError:
            Raised if the given callback is not callable.
        :return:
            The repeat instance.
        :rtype: Repeat
        """

        # Get the repeat instance...
        repeat = Repeat(callback, args, kwargs, cancel_on_level_end)

        # Add the repeat to the dictionary...
        _entity_repeats[self.index].add(repeat)

        # Return the repeat instance...
        return repeat

    def get_output(self, name):
        """Return the output instance matching the given name.

        :parma str name:
            Name of the output.
        :rtype: BaseEntityOutput
        :raise KeyError:
            Raised if the output instance wasn't found.
        """
        return self.outputs[name]

    def get_input(self, name):
        """Return the input function matching the given name.

        :parma str name:
            Name of the input function.
        :rtype: InputFunction
        :raise KeyError:
            Raised if the input function wasn't found.
        """
        return self.inputs[name]

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
            self, mask=ContentMasks.ALL, generator=None):
        """Return whether or not the entity is in solid.

        :param ContentMasks mask:
            Contents the ray can possibly collide with.
        :param generator:
            A callable that returns an iterable which contains
            :class:`BaseEntity` instances that are ignored by the ray.
        :rtype: bool
        """
        # Get the entity's origin
        origin = self.origin

        # Get a Ray object of the entity physic box
        ray = Ray(origin, origin, self.mins, self.maxs)

        # Get a new GameTrace instance
        trace = GameTrace()

        # Do the trace
        if generator is None:

            # No need to trace against anything but the world if we are going
            # to filter out everything regardless.
            engine_trace.clip_ray_to_entity(
                ray, mask, BaseEntity(WORLD_ENTITY_INDEX), trace
            )
        else:
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
# NOTE: This callback is called by sp_main.cpp after all registered entity
#       deletion listeners have been called.
def _on_networked_entity_deleted(index):
    """Called when a networked entity is removed.

    :param int index:
        The removed entity index.
    """
    # Loop through all delays...
    for delay in _entity_delays.pop(index, ()):

        # Cancel the delay...
        with suppress(ValueError):
            delay.cancel()

    # Loop through all repeats...
    for repeat in _entity_repeats.pop(index, ()):

        # Stop the repeat if running
        if repeat.status is RepeatStatus.RUNNING:
            repeat.stop()

    # Invalidate the internal entity caches for this entity
    for cls in _entity_classes:
        cls.cache.pop(index, None)
