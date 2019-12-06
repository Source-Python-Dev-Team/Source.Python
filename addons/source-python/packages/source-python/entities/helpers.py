# ../entities/helpers.py

"""Provides helper functions to convert from one type to another."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Python Imports
#   WeakRef
from weakref import proxy

# Source.Python Imports
#   Core
from core.cache import CachedProperty


# =============================================================================
# >> FORWARD IMPORTS
# =============================================================================
# Source.Python Imports
#   Entities
from _entities._helpers import baseentity_from_basehandle
from _entities._helpers import baseentity_from_edict
from _entities._helpers import baseentity_from_index
from _entities._helpers import baseentity_from_inthandle
from _entities._helpers import baseentity_from_pointer
from _entities._helpers import basehandle_from_baseentity
from _entities._helpers import basehandle_from_edict
from _entities._helpers import basehandle_from_index
from _entities._helpers import basehandle_from_inthandle
from _entities._helpers import basehandle_from_pointer
from _entities._helpers import edict_from_baseentity
from _entities._helpers import edict_from_basehandle
from _entities._helpers import edict_from_index
from _entities._helpers import edict_from_inthandle
from _entities._helpers import edict_from_pointer
from _entities._helpers import find_output_name
from _entities._helpers import index_from_baseentity
from _entities._helpers import index_from_basehandle
from _entities._helpers import index_from_edict
from _entities._helpers import index_from_inthandle
from _entities._helpers import index_from_pointer
from _entities._helpers import inthandle_from_baseentity
from _entities._helpers import inthandle_from_basehandle
from _entities._helpers import inthandle_from_edict
from _entities._helpers import inthandle_from_index
from _entities._helpers import inthandle_from_pointer
from _entities._helpers import pointer_from_baseentity
from _entities._helpers import pointer_from_basehandle
from _entities._helpers import pointer_from_edict
from _entities._helpers import pointer_from_index
from _entities._helpers import pointer_from_inthandle
#   Memory
from memory.manager import MemberFunction


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('baseentity_from_basehandle',
           'baseentity_from_edict',
           'baseentity_from_index',
           'baseentity_from_inthandle',
           'baseentity_from_pointer',
           'basehandle_from_baseentity',
           'basehandle_from_edict',
           'basehandle_from_index',
           'basehandle_from_inthandle',
           'basehandle_from_pointer',
           'edict_from_baseentity',
           'edict_from_basehandle',
           'edict_from_index',
           'edict_from_inthandle',
           'edict_from_pointer',
           'find_output_name',
           'index_from_baseentity',
           'index_from_basehandle',
           'index_from_edict',
           'index_from_inthandle',
           'index_from_pointer',
           'inthandle_from_baseentity',
           'inthandle_from_basehandle',
           'inthandle_from_edict',
           'inthandle_from_index',
           'inthandle_from_pointer',
           'pointer_from_baseentity',
           'pointer_from_basehandle',
           'pointer_from_edict',
           'pointer_from_index',
           'pointer_from_inthandle',
           'EntityMemFuncWrapper',
           'wrap_entity_mem_func',
           )


# =============================================================================
# >> CLASSES
# =============================================================================
class EntityMemFuncWrapper(MemberFunction):
    def __init__(self, wrapped_self, wrapper):
        func = wrapped_self.__getattr__(wrapper.__name__)
        super().__init__(func._manager, func._type_name, func, func._this)
        self.wrapper = wrapper
        self.wrapped_self = proxy(wrapped_self)

    def __call__(self, *args, **kwargs):
        return super().__call__(
            *self.wrapper(self.wrapped_self, *args, **kwargs))

    def call_trampoline(self, *args, **kwargs):
        return super().call_trampoline(
            *self.wrapper(self.wrapped_self, *args, **kwargs))

    def skip_hooks(self, *args, **kwargs):
        return super().skip_hooks(
            *self.wrapper(self.wrapped_self, *args, **kwargs))


# =============================================================================
# >> FUNCTIONS
# =============================================================================
def wrap_entity_mem_func(wrapper):
    """A decorator to wrap an entity memory function."""

    return CachedProperty(
        lambda self: EntityMemFuncWrapper(self, wrapper),
        doc=wrapper.__doc__
    )
