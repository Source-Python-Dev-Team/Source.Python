# ../stringtables/__init__.py

"""Provides access to server stringtables."""

# =============================================================================
# >> FORWARD IMPORTS
# =============================================================================
# Source.Python Imports
#   StringTables
from _stringtables import StringTable
from _stringtables import StringTables
from _stringtables import INVALID_STRING_TABLE
from _stringtables import INVALID_STRING_INDEX


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('StringTable',
           'StringTables',
           'INVALID_STRING_TABLE',
           'INVALID_STRING_INDEX',
           )
