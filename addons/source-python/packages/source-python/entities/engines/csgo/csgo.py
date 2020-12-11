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
#   Weapons
from weapons.manager import weapon_manager


# =============================================================================
# >> GLOBAL VARIABLES
# =============================================================================
_weapon_names_for_definition = {
    weapon_manager[weapon].name: values.get('item_definition_index')
    for weapon, values in weapon_manager.ini.get('weapons', {}).items()
    if values.get('item_definition_index')
}
_weapon_parents = {
    weapon_manager[weapon].name: values.get('parent_class')
    for weapon, values in weapon_manager.ini.get('weapons', {}).items()
    if values.get('parent_class')
}
_parent_weapons = set(_weapon_parents.values())


# =============================================================================
# >> CLASSES
# =============================================================================
class Entity(_Entity):
    """Class used to interact directly with entities."""

    # Instances of this class will be cached by default
    caching = True

    @classmethod
    def create(cls, classname):
        index = _weapon_names_for_definition.get(classname)
        if index is not None:
            parent_class = _weapon_parents.get(classname)
            entity = super().create(parent_class or classname)
            entity.item_definition_index = index
        else:
            entity = super().create(classname)
        return entity

    @classmethod
    def find(cls, classname):
        index = _weapon_names_for_definition.get(classname)
        if classname in _weapon_parents and index is not None:
            parent_classname = _weapon_parents[classname]
            for entity in BaseEntityGenerator(parent_classname, True):
                if not entity.is_networked():
                    continue
                if entity.get_network_property_int(
                    'm_AttributeManager.m_Item.m_iItemDefinitionIndex'
                ) == index:
                    return cls(entity.index)
        elif classname in _parent_weapons:
            for entity in BaseEntityGenerator(classname, True):
                if not entity.is_networked():
                    continue
                if entity.get_network_property_int(
                    'm_AttributeManager.m_Item.m_iItemDefinitionIndex'
                ) in (index, 0):
                    return cls(entity.index)
        return super().find(classname)

    thrower = property(
        lambda self: self.__getattr__('thrower'),
        lambda self, inthandle: self.set_attacker(pointer_from_inthandle(inthandle))
    )
