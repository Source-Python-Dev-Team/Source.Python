# ../cvars.py

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
from _cvars import *


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
# Add all the global variables to __all__
__all__ = [
    'Cvar',
    'ConCommandBase',
    'ConVar',
    'FCVAR_NONE',
    'FCVAR_UNREGISTERED',
    'FCVAR_DEVELOPMENTONLY',
    'FCVAR_GAMEDLL',
    'FCVAR_CLIENTDLL',
    'FCVAR_PROTECTED',
    'FCVAR_ARCHIVE',
    'FCVAR_NOTIFY',
    'FCVAR_USERINFO',
    'FCVAR_PRINTABLEONLY',
    'FCVAR_UNLOGGED',
    'FCVAR_NEVER_AS_STRING',
    'FCVAR_REPLICATED',
    'FCVAR_CHEAT',
    'FCVAR_DEMO',
    'FCVAR_DONTRECORD',
    'FCVAR_NOT_CONNECTED',
    'FCVAR_ARCHIVE_XBOX',
    'FCVAR_SERVER_CAN_EXECUTE',
    'FCVAR_SERVER_CANNOT_QUERY',
    'FCVAR_CLIENTCMD_CAN_EXECUTE'
    'ConVarFlags',
    'ServerVar'
]


# =============================================================================
# >> CLASSES
# =============================================================================
class ConVarFlags:
    NONE = FCVAR_NONE
    UNREGISTERED = FCVAR_UNREGISTERED
    DEVELOPMENTONLY = FCVAR_DEVELOPMENTONLY
    GAMEDLL = FCVAR_GAMEDLL
    CLIENTDLL = FCVAR_CLIENTDLL
    PROTECTED = FCVAR_PROTECTED
    ARCHIVE = FCVAR_ARCHIVE
    NOTIFY = FCVAR_NOTIFY
    USERINFO = FCVAR_USERINFO
    PRINTABLEONLY = FCVAR_PRINTABLEONLY
    UNLOGGED = FCVAR_UNLOGGED
    NEVER_AS_STRING = FCVAR_NEVER_AS_STRING
    REPLICATED = FCVAR_REPLICATED
    CHEAT = FCVAR_CHEAT
    DEMO = FCVAR_DEMO
    DONTRECORD = FCVAR_DONTRECORD
    NOT_CONNECTED = FCVAR_NOT_CONNECTED
    ARCHIVE_XBOX = FCVAR_ARCHIVE_XBOX
    SERVER_CAN_EXECUTE = FCVAR_SERVER_CAN_EXECUTE
    SERVER_CANNOT_QUERY = FCVAR_SERVER_CANNOT_QUERY
    CLIENTCMD_CAN_EXECUTE = FCVAR_CLIENTCMD_CAN_EXECUTE


class ServerVar(ConVar):
    '''ConVar wrapper to provide easy access to cvars.'''

    def __init__(
            self, name, value='0', flags=0,
            description='', min_value=None, max_value=None):

        '''Called when a server-var is initilized.'''
        super(ServerVar, self).__init__(
            name, value, flags, description,
            not min_value is None, min_value or 0.0,
            not max_value is None, max_value or 0.0)

    def __getattr__(self, attr):
        '''Gets the value of the given attribute'''

        # Get the flag
        flag = getattr(ConVarFlags, attr.upper(), None)

        # Is the attribute a flag?
        if not flag is None:

            # Return the value of the cvar's flag
            return self.is_flag_set(flag)

        # If not, raise an error
        raise AttributeError(
            '"ServerVar" object has no attribute "{0}"'.format(attr))

    def __setattr__(self, attr, value):
        '''Sets the value of the given attribute'''

        # Get the flag
        flag = getattr(ConVarFlags, attr.upper(), None)

        # Is the attribute a flag?
        if flag is None:

            # Does the instance itself have the attribute?
            if not hasattr(self, attr):

                # If not, raise an error
                raise AttributeError(
                    '"ServerVar" object has no attribute "{0}"'.format(attr))

            # Set the attribute
            super(ServerVar, self).__setattr__(attr, value)

            # No need to go further
            return

        # Is the value "True"
        if value:

            # Add the flag
            self.add_flags(flag)

            # No need to go further
            return

        # Remove the flag
        self.remove_flags(flag)

    def make_public(self):
        '''Sets the notify flag for the cvar.'''
        self.add_flags(ConVarFlags.NOTIFY)
