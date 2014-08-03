# ../auth/paths.py

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
#   Paths
from paths import SP_PACKAGES_PATH
from paths import CFG_PATH


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('AUTH_CFG_PATH',
           'AUTH_PROVIDER_PATH',
           )


# =============================================================================
# >> GLOBAL VARIABLES
# =============================================================================
# Store the path to the auth providers
AUTH_PROVIDER_PATH = SP_PACKAGES_PATH.joinpath('auth', 'providers')

# Store the path to the auth configurations
AUTH_CFG_PATH = CFG_PATH.joinpath('auth_providers')
