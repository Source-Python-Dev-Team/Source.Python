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
           'is_newer_version_available',
           'is_unversioned',
           )


# =============================================================================
# >> CONSTANTS
# =============================================================================
VERSION = 'unversioned'

# TODO: Update this url when the new subdomain has been created
LAST_SUCCESSFUL_BUILD_NUMBER_URL = ('http://build.affecta.net/job/Source.Pyth'
    'on/api/xml?xpath=/freeStyleProject/lastSuccessfulBuild/number')


# =============================================================================
# >> GLOBAL VARIABLES
# =============================================================================
_sp_version = PublicConVar(
    'sp_version', VERSION, description='Source.Python version')


# =============================================================================
# >> FUNCTIONS
# =============================================================================
def get_last_successful_build_number(timeout=3):
    """Return the latest successful build number as an integer."""
    with urlopen(LAST_SUCCESSFUL_BUILD_NUMBER_URL, timeout=timeout) as url:
        # Remove the <number></number> tags, so we just have the build number
        return int(url.read()[8:-9])

def is_unversioned():
    """Return True if the current version is set to 'unversioned'."""
    return VERSION == 'unversioned'

def is_newer_version_available():
    """Return a tuple that contains a boolean that indicates if a new
    Source.Python version is available and an integer that defines the latest
    successful build number.

    If the current version is set to 'unversioned', it will return True for
    the first value.
    """
    build_number = get_last_successful_build_number()
    return (is_unversioned() or VERSION < build_number, build_number)
