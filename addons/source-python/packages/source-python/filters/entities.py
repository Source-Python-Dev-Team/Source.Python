# ../filters/entities.py

"""Provides entity filtering functionality."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
#   Entities
from entities import BaseEntityGenerator
from entities import EntityGenerator
from entities.entity import Entity
from entities.helpers import index_from_edict
#   Filters
from filters.iterator import _IterObject


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('BaseEntityIter',
           'EntityIter',
           )


# =============================================================================
# >> ENTITY ITERATION CLASSES
# =============================================================================
class BaseEntityIter(_IterObject):
    """BaseEntity iterate class."""

    def __init__(self, class_names=None, exact_match=True):
        """Store the base attributes for the generator."""
        # Was only one class name given?
        if isinstance(class_names, str):

            # Convert the class name to a list
            class_names = [class_names]

        # Store the base attributes given
        self.class_names = list() if class_names is None else class_names
        self.exact_match = exact_match

    @staticmethod
    def iterator():
        """Iterate over all :class:`entities.entity.BaseEntity` objects."""
        return BaseEntityGenerator()

    def _is_valid(self, entity):
        """Verify that the edict needs yielded."""
        # Are there any class names to be checked?
        if not self.class_names:

            # Return True for all entities
            return True

        # Loop through all class names for the generator
        for check_name in self.class_names:

            # Does the current class name match part of the edict's class name?
            if not self.exact_match and check_name in entity.classname:
                return True

            # Does the current class name match exactly the edict's class name?
            elif self.exact_match and check_name == entity.classname:
                return True

        # If none of the class names returned True, return False
        return False


class EntityIter(BaseEntityIter):
    """Entity iterate class."""

    @staticmethod
    def iterator():
        """Iterate over all :class:`entities.entity.Entity` objects."""
        for edict in EntityGenerator():
            yield Entity(index_from_edict(edict))
