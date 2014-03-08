# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python
from memory_c import *


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = [
    'callback',

    # From memory_c
    'Argument',
    'BinaryFile',
    'Callback',
    'Convention',
    'DC_ERROR_NONE',
    'DC_ERROR_UNSUPPORTED_MODE',
    'Function',
    'NULL',
    'Pointer',
    'Return',
    'StackData',
    'TYPE_SIZES',
    'alloc',
    'find_binary',
    'get_error',
    'get_pointer',
    'get_size',
    'make_object'
]


# =============================================================================
# >> FUNCTIONS
# =============================================================================
def callback(convention=Convention.CDECL, args=(), return_type=Return.VOID):
    '''
    Use this decorator to create a C++ callback that calls back to the
    decorated function.

    EXAMPLE:

    @callback(Convention.CDECL, (Argument.INT, Argument.INT), Return.INT)
    def add(x, y, ebp):
        return x + y

    <add> is now a Callback instance, but you can still call it like a normal
    Python function:

    assert add(4, 6) == 10
    '''

    def wait_for_func(func):
        return Callback(func, convention, args, return_type)

    return wait_for_func