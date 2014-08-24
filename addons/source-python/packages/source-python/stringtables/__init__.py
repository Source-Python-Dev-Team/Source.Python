# ../stringtables/__init__.py

"""Provides access to server stringtables."""

# =============================================================================
# >> FORWARD IMPORTS
# =============================================================================
# Source.Python Imports
#   StringTables
from _stringtables import StringTable
from _stringtables import string_tables
from _stringtables import INVALID_STRING_TABLE
from _stringtables import INVALID_STRING_INDEX


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('INVALID_STRING_INDEX',
           'INVALID_STRING_TABLE',
           'StringTable',
           'string_tables',
           )
