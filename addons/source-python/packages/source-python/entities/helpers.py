# ../entities/helpers.py

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
from conversions_c import *


# =============================================================================
# >> PLAYER OBJECT REMOVAL
# =============================================================================
# Loop through all functions imported
for function in dict(globals()):

    # Is the current function a "player" based function?
    if 'playerinfo' in function or 'userid' in function:

        # Remove the function
        del globals()[function]

# Remove the function variable
del function


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = [x for x in globals() if not x.startswith('__')]
