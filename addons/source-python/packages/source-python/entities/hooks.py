# ../entities/hooks.py

"""Provides memory hooking functionality for entities."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
#   Core
from core import AutoUnload
#   Entities
#   Memory
from memory.hooks import HookType
#   Filters
from filters.entities import EntityIter
#   Listeners
from listeners import OnNetworkedEntityCreated
#   Entities
from entities.entity import Entity
#   Players
from players.entity import Player


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('EntityCondition',
           'EntityPostHook',
           'EntityPreHook',
           )


# =============================================================================
# >> CLASSES
# =============================================================================
class EntityCondition(object):
    """Store some default entity conditions."""

    @staticmethod
    def is_player(entity):
        """Return ``True`` if the entity is a player.

        :rtype: bool
        """
        return entity.is_player()

    @staticmethod
    def is_not_player(entity):
        """Return ``True`` if the entity is not a player.

        :rtype: bool
        """
        return not entity.is_player()

    @staticmethod
    def is_human_player(entity):
        """Return ``True`` if the entity is a human player.

        :rtype: bool
        """
        return entity.is_player() and Player(entity.index).steamid != 'BOT'

    @staticmethod
    def is_bot_player(entity):
        """Return ``True`` if the entity is a bot.

        :rtype: bool
        """
        return entity.is_player() and Player(entity.index).steamid == 'BOT'

    @staticmethod
    def equals_entity_classname(*classnames):
        """Return a function that requires an :class:`entities.entity.Entity``
        instace. The returned function returns ``True`` if the entity's
        classname equals one of the passed classnames.

        :rtype: lambda
        """
        return lambda entity: entity.classname in classnames

    @staticmethod
    def equals_datamap_classname(*classnames):
        """Return a function that requires an :class:`entities.entity.Entity``
        instance. The returned function returns ``True`` if the entity's
        datamap classname equals one of the passed classnames.

        :rtype: lambda
        """
        return lambda entity: entity.datamap.class_name in classnames


class _EntityHook(AutoUnload):
    """Create entity pre and post hooks that auto unload."""

    def __init__(self, test_function, function):
        """Initialize the hook object.

        :param callable test_function:
            A callable object that accepts an :class:`entities.entity.Entity`
            instance as a parameter. The function should return ``True`` if
            the entity matches the required one.
        :param str/callable function:
            This is the function to hook. It can be either a string that
            defines the name of a function of the entity or a callable object
            that returns a :class:`memory.Function` instance.
        """
        self.test_function = test_function
        self.function = function
        self.hooked_function = None
        self.callback = None

    def __call__(self, callback):
        """Store the callback and try initializing the hook.

        :param callable callback:
            The callback to store.
        :return:
            The passed callback.
        :rtype: callable
        """
        # Validate the given callback...
        if not callable(callback):
            raise TypeError('Given callback is not callable.')

        self.callback = callback

        # Try initializing the hook...
        for entity in EntityIter():
            if self.initialize(entity):
                # Yay! The entity was the one we were looking for
                return self.callback

        # Initialization failed. There is currently no entity with the given
        # class name. So, we need to wait until such an entity has been
        # created.
        _waiting_entity_hooks.append(self)

        # Return the callback
        return self.callback

    @property
    def hook_type(self):
        """Return the hook type of the decorator.

        :rtype: HookType
        """
        raise NotImplementedError('No hook_type defined for class.')

    def initialize(self, entity):
        """Initialize the hook.

        Return ``True`` if the initialization was successful.

        :rtype: bool
        """
        if not self.test_function(entity):
            return False

        if callable(self.function):
            self.hooked_function = self.function(entity)
        else:
            self.hooked_function = getattr(entity, self.function)

        self.hooked_function.add_hook(self.hook_type, self.callback)
        return True

    def _unload_instance(self):
        """Unload the hook."""
        # Was a function hooked?
        if self.hooked_function is not None:

            # Was no callback registered?
            if self.callback is None:
                return

            # Unregister the hook...
            self.hooked_function.remove_hook(self.hook_type, self.callback)

        # Otherwise, make sure the hook is still pending before removing it...
        elif self in _waiting_entity_hooks:
            _waiting_entity_hooks.remove(self)


class EntityPreHook(_EntityHook):
    """Decorator class used to create entity pre hooks that auto unload."""

    hook_type = HookType.PRE


class EntityPostHook(_EntityHook):
    """Decorator class used to create entity post hooks that auto unload."""

    hook_type = HookType.POST


class _WaitingEntityHooks(list):
    """A dictionary to store hooks waiting for intialization."""

    def initialize(self, index):
        """Initialize all hooks waiting for the given entity.

        :param int index:
            Index of the entity that should be used to initialize the hooks.
        """
        # There is nothing to do if no hook is waiting
        if not self:
            return

        entity = Entity(index)
        for hook in tuple(self):
            # Try initializing the hook
            if hook.initialize(entity):
                # If it succeeded, remove the hook from the waiting list
                self.remove(hook)

_waiting_entity_hooks = _WaitingEntityHooks()


# =============================================================================
# >> LISTENERS
# =============================================================================
@OnNetworkedEntityCreated
def on_networked_entity_created(entity):
    """Called when a new networked entity has been created."""
    _waiting_entity_hooks.initialize(entity.index)
