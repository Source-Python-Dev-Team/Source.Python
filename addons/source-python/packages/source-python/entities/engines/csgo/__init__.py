# ../engines/engines/csgo/__init__.py

"""Provides CS:GO specific Entity based functionality."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
from entities._base import Entity as _Entity
from weapons.manager import weapon_manager


# =============================================================================
# >> GLOBAL VARIABLES
# =============================================================================
_weapon_names_for_definition = {
    weapon_manager[weapon].name: values.get('item_definition_index')
    for weapon, values in weapon_manager.ini['weapons'].items()
    if values.get('item_definition_index')
}
_weapon_parents = {
    weapon_manager[weapon].name: values.get('parent_class')
    for weapon, values in weapon_manager.ini['weapons'].items()
    if values.get('parent_class')
}
_parent_weapons = set(_weapon_parents.values())


# =============================================================================
# >> CLASSES
# =============================================================================
class Entity(_Entity):
    """Class used to interact directly with entities."""

    @classmethod
    def create(cls, classname):
        index = _weapon_names_for_definition.get(classname)
        if classname in _weapon_parents and index is not None:
            entity = super().create(_weapon_parents[classname])
            entity.item_definition_index = index
        else:
            entity = super().create(classname)
        return entity

    @classmethod
    def find(cls, classname):
        from filters.entities import EntityIter
        index = _weapon_names_for_definition.get(classname)
        if classname in _weapon_parents and index is not None:
            for entity in EntityIter(_weapon_parents[classname]):
                if entity.item_definition_index == index:
                    return entity
        elif classname in _parent_weapons:
            for entity in EntityIter(classname):
                if entity.item_definition_index in (index, 0):
                    return entity
        return super().find(classname)
