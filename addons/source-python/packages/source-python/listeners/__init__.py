# ../listeners/__init__.py

"""Provides listener based functionality."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
#   Core
from core import AutoUnload
#   Loggers
from loggers import _sp_logger


# =============================================================================
# >> FORWARD IMPORTS
# =============================================================================
# Source.Python Imports
#   Listeners
from _listeners import ClientActiveListenerManager
from _listeners import ClientConnectListenerManager
from _listeners import ClientDisconnectListenerManager
from _listeners import ClientFullyConnectListenerManager
from _listeners import ClientPutInServerListenerManager
from _listeners import ClientSettingsChangedListenerManager
from _listeners import LevelInitListenerManager
from _listeners import LevelShutdownListenerManager
from _listeners import NetworkidValidatedListenerManager
from _listeners import OnEdictAllocatedListenerManager
from _listeners import OnEdictFreedListenerManager
from _listeners import OnQueryCvarValueFinishedListenerManager
from _listeners import ServerActivateListenerManager
from _listeners import TickListenerManager


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('ClientActive',
           'ClientActiveListenerManager',
           'ClientConnect',
           'ClientConnectListenerManager',
           'ClientDisconnect',
           'ClientDisconnectListenerManager',
           'ClientFullyConnect',
           'ClientFullyConnectListenerManager',
           'ClientPutInServer',
           'ClientPutInServerListenerManager',
           'ClientSettingsChanged',
           'ClientSettingsChangedListenerManager',
           'LevelInit',
           'LevelInitListenerManager',
           'LevelShutdown',
           'LevelShutdownListenerManager',
           'NetworkidValidated',
           'NetworkidValidatedListenerManager',
           'OnEdictAllocated',
           'OnEdictAllocatedListenerManager',
           'OnEdictFreed',
           'OnEdictFreedListenerManager',
           'OnQueryCvarValueFinished',
           'OnQueryCvarValueFinishedListenerManager',
           'ServerActivate',
           'ServerActivateListenerManager',
           'Tick',
           'TickListenerManager',
           )


# =============================================================================
# >> GLOBAL VARIABLES
# =============================================================================
# Get the sp.listeners logger
listeners_logger = _sp_logger.listeners


# =============================================================================
# >> CLASSES
# =============================================================================
class _ListenerManager(AutoUnload):

    """Base decorator class used to register/unregister a listener."""

    def __init__(self, callback):
        """Store the callback and register the listener."""
        # Log the <instance>.__init__ message
        listeners_logger.log_info(
            '{0}.__init__<{1}>'.format(self.name, callback))

        # Is the callback callable?
        if not callable(callback):

            # Raise an error
            raise TypeError(
                "'" + type(callback).__name__ + "'object is not callable")

        # Log the registering message
        listeners_logger.log_info(
            '{0}.__init__ - Registering'.format(self.name))

        # Store the callback
        self.callback = callback

        # Register the listener
        self.manager.register_listener(self.callback)

    def __call__(self, *args):
        """Call the listener."""
        # Log the calling
        listeners_logger.log_info(
            '{0}.__call__<{1}>'.format(self.name, args))

        # Call the listener
        return self.callback(*args)

    @property
    def name(self):
        """Return the class name of the instance."""
        return self.__class__.__name__

    def _unload_instance(self):
        """Unregister the listener."""
        # Log the unregistering
        listeners_logger.log_info(
            '{0}._unload_instance - Unregistering <{1}>'.format(
                self.name, self.callback))

        # Unregister the listener
        self.manager.unregister_listener(self.callback)


class ClientActive(_ListenerManager):

    """Register/unregister a ClientActive listener."""

    manager = ClientActiveListenerManager


class ClientConnect(_ListenerManager):

    """Register/unregister a ClientConnect listener."""

    manager = ClientConnectListenerManager


class ClientDisconnect(_ListenerManager):

    """Register/unregister a ClientDisconnect listener."""

    manager = ClientDisconnectListenerManager


class ClientFullyConnect(_ListenerManager):

    """Register/unregister a ClientFullyConnect listener."""

    manager = ClientFullyConnectListenerManager


class ClientPutInServer(_ListenerManager):

    """Register/unregister a ClientPutInServer listener."""

    manager = ClientPutInServerListenerManager


class ClientSettingsChanged(_ListenerManager):

    """Register/unregister a ClientSettingsChanged listener."""

    manager = ClientSettingsChangedListenerManager


class LevelInit(_ListenerManager):

    """Register/unregister a LevelInit listener."""

    manager = LevelInitListenerManager


class LevelShutdown(_ListenerManager):

    """Register/unregister a LevelShutdown listener."""

    manager = LevelShutdownListenerManager


class NetworkidValidated(_ListenerManager):

    """Register/unregister a NetworkidValidated listener."""

    manager = NetworkidValidatedListenerManager


class OnEdictAllocated(_ListenerManager):

    """Register/unregister an OnEdictAllocated listener."""

    manager = OnEdictAllocatedListenerManager


class OnEdictFreed(_ListenerManager):

    """Register/unregister an OnEdictFreed listener."""

    manager = OnEdictFreedListenerManager


class OnQueryCvarValueFinished(_ListenerManager):

    """Register/unregister an OnQueryCvarValueFinished listener."""

    manager = OnQueryCvarValueFinishedListenerManager


class ServerActivate(_ListenerManager):

    """Register/unregister a ServerActivate listener."""

    manager = ServerActivateListenerManager


class Tick(_ListenerManager):

    """Register/unregister a Tick listener."""

    manager = TickListenerManager
