# ../entities/helpers.py

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
from entity_c import BaseEntityHandle
from entity_c import Edict
from entity_c import index_of_pointer
from public import public


# =============================================================================
# >> FUNCTIONS
# =============================================================================
@public
def index_from_edict(edict):
    '''Returns an index from the given edict'''
    return edict.get_index()


@public
def index_from_basehandle(bhandle):
    '''Returns an index from the given BaseHandle instance'''
    return bhandle.get_entry_index()


@public
def index_from_inthandle(ihandle):
    '''Returns an index from the given handle in int form'''
    return index_from_basehandle(basehandle_from_inthandle(ihandle))


@public
def index_from_pointer(pointer):
    '''Returns an index from the given BaseEntity pointer'''
    return index_of_pointer(pointer)


@public
def edict_from_index(index):
    '''Returns an edict from the given index'''
    return Edict(index)


@public
def edict_from_basehandle(bhandle):
    '''Returns an edict from the given BaseHandle instance'''
    return edict_from_index(index_from_basehandle(bhandle))


@public
def edict_from_inthandle(ihandle):
    '''Returns an edict from the given handle in int form'''
    return edict_from_index(index_from_inthandle(ihandle))


@public
def edict_from_pointer(pointer):
    '''Returns an edict from the given BaseEntity pointer'''
    return edict_from_index(index_of_pointer(pointer))


@public
def basehandle_from_index(index):
    '''Returns a BaseHandle instance from the given index'''
    return basehandle_from_edict(edict_from_index(index))


@public
def basehandle_from_edict(edict):
    '''Returns a BaseHandle instance from the given edict'''
    return edict.get_networkable().get_entity_handle().get_ref_ehandle()


@public
def basehandle_from_inthandle(ihandle):
    '''Returns a BaseHandle instance from the given handle in int form'''
    return BaseEntityHandle(ihandle)


@public
def basehandle_from_pointer(pointer):
    '''Returns a BaseHandle instance from the given BaseEntity pointer'''
    return basehandle_from_index(index_of_pointer(pointer))


@public
def inthandle_from_index(index):
    '''Returns a handle in int form from the given index'''
    return inthandle_from_basehandle(basehandle_from_index(index))


@public
def inthandle_from_edict(edict):
    '''Returns a handle in int form from the given edict'''
    return inthandle_from_basehandle(basehandle_from_edict(edict))


@public
def inthandle_from_basehandle(bhandle):
    '''Returns a handle in int form from the given BaseHandle instance'''
    return bhandle.to_int()


@public
def inthandle_from_pointer(pointer):
    '''Returns a handle in int form from the given BaseEntity pointer'''
    return inthandle_from_index(index_of_pointer(pointer))


@public
def pointer_from_index(index):
    '''Returns a BaseEntity pointer from the given index'''
    return pointer_from_edict(edict_from_index(index))


@public
def pointer_from_edict(edict):
    '''Returns a BaseEntity pointer from the given edict'''
    return edict.get_unknown().get_base_entity()


@public
def pointer_from_basehandle(bhandle):
    '''Returns a BaseEntity pointer from the given BaseHandle instance'''
    return pointer_from_edict(edict_from_basehandle(bhandle))


@public
def pointer_from_inthandle(ihandle):
    '''Returns a BaseEntity pointer from the given handle in int form'''
    return pointer_from_edict(edict_from_inthandle(ihandle))
