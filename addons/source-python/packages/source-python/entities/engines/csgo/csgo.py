# ../entities/engines/csgo/csgo.py

"""Provides CS:GO game specific Entity based functionality."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python
#   Entities
from . import Entity as _Entity
from entities import BaseEntityGenerator
from entities.helpers import pointer_from_inthandle
#   Filters
from filters.weapons import WeaponClassIter


# =============================================================================
# >> GLOBAL VARIABLES
# =============================================================================
_cant_create_weapons = {
    weapon_class.name: weapon_class
    for weapon_class in WeaponClassIter('cant_create')
}
_cant_find_weapons = {
    weapon_class.name: weapon_class
    for weapon_class in WeaponClassIter('cant_find')
}
_parent_weapons = {
    weapon_class.name: weapon_class.item_definition_index
    for weapon_class in WeaponClassIter('parent')
}


# =============================================================================
# >> CLASSES
# =============================================================================
class Entity(_Entity):
    """Class used to interact directly with entities."""

    # Instances of this class will be cached by default
    caching = True

    @classmethod
    def create(cls, classname):
        # Specific weapons cannot be created directly, create a parent class
        # for the weapon and set the item_definition_index.
        weapon_class = _cant_create_weapons.get(classname, None)
        if weapon_class is not None:
            entity = super().create(weapon_class.parent_class)
            entity.item_definition_index = weapon_class.item_definition_index
        else:
            entity = super().create(classname)
        return entity

    @classmethod
    def find(cls, classname):
        # Specific weapons cannot be found directly, instead iterate over
        # the parent class and detect if the item_definition_index matches.
        weapon_class = _cant_find_weapons.get(classname, None)
        if weapon_class is not None:
            parent_classname = weapon_class.parent_class
            item_definition_index = weapon_class.item_definition_index
            for entity in BaseEntityGenerator(parent_classname, True):
                if not entity.is_networked():
                    continue
                if entity.get_network_property_int(
                    'm_AttributeManager.m_Item.m_iItemDefinitionIndex'
                ) == item_definition_index:
                    return cls(entity.index)

        # Parent weapon class is equivalent to the child class, therefore
        # iterates over the parent class to detect if
        # the item_definition_index matches.
        parent_item_definition_index = _parent_weapons.get(classname, None)
        if parent_item_definition_index is not None:
            for entity in BaseEntityGenerator(classname, True):
                if not entity.is_networked():
                    continue
                if entity.get_network_property_int(
                    'm_AttributeManager.m_Item.m_iItemDefinitionIndex'
                ) in (parent_item_definition_index, 0):
                    return cls(entity.index)

        return super().find(classname)

    thrower = property(
        lambda self: self.__getattr__('thrower'),
        lambda self, inthandle: self.set_attacker(pointer_from_inthandle(inthandle))
    )
