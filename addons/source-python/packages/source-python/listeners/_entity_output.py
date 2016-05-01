# ../listeners/_entity_output.py

"""Provides a listener manager for entity outputs."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
#   Core
from core import GameConfigObj
from core import PLATFORM
#   Paths
from paths import SP_DATA_PATH
#   Memory
import memory

from memory.hooks import PreHook
from memory.manager import manager
#   Entity
from entities.entity import BaseEntity
from entities.entity import Entity
from entities.datamaps import Variant
from entities.helpers import find_output_name
#   Listeners
from _listeners import ListenerManager


# =============================================================================
# >> GLOBAL VARIABLES
# =============================================================================
BaseEntityOutput = manager.create_type_from_dict(
    'BaseEntityOutput', GameConfigObj(
        SP_DATA_PATH / 'entity_output' / 'CBaseEntityOutput.ini'))

on_entity_output_listener_manager = ListenerManager()


# =============================================================================
# >> CALLBACKS
# =============================================================================
@PreHook(BaseEntityOutput.fire_output)
def _pre_fire_output(args):
    """Called when an output is about to be fired."""
    if not on_entity_output_listener_manager:
        return

    # Windows is a bit weird: the function takes 4 additional arguments...
    if PLATFORM == 'windows':
        args = (args[0],) + tuple(args)[5:]

    caller_ptr = args[3]
    if not caller_ptr:
        # If we don't know the caller, we won't be able to retrieve the output
        # name
        return

    caller = memory.make_object(BaseEntity, caller_ptr)
    output_name = find_output_name(caller, args[0])
    if output_name is None:
        return None

    if caller.is_networked():
        caller = memory.make_object(Entity, caller_ptr)

    value_ptr = args[1]
    value = (value_ptr or None) and memory.make_object(Variant, value_ptr)

    activator_ptr = args[2]
    activator = ((activator_ptr or None) and memory.make_object(
        BaseEntity, activator_ptr))
    if activator is not None and activator.is_networked():
        activator = memory.make_object(Entity, activator_ptr)

    delay = args[4]
    on_entity_output_listener_manager.notify(
        output_name, activator, caller, value, delay)
