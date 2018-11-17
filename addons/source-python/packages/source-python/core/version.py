# ../core/version.py

"""Provides the Source.Python version."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Python Imports
from urllib.request import urlopen
# Source.Python Imports
#   Cvars
from cvars.public import PublicConVar


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('VERSION',
           'GIT_COMMIT',
           'get_last_successful_build_number',
           'is_unversioned',
           )


# =============================================================================
# >> CONSTANTS
# =============================================================================
#: Version of the Source.Python intallation.
VERSION = None

#: The Github commit this Source.Python version was built with.
GIT_COMMIT = None

LAST_SUCCESSFUL_BUILD_NUMBER_URL = (
    'http://downloads.sourcepython.com/version.txt')


# =============================================================================
# >> GLOBAL VARIABLES
# =============================================================================
_sp_version = PublicConVar('sp_version', str(VERSION), 'Source.Python version')


# =============================================================================
# >> FUNCTIONS
# =============================================================================
def get_last_successful_build_number(timeout=3):
    """Return the latest successful build number as an integer.

    :param float timeout:
        Number of seconds that need to pass until a timeout occurs.
    :rtype: int
    """
    with urlopen(LAST_SUCCESSFUL_BUILD_NUMBER_URL, timeout=timeout) as url:
        return int(url.read().decode('utf-8'))

def is_unversioned():
    """Return ``True`` if this Source.Python installation has no version.

    This can happen in multiple cases:

    * This is an unofficial build (not a build from the build server).
    * The Python files have been updated directly from the repository.

    :rtype: bool
    """
    return VERSION is None
