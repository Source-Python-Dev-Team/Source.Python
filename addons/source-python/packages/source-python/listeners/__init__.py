# ../listeners/__init__.py

"""Provides listener based functionality."""

# TODO: Fix name conflict with _ListenerManager

# =============================================================================
# >> IMPORTS
# =============================================================================
# Python Imports
import socket
from urllib.error import URLError
# Source.Python Imports
#   Cvars
from cvars import ConVar
#   Core
from core import AutoUnload
from core.settings import _core_settings
from core.version import is_newer_version_available
from core.version import is_unversioned
from core.version import VERSION
#   Loggers
from loggers import _sp_logger


# =============================================================================
# >> FORWARD IMPORTS
# =============================================================================
# Source.Python Imports
#   Listeners
from _listeners import _ListenerManager
from _listeners import client_active_listener_manager
from _listeners import client_connect_listener_manager
from _listeners import client_disconnect_listener_manager
from _listeners import client_fully_connect_listener_manager
from _listeners import client_put_in_server_listener_manager
from _listeners import client_settings_changed_listener_manager
from _listeners import level_init_listener_manager
from _listeners import level_shutdown_listener_manager
from _listeners import network_id_validated_listener_manager
from _listeners import on_edict_allocated_listener_manager
from _listeners import on_edict_freed_listener_manager
from _listeners import on_entity_pre_spawned_listener_manager
from _listeners import on_entity_created_listener_manager
from _listeners import on_entity_spawned_listener_manager
from _listeners import on_entity_deleted_listener_manager
from _listeners import on_data_loaded_listener_manager
from _listeners import on_combiner_pre_cache_listener_manager
from _listeners import on_data_unloaded_listener_manager
from _listeners import on_query_cvar_value_finished_listener_manager
from _listeners import server_activate_listener_manager
from _listeners import tick_listener_manager
#   Entity output
from listeners._entity_output import entity_output_listener_manager


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('ClientActive',
           'ClientConnect',
           'ClientDisconnect',
           'ClientFullyConnect',
           'ClientPutInServer',
           'ClientSettingsChanged',
           'EntityOutput',
           'LevelInit',
           'LevelShutdown',
           'NetworkidValidated',
           'OnCombinerPreCache',
           'OnDataLoaded',
           'OnDataUnloaded',
           'OnEdictAllocated',
           'OnEdictFreed',
           'OnEntityCreated',
           'OnEntityDeleted',
           'OnEntityPreSpawned',
           'OnEntitySpawned',
           'OnQueryCvarValueFinished',
           'ServerActivate',
           'Tick',
           'VersionUpdate',
           'client_active_listener_manager',
           'client_connect_listener_manager',
           'client_disconnect_listener_manager',
           'client_fully_connect_listener_manager',
           'client_put_in_server_listener_manager',
           'client_settings_changed_listener_manager',
           'entity_output_listener_manager',
           'level_init_listener_manager',
           'level_shutdown_listener_manager',
           'network_id_validated_listener_manager',
           'on_combiner_pre_cache_listener_manager',
           'on_data_loaded_listener_manager',
           'on_data_unloaded_listener_manager',
           'on_edict_allocated_listener_manager',
           'on_edict_freed_listener_manager',
           'on_entity_created_listener_manager',
           'on_entity_deleted_listener_manager',
           'on_entity_pre_spawned_listener_manager',
           'on_entity_spawned_listener_manager',
           'on_query_cvar_value_finished_listener_manager',
           'server_activate_listener_manager',
           'tick_listener_manager',
           'version_update_listener_manager',
           )


# =============================================================================
# >> GLOBAL VARIABLES
# =============================================================================
# Get the sp.listeners logger
listeners_logger = _sp_logger.listeners
version_update_listener_manager = _ListenerManager()

_check_for_update = ConVar('sp_check_for_update',
    _core_settings['VERSION_SETTINGS']['check_for_update'],
    description='Enable/disable checking for version updates.',
    min_value=0, max_value=1)

_notify_on_update = ConVar('sp_notify_on_update',
    _core_settings['VERSION_SETTINGS']['notify_on_update'],
    description=('Log a warning when a Source.Python update is available. Req'
        'uires sp_check_for_update to be set to 1.'),
    min_value=0, max_value=1)


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
                "'" + type(callback).__name__ + "' object is not callable.")

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

    @property
    def manager(self):
        """Return a _ListenerManager object."""
        raise NotImplementedError('Must be implemented by a subclass.')

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

    manager = client_active_listener_manager


class ClientConnect(_ListenerManager):

    """Register/unregister a ClientConnect listener."""

    manager = client_connect_listener_manager


class ClientDisconnect(_ListenerManager):

    """Register/unregister a ClientDisconnect listener."""

    manager = client_disconnect_listener_manager


class ClientFullyConnect(_ListenerManager):

    """Register/unregister a ClientFullyConnect listener."""

    manager = client_fully_connect_listener_manager


class ClientPutInServer(_ListenerManager):

    """Register/unregister a ClientPutInServer listener."""

    manager = client_put_in_server_listener_manager


class ClientSettingsChanged(_ListenerManager):

    """Register/unregister a ClientSettingsChanged listener."""

    manager = client_settings_changed_listener_manager


class EntityOutput(_ListenerManager):

    """Register/unregister an EntityOutput listener."""

    manager = entity_output_listener_manager


class LevelInit(_ListenerManager):

    """Register/unregister a LevelInit listener."""

    manager = level_init_listener_manager


class LevelShutdown(_ListenerManager):

    """Register/unregister a LevelShutdown listener."""

    manager = level_shutdown_listener_manager


class NetworkidValidated(_ListenerManager):

    """Register/unregister a NetworkidValidated listener."""

    manager = network_id_validated_listener_manager


class OnEdictAllocated(_ListenerManager):

    """Register/unregister an OnEdictAllocated listener."""

    manager = on_edict_allocated_listener_manager


class OnEdictFreed(_ListenerManager):

    """Register/unregister an OnEdictFreed listener."""

    manager = on_edict_freed_listener_manager


class OnEntityPreSpawned(_ListenerManager):

    """Register/unregister a OnEntityPreSpawned listener."""

    manager = on_entity_pre_spawned_listener_manager


class OnEntityCreated(_ListenerManager):

    """Register/unregister a OnEntityCreated listener."""

    manager = on_entity_created_listener_manager


class OnEntitySpawned(_ListenerManager):

    """Register/unregister a OnEntitySpawned listener."""

    manager = on_entity_spawned_listener_manager


class OnEntityDeleted(_ListenerManager):

    """Register/unregister a OnEntityDeleted listener."""

    manager = on_entity_deleted_listener_manager


class OnDataLoaded(_ListenerManager):

    """Register/unregister a OnDataLoaded listener."""

    manager = on_data_loaded_listener_manager


class OnCombinerPreCache(_ListenerManager):

    """Register/unregister a OnCombinerPreCache listener."""

    manager = on_combiner_pre_cache_listener_manager


class OnDataUnloaded(_ListenerManager):

    """Register/unregister a OnDataUnloaded listener."""

    manager = on_data_unloaded_listener_manager


class OnQueryCvarValueFinished(_ListenerManager):

    """Register/unregister an OnQueryCvarValueFinished listener."""

    manager = on_query_cvar_value_finished_listener_manager


class ServerActivate(_ListenerManager):

    """Register/unregister a ServerActivate listener."""

    manager = server_activate_listener_manager


class Tick(_ListenerManager):

    """Register/unregister a Tick listener."""

    manager = tick_listener_manager


class VersionUpdate(_ListenerManager):

    """Register/unregister a version update listener."""

    manager = version_update_listener_manager


# =============================================================================
# >> CALLBACKS
# =============================================================================
@LevelInit
def _on_level_init(map_name):
    """Called when a new map gets initialized."""
    if not _check_for_update.get_int():
        return

    try:
        update_available, version = is_newer_version_available()
    except (URLError, socket.timeout):
        return

    if not update_available:
        return

    if _notify_on_update.get_int():
        # TODO: Add translations
        listeners_logger.log_warning(
            'A new Source.Python version is available!')

    version_update_listener_manager.notify(VERSION, version, is_unversioned())
