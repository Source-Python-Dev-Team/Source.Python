# ../auth/manager.py

"""TODO"""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Python Imports
#   Re
import re
#   Importlib
from importlib.machinery import SourceFileLoader
# Site-Package Imports
#   Configobj
from configobj import ConfigObj
# Source.Python Imports
#   Auth
from auth.base import Backend
#   Engines
from engines.server import engine_server
#   Paths
from paths import BACKEND_CONFIG_FILE
from paths import BACKENDS_PATH
#   Players
from players.helpers import playerinfo_from_index
#   Steam
from steam import SteamID


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('_AuthManager',
           'auth_manager',
           'GroupPermissionDict',
           'GroupPermissions',
           'PermissionBase',
           'PlayerPermissionDict',
           'PlayerPermissions',
    )


# =============================================================================
# >> CLASSES
# =============================================================================
class PermissionBase(dict):
    """Base class for group and player permissions."""

    def __init__(self, name):
        """Initialize the object."""
        super().__init__()
        self.parents = set()
        self.name = name

    def __hash__(self):
        """Return a hash value based on the name."""
        # This is required, because we are adding dicts to sets
        return hash(self.name)

    def add(self, permission, server_id=None, update_backend=True):
        """Add a permission.

        :param str permission: The permission to add.
        :param int server_id: The server ID to which the permission should be
            added. If no server ID is given, it will be only added to this
            server.
        :param bool update_backend: If True, the backend will be updated.
        """
        if (auth_manager.targets_this_server(server_id) and
                permission not in self.keys()):
            self[permission] = self._compile_permission(permission)

        if update_backend and auth_manager.active_backend is not None:
            auth_manager.active_backend.permission_added(
                self, permission, server_id)

    def remove(self, permission, server_id=None, update_backend=True):
        """Remove a permission.

        :param str permission: The permission to remove.
        :param int server_id: The server ID from which the permission should
            be removed. If no server ID is given, it will be only removed from
            this server.
        :param bool update_backend: If True, the backend will be updated.
        """
        if auth_manager.targets_this_server(server_id):
            try:
                del self[permission]
            except KeyError:
                pass

        if update_backend and auth_manager.active_backend is not None:
            auth_manager.active_backend.permission_removed(
                self, permission, server_id)

    def add_parent(self, parent, update_backend=True):
        """Add a parent permission.

        :param str parent: Name of the permission group.
        :param bool update_backend: If True, the backend will be updated.
        """
        group = auth_manager.groups[parent]
        if group not in self.parents:
            # TODO: Detect cycles
            self.parents.add(group)
            group.children.add(self)

        if update_backend and auth_manager.active_backend is not None:
            auth_manager.active_backend.parent_added(self, parent)

    def remove_parent(self, parent, update_backend=True):
        """Remove a parent permission.

        :param str parent: Name of the permission group.
        :param bool update_backend: If True, the backend will be updated.
        """
        group = auth_manager.groups[parent]
        if group not in self.parents:
            self.parents.remove(group)
            group.children.remove(self)

        if update_backend and auth_manager.active_backend is not None:
            auth_manager.active_backend.parent_removed(self, parent)

    @staticmethod
    def _compile_permission(permission):
        """Compile a permission."""
        return re.compile(permission.replace('.', '\\.').replace('*', '(.*)'))

    def __contains__(self, permission):
        """Return True if the permission is granted by this object."""
        return self._has_permission(permission, [])

    def _has_permission(self, permission, name_list):
        # Checks to see if parents are recursive
        if self.name in name_list:
            # Break if recursive
            return False
        else:
            name_list.append(self.name)

        for re_perm in self.values():
            if re_perm.match(permission):
                return True

        for parent in self.parents:
            if parent._has_permission(permission, name_list):
                return True

        return False

    def flatten(self):
        """Return all permissions flattened recursively.

        :rtype: generator
        """
        yield from self
        for parent in self.parents:
            yield from parent

    def clear(self):
        super().clear()
        self.parents.clear()


class PlayerPermissions(PermissionBase):
    """A container for player permissions."""

    def __init__(self, name, steamid64):
        """Initialize the object.

        :param str name: A SteamID2, SteamID3 or SteamID64 value.
        :param int steamid64: The SteamID64 value that was also used to store
            the object in the :class:PlayerPermissionDict`` object.
        """
        super().__init__(name)
        self.steamid64 = steamid64


class GroupPermissions(PermissionBase):
    """A container for group permissions."""

    def __init__(self, name):
        """Initialize the object.

        :param str name: Name of the group.
        """
        super().__init__(name)
        self.children = set()


class _PermissionDict(dict):
    """A permission storage."""

    def clear(self):
        for value in self.values():
            value.clear()

        super().clear()


class GroupPermissionDict(_PermissionDict):
    def __missing__(self, group_name):
        """Create, store and return a :class:`GroupPermissions` object.

        :param str group_name: The name of the group to retrieve.
        """
        instance = self[group_name] = GroupPermissions(group_name)
        return instance


class PlayerPermissionDict(_PermissionDict):
    def __missing__(self, steamid):
        """Create, store and return a :class:`PlayerPermissions` object.

        :param str/int steamid: A SteamID2, SteamID3 or SteamID64 value.
        """
        if not isinstance(steamid, int):
            steamid64 = SteamID.parse(steamid).to_uint64()
            if steamid64 in self:
                return self[steamid64]

            # We got a SteamID in a string format, so we can store it by using
            # its SteamID64 value, but keep the original name.
            instance = self[steamid64] = PlayerPermissions(steamid, steamid64)
        else:
            instance = self[steamid] = PlayerPermissions(steamid, steamid)

        return instance


class _AuthManager(dict):
    """Manages backends and configuration files."""

    def __init__(self):
        """Initialize the object."""
        self.groups = GroupPermissionDict()
        self.players = PlayerPermissionDict()
        self.active_backend = None
        self.server_id = -1

    def _find_available_backends(self):
        """Find all available backends.

        :raise ValueError: Raised if no backend or multiple backends are found
            within a single file.
        """
        for backend in BACKENDS_PATH.glob('*.py'):
            name = 'auth.backend.' + backend.basename().splitext()[0]
            loader = SourceFileLoader(name, str(backend))
            module = loader.load_module(name)
            for var in vars(module).values():
                if isinstance(var, Backend):
                    self[var.name.casefold()] = var
                    break
            else:
                raise ValueError(
                    'Found no backend or multiple backends in "{}".'.format(
                        backend))

    def _load_config(self):
        """Load the configuration."""
        config = ConfigObj()
        config['config'] = {
            'backend': 'flatfile',
            'server_id': -1
        }

        backends_config = {}
        for backend in self.values():
            backends_config[backend.name] = backend.options

        config['backends'] = backends_config
        config.filename = BACKEND_CONFIG_FILE

        if BACKEND_CONFIG_FILE.exists():
            user_config = ConfigObj(BACKEND_CONFIG_FILE)
            config.merge(user_config)

        config.write()

        for backend in self.values():
            backend.options = config['backends'][backend.name]

        self.server_id = int(config['config']['server_id'])
        self.set_active_backend(config['config']['backend'])

    def load(self):
        """Load the auth manager."""
        self._find_available_backends()
        self._load_config()

    def unload(self):
        """Unload the auth manager."""
        self._unload_active_backend()

    def set_active_backend(self, backend_name):
        """Set the active backend.

        :param str backend_name: Name of the backend.
        :raise ValueError: Raised if the backend does not exist.
        """
        try:
            backend = self[backend_name.casefold()]
        except KeyError:
            raise ValueError(
                'Backend "{}" does not exist.'.format(backend_name))

        self._unload_active_backend()
        backend.load()
        self.active_backend = backend

    def _unload_active_backend(self):
        """Unload the active backend if there is one."""
        if self.active_backend is not None:
            self.active_backend.unload()
            self.groups.clear()
            self.players.clear()
            self.active_backend = None

    def is_backend_loaded(self, backend_name):
        """Return True if the given backend is currently loaded.

        :rtype: bool
        """
        return (self.active_backend is not None and
            backend_name.casefold() == self.active_backend.name)

    def get_player_permissions(self, index):
        """.. seealso:: :meth:`get_player_permissions_from_steamid`"""
        return self.get_player_permissions_from_steamid(
            playerinfo_from_index(index).steamid)

    def get_player_permissions_from_steamid(self, steamid):
        """Return the permissions of a player.

        :param str/int steamid: The SteamID2, SteamID3 or SteamID64 of a
            player.
        :return: If the given SteamID is invalid (e.g. 'BOT'), None will be
            returned.
        :rtype: PlayerPermissions
        """
        try:
            return self.players[steamid]
        except ValueError:
            return None

    def is_player_authorized(self, index, permission):
        """Return True if the player has been granted the given permission.

        :rtype: bool
        """
        return permission in self.get_player_permissions(index)

    def get_group_permissions(self, group_name):
        """Return the group permissions.

        :param str group_name: Name of the group.
        :rtype: GroupPermissions
        """
        return self.groups[group_name]

    def is_group_authorized(self, index, permission):
        """Return True if the group has been granted the given permission.

        :rtype: bool
        """
        return permission in self.get_group_permissions(index)

    def targets_this_server(self, server_id):
        """Return whether the server ID targets this server.

        :param int server_id: A server ID to test.
        :rtype: bool
        """
        return server_id in (-1, self.server_id, None)

#: The singleton object of :class:`_AuthManager`.
auth_manager = _AuthManager()
