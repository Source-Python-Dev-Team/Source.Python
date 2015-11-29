# ../engines/precache.py

"""Provides classes to automatically precache models and decals."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Python Imports
#   Contextlib
from contextlib import suppress

# Source.Python Imports
#   Core
from core import AutoUnload
#   Engines
from engines.server import engine_server
from engines.server import global_vars
#   Events
from events.manager import event_manager
#   Stringtables
from stringtables import INVALID_STRING_INDEX
from stringtables import string_tables
from stringtables.downloads import Downloadables


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('Decal',
           'Generic',
           'Model',
           '_PrecacheBase',
           )


# =============================================================================
# >> CLASSES
# =============================================================================
class PrecacheError(Exception):
    """Object was not able to be precached due to limit being reached."""


class _PrecacheBase(AutoUnload):
    """Base precache class used to interact with a specific object."""

    # Set the base _downloads attribute to know whether
    #   or not the path was added to the downloadables
    _downloads = None

    def __init__(self, path, preload=False, download=False):
        """Add the file to downloadables if download is True."""
        # Save the path that should be precached
        self._path = path

        # Save whether the the file should be preloaded
        self._preload = preload

        # Is the map loaded?
        if global_vars.map_name:

            # Precache the instance
            self._precache()

        # Register the server_spawn event to precache every map change
        event_manager.register_for_event('server_spawn', self._server_spawn)

        # Should the path be added to the downloadables?
        if download:

            # Add the path to the downloadables
            self._downloads = Downloadables()
            self._downloads.add(self._path)

    @property
    def index(self):
        """Return the precached index of the object."""
        # Get the index of the object in its precache table
        index = string_tables[self._precache_table][self._path]

        # Is the object precached?
        if index != INVALID_STRING_INDEX:

            # Return the precache index
            return index

        # If the object was not precached, raise an error
        raise PrecacheError(
            '"{0}" was not able to be precached due to the "{1}" table '
            'reaching its limit.'.format(self._path, self._precache_table))

    @property
    def path(self):
        """Return the path."""
        return self._path

    def _precache(self):
        """Precache the path."""
        self._precache_method(self._path, self._preload)

    def _server_spawn(self, game_event):
        """Precache the object on map change."""
        self._precache()

    def _unload_instance(self):
        """Remove from the downloads list and unregister server_spawn."""
        # Remove the path from the downloads list
        with suppress(AttributeError):
            self._downloads._unload_instance()

        # Unregister the server_spawn event
        event_manager.unregister_for_event('server_spawn', self._server_spawn)

    @property
    def _precache_table(self):
        """Should define the name of the precache table."""
        raise NotImplementedError('No _precache_table defined for class.')

    @property
    def _precache_method(self):
        """Should define the method to be used to precache the path."""
        raise NotImplementedError('No _precache_method defined for class.')


class Decal(_PrecacheBase):
    """Class used to handle a specific decal."""

    # Set the base attributes
    _precache_table = 'decalprecache'
    _precache_method = engine_server.precache_decal


class Generic(_PrecacheBase):
    """Class used to handle generic precaching."""

    # Set the base attributes
    _precache_table = 'genericprecache'
    _precache_method = engine_server.precache_generic


class Model(_PrecacheBase):
    """Class used to handle a specific model."""

    # Set the base attributes
    _precache_table = 'modelprecache'
    _precache_method = engine_server.precache_model
