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
from _memory import HookType
#   Filters
from filters.entities import EntityIter
#   Listeners
from listeners import OnEntityCreated
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
        """Return True if the entity is a player."""
        return entity.is_player()

    @staticmethod
    def is_not_player(entity):
        """Return True if the entity is not a player."""
        return not entity.is_player()

    @staticmethod
    def is_human_player(entity):
        """Return True if the entity is a human player."""
        return entity.is_player() and Player(entity.index).steamid != 'BOT'

    @staticmethod
    def is_bot_player(entity):
        """Return True if the entity is a bot."""
        return entity.is_player() and Player(entity.index).steamid == 'BOT'

    @staticmethod
    def equals_entity_classname(*classnames):
        """Return a function that requires an Entity object.

        The function returns True if the entity's
        classname equals one of the passed classnames.
        """
        return lambda entity: entity.classname in classnames

    @staticmethod
    def equals_datamap_classname(*classnames):
        """Return a function that requires an Entity object.

        The function returns True if the entity's
        datamap classname equals one of the passed classnames.
        """
        return lambda entity: entity.datamap.class_name in classnames


class _EntityHook(AutoUnload):
    """Create entity pre and post hooks that auto unload."""

    def __init__(self, test_function, function_name):
        """Initialize the hook object.

        @param <test_function>:
        A callable object that accepts an Entity object as a parameter. The
        function should return True if the entity matches the required one.

        @<function_name>:
        The name of the function to hook. The function must be available
        through the Entity class.
        """
        self.test_function = test_function
        self.function_name = function_name
        self.function = None
        self.callback = None

    def __call__(self, callback):
        """Store the callback and try initializing the hook."""
        self.callback = callback

        # Try initializing the hook...
        for entity in EntityIter():
            if self.initialize(entity):
                # Yay! The entity was the one we were looking for
                return self

        # Initialization failed. There is currently no entity with the given
        # class name. So, we need to wait until such an entity has been
        # created.
        _waiting_entity_hooks.append(self)

        # Return the callback
        return self.callback

    @property
    def hook_type(self):
        """Raise an error if the inheriting class does not have their own."""
        raise NotImplementedError('No hook_type defined for class.')

    def initialize(self, entity):
        """Initialize the hook.

        Return True if the initialization was successful.
        """
        if not self.test_function(entity):
            return False

        self.function = getattr(entity, self.function_name)
        self.function.add_hook(self.hook_type, self.callback)
        return True

    def _unload_instance(self):
        """Unload the hook."""
        if self.function is not None:
            self.function.remove_hook(self.hook_type, self.callback)
        else:
            # If the function is None, the hook wasn't initialized, so it's
            # still in the _waiting_entity_hooks dict
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
        """Initialize all hooks waiting for the given entity."""
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
@OnEntityCreated
def on_entity_created(index, ptr):
    """Called when a new entity has been created."""
    if index is not None:
        _waiting_entity_hooks.initialize(index)
