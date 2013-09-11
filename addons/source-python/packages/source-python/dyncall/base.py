# ../dyncall/base.py

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
from Source import Binutils


# =============================================================================
# >> GLOBAL VARIABLES
# =============================================================================
# Store the virtual machine instance
DynCallVM = Binutils.dcGetVM()


# Store the calling conventions
DynCallModes = {
    'cdecl': Binutils.DC_CALL_C_DEFAULT,
    'nt': Binutils.DC_CALL_C_X86_WIN32_THIS_MS,
    'linux': Binutils.DC_CALL_C_X86_WIN32_THIS_GNU,
}


# Store the argument methods by code
DynCallArgs = {
    'b': Binutils.dcArgBool,
    'c': Binutils.dcArgString,
    's': Binutils.dcArgShort,
    'i': Binutils.dcArgInt,
    'l': Binutils.dcArgLong,
    'f': Binutils.dcArgFloat,
    'd': Binutils.dcArgDouble,
    #'t': Binutils.dcArgStruct,
    'p': Binutils.dcArgPointer,
}


# Store the call methods by code
DynCallCalls = {
    'v': Binutils.dcCallVoid,
    'b': Binutils.dcCallBool,
    'c': Binutils.dcCallChar,
    's': Binutils.dcCallShort,
    'i': Binutils.dcCallInt,
    'l': Binutils.dcCallLong,
    'f': Binutils.dcCallFloat,
    'd': Binutils.dcCallDouble,
    #'t': Binutils.dcCallStruct,
    'p': Binutils.dcCallPointer,
}
