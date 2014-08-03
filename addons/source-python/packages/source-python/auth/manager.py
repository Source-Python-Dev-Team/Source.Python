# ../auth/manager.py

"""Contains a dictionary class which is used to store loaded Auth Providers."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python imports
#   Auth
from auth import AuthLogger
from auth import _auth_strings
from auth.base import AuthBase
from auth.paths import AUTH_PROVIDER_PATH
#   Players
from players.helpers import uniqueid_from_playerinfo


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('AuthManager',
           )


# =============================================================================
# >> GLOBAL VARIABLES
# =============================================================================
# Get the sp.auth.manager logger
AuthManagerLogger = AuthLogger.manager


# =============================================================================
# >> CLASSES
# =============================================================================
class _AuthManager(dict):

    """Stores loaded auth providers."""

    def load_auth(self, provider):
        """Load the given provider."""
        # Send a message that the auth provider is being loaded
        AuthManagerLogger.log_message(
            '[SP Auth] ' + _auth_strings[
                'Loading'].get_string(provider=provider))

        # Is the provider loaded?
        if provider in self:

            # If so, send a message that the provider is already loaded
            AuthManagerLogger.log_message(
                '[SP Auth] ' + _auth_strings[
                    'Already Loaded'].get_string(provider=provider))

            # No need to go further
            return

        # Does the provider's file exist?
        if not AUTH_PROVIDER_PATH.joinpath(provider + '.py').isfile():

            # Send a message that the file does not exist
            AuthManagerLogger.log_message(
                '[SP Auth] ' + _auth_strings[
                    'No Module'].get_string(provider=provider))

            # No need to go further
            return

        # Import the provider's module
        module = __import__(
            'auth.providers.{0}'.format(provider), fromlist=[''])

        # Loop through all objects in the module
        for module_object in dir(module):

            # Get the object's instance
            instance = getattr(module, module_object)

            # Is the current object a AuthBase instance?
            if isinstance(instance, AuthBase):

                # Found the instance
                break

        # Was no AuthBase instance found?
        else:

            # Raise an error that the object was not found
            raise NotImplementedError(
                'No AuthBase instance found in provider'
                ' "{0}"'.format(provider))

        # Attempt to call the provider's load function
        instance.load()

        # Add the provider to the dictionary
        self[provider] = instance

        # Send a message that the provider was loaded
        AuthManagerLogger.log_message(
            '[SP Auth] ' + _auth_strings[
                'Load Successful'].get_string(provider=provider))

    def unload_auth(self, provider):
        """Unload the given provider."""
        # Send a message that the auth provider is being unloaded
        AuthManagerLogger.log_message(
            '[SP Auth] ' + _auth_strings[
                'Unloading'].get_string(provider=provider))

        # Is the provider loaded?
        if provider not in self:

            # If not, send a message that the provider is not loaded
            AuthManagerLogger.log_message(
                '[SP Auth] ' + _auth_strings[
                    'Not Loaded'].get_string(provider=provider))

            # No need to go further
            return

        # Call the providers unload method
        self[provider].unload()

        # Remove the provider
        del self[provider]

        # Send a message that the provider was unloaded
        AuthManagerLogger.log_message(
            '[SP Auth] ' + _auth_strings[
                'Unload Successful'].get_string(provider=provider))

    def reload_auth(self, provider):
        """Reload the given provider."""
        # Unload the provider
        self.unload_auth(provider)

        # Load the provider
        self.load_auth(provider)

    def is_player_authorized(
            self, playerinfo, level=None, permission=None, flag=None):
        """Check to see if the player is authorized."""
        # Get the player's uniqueid
        uniqueid = uniqueid_from_playerinfo(playerinfo)

        # Loop through all loaded auth providers
        for provider in self:

            # Is the player authorized?
            if self[provider].is_player_authorized(
                    uniqueid, level, permission, flag):

                # If the player is authorized, return true
                return True

        # If the player is not found authorized for any provider, return false
        return False

# Get the _AuthManager instance
AuthManager = _AuthManager()
