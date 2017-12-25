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
        """Add the file to downloadables if download is True.

        :param str path:
            Path to file to use.
        :param bool preload:
            If ``True`` the file will be pre-loaded.
        :param bool download:
            If ``True`` the file will be added to the ``downloadables`` string
            table.
        """
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
        """Return the precached index of the object.

        :rtype: int
        """
        # Get the index of the object in its precache table
        index = string_tables[self.precache_table][self._path]

        # Is the object precached?
        if index != INVALID_STRING_INDEX:

            # Return the precache index
            return index

        # If the object was not precached, raise an error
        raise PrecacheError(
            '"{0}" was not able to be precached due to the "{1}" table '
            'reaching its limit.'.format(self._path, self.precache_table))

    @property
    def path(self):
        """Return the path.

        :rtype: str
        """
        return self._path

    def _precache(self):
        """Precache the file."""
        raise NotImplementedError('Must be implemented by a subclass.')

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
    def precache_table(self):
        """Return the name of the precache table.

        :rtype: str
        """
        raise NotImplementedError('No precache_table defined for class.')


class Decal(_PrecacheBase):
    """Class used to handle a specific decal."""

    # Set the base attributes
    precache_table = 'decalprecache'

    def _precache(self):
        return engine_server.precache_decal(self._path, self._preload)


class Generic(_PrecacheBase):
    """Class used to handle generic precaching."""

    # Set the base attributes
    precache_table = 'genericprecache'

    def _precache(self):
        return engine_server.precache_generic(self._path, self._preload)


class Model(_PrecacheBase):
    """Class used to handle a specific model."""

    # Set the base attributes
    precache_table = 'modelprecache'

    def _precache(self):
        return engine_server.precache_model(self._path, self._preload)
