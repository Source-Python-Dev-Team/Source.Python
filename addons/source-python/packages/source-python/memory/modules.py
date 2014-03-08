# ../memory/modules.py

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
from memory_c import find_binary


# =============================================================================
# >> CLASSES
# =============================================================================
class _ModuleData(dict):
    '''Dictionary used to store binaries by module'''

    def __missing__(self, item):
        '''Add the module to the dictionary'''

        # Add the missing module to the dictionary
        value = self[item] = find_binary(item)

        # Return the module's instance
        return value

# Get the _ModuleData instance
ModuleData = _ModuleData()
