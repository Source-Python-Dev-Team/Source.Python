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
           'get_last_successful_build_number',
           'is_unversioned',
           )


# =============================================================================
# >> CONSTANTS
# =============================================================================
VERSION = 'unversioned'

LAST_SUCCESSFUL_BUILD_NUMBER_URL = (
    'http://builds.sourcepython.com/job/Source.Python' +
    '/api/xml?xpath=/freeStyleProject/lastSuccessfulBuild/number')


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
        # Remove the <number></number> tags, so we just have the build number
        return int(url.read()[8:-9])


def is_unversioned():
    """Return True if the current version is set to 'unversioned'.

    :rtype: bool
    """
    return VERSION == 'unversioned'
