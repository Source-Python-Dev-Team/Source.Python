# ../entities/transmit.py

"""Provides entity transmission filtering."""

# ============================================================================
# >> IMPORTS
# ============================================================================
# Source.Python
#   Core
from core import AutoUnload
#   Entities
from entities.entity import Entity
from entities.hooks import EntityCondition
#   Filters
from filters.entities import EntityIter
#   Listeners
from listeners import on_entity_created_listener_manager
from listeners import on_entity_deleted_listener_manager


# ============================================================================
# >> FORWARD IMPORTS
# ============================================================================
# Source.Python
#   Entities
from _entities._transmit import BaseTransmitCriteria
from _entities._transmit import BaseTransmitFilter
from _entities._transmit import TransmitManager
from _entities._transmit import TransmitStates
from _entities._transmit import TransmitTarget
from _entities._transmit import TransmitType
from _entities._transmit import transmit_manager


# ============================================================================
# >> ALL DECLARATION
# ============================================================================
__all__ = [
    'BaseTransmitCriteria',
    'BaseTransmitFilter',
    'TransmitCriteria',
    'TransmitFilter',
    'TransmitManager',
    'TransmitStates',
    'TransmitTarget',
    'TransmitType',
    'transmit_manager',
]


# ============================================================================
# >> CLASSES
# ============================================================================
class TransmitFilter(AutoUnload, BaseTransmitFilter):
    """Decorator class used to register entity transmission filter."""

    def __call__(self, callback):
        """Register the given callback and initialize the filter.

        :param function callback:
            The callback to register.
        """
        self.callback = callback
        self.initialize()
        return self

    def initialize(self):
        """Initializes the filter."""
        transmit_manager.register_filter(self)

    def _unload_instance(self):
        """Unregister ourself as filter."""
        transmit_manager.unregister_filter(self)


class TransmitCriteria(AutoUnload, BaseTransmitCriteria):
    """Class used to narrow down entity transmission filtering."""

    def __init__(self, conditions, target=TransmitTarget.ENTITY):
        """Initialize the criteria.

        :param function/iterable conditions:
            Conditions that an entity have to meet in order to match this
            criteria.
        :param TransmitTarget target:
            Whether the criteria is to be checked against the entity being
            transmitted or the receiving player.
        """
        super().__init__(target)

        # Validate and store the given conditions
        if callable(conditions):
            self.conditions = (conditions,)
        else:
            for condition in conditions:
                if not callable(condition):
                    raise ValueError(f'"{condition}" is not callable.')
            self.conditions = conditions

        # Let's test existing entities
        for entity in EntityIter():
            self[entity.index] = self.test_entity(entity)

        # Register our internal listeners
        on_entity_created_listener_manager.register_listener(
            self._on_entity_created)
        on_entity_deleted_listener_manager.register_listener(
            self._on_entity_deleted)

    def test_entity(self, entity):
        """Test whether the entity matches all the conditions.

        :param Entity entity::
            The entity to test against all conditions.

        :rtype:
            bool
        """
        for condition in self.conditions:
            if not condition(entity):
                return False

        return True

    def _on_entity_created(self, base_entity):
        """Called when an entity is created.

        :param BaseEntity base_entity:
            The entity that was created.
        """
        # Try to grab an index or exit if the entity isn't networked
        try:
            entity = Entity(base_entity.index)
        except ValueError:
            return

        # Test the entity and set the result
        self[entity.index] = self.test_entity(entity)

    def _on_entity_deleted(self, base_entity):
        """Called when an entity is being deleted.

        :param BaseEntity base_entity:
            The entity that is being deleted.
        """
        # Try to grab an index or exit if the entity isn't networked
        try:
            entity_index = base_entity.index
        except ValueError:
            return

        # Mark the entity as no longer matching
        del self[entity_index]

    def _unload_instance(self):
        """Unregister our internal listeners."""
        self.listener.unregister_listener(self._on_entity_created)
        on_entity_deleted_listener_manager.unregister_listener(
            self._on_entity_deleted)


# For convenience, register some commonly used criterias
TransmitCriteria.is_player = TransmitCriteria(
    EntityCondition.is_player)
TransmitCriteria.is_not_player = TransmitCriteria(
    EntityCondition.is_not_player)
TransmitCriteria.is_human_player = TransmitCriteria(
    EntityCondition.is_human_player)
TransmitCriteria.is_bot_player = TransmitCriteria(
    EntityCondition.is_bot_player)
TransmitCriteria.equals_entity_classname = (
    lambda *classnames: TransmitCriteria(
        EntityCondition.equals_entity_classname(*classnames)))
TransmitCriteria.equals_entity_classname = (
    lambda *classnames: TransmitCriteria(
        EntityCondition.equals_entity_classname(*classnames)))
