# ../engines/precache.py

"""Provides classes to automatically precache models and decals."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Python Imports
#   Contextlib
from contextlib import suppress
#   Inspect
from inspect import getmodule
from inspect import stack

# Site-Package Imports
#   Path
from path import Path

# Source.Python Imports
#   Core
from core import AutoUnload
from core import global_vars
#   Engines
from engines.server import engine_server
#   Events
from events.manager import event_registry
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
           )


# =============================================================================
# >> CLASSES
# =============================================================================
class PrecacheError(Exception):

    """Object was not able to be precached due to limit being reached."""


class _PrecacheBase(Path, AutoUnload):

    """Base precache class used to interact with a specific object."""

    # Set the base _downloads attribute to know whether
    #   or not the path was added to the downloadables
    _downloads = None

    def __init__(self, path, download=False):
        """Add the file to downloadables if download is True."""
        # Call Path's __init__ with the given path
        super(_PrecacheBase, self).__init__(path)

        # Get the calling module
        caller = getmodule(stack()[1][0])

        # Set the _calling_module attribute for the instance
        self._calling_module = caller.__name__

        # Is the map loaded?
        if global_vars.map_name:
            # Precache the instance
            self._precache_method(self)

        # Register the server_spawn event to precache every map change
        event_registry.register_for_event('server_spawn', self._server_spawn)

        # Should the path be added to the downloadables?
        if download:

            # Add the path to the downloadables
            self._downloads = Downloadables()
            self._downloads.add(self)

    @property
    def index(self):
        """Return the precached index of the object."""
        # Get the index of the object in its precache table
        index = string_tables[self._precache_table][self]

        # Is the object precached?
        if index != INVALID_STRING_INDEX:

            # Return the precache index
            return index

        # If the object was not precached, raise an error
        raise PrecacheError(
            '{0} was not able to be precached due to the '.format(self) +
            '{0} table reaching its limit.'.format(self._precache_table))

    def _server_spawn(self, game_event):
        """Precache the object on map change."""
        self._precache_method(self)

    def _unload_instance(self):
        """Remove from the downloads list and unregister server_spawn."""
        # Remove the path from the downloads list
        with suppress(AttributeError):
            self._downloads._unload_instance()

        # Unregister the server_spawn event
        event_registry.unregister_for_event('server_spawn', self._server_spawn)


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
