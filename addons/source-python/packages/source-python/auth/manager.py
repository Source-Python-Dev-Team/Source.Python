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
from auth.base import PermissionSource
#   Paths
from paths import BACKEND_CONFIG_FILE
from paths import BACKENDS_PATH
#   Players
from players.helpers import uniqueid_from_index


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('_AuthManager',
           'auth_manager'
           'GroupPermissions',
           'PermissionBase',
           'PermissionDict',
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

    def add(self, permission):
        """Add a permission.

        :param str permission: The permission to add.
        """
        if permission in self:
            return

        self[permission] = self._compile_permission(permission)
        if auth_manager.active_backend is not None:
            auth_manager.active_backend.permission_added(self, permission)

    def remove(self, permission):
        """Remove a permission.

        :param str permission: The permission to remove.
        """
        try:
            del self[permission]
        except KeyError:
            pass
        else:
            if auth_manager.active_backend is not None:
                auth_manager.active_backend.permission_removed(
                    self, permission)

    def add_parent(self, parent):
        """Add a parent permission.

        :param str parent: Name of the permission group.
        """
        group = auth_manager.groups[parent]
        if group in self.parents:
            return

        self.parents.add(group)
        group.children.add(self)
        if auth_manager.active_backend is not None:
            auth_manager.active_backend.parent_added(self, parent)

    def remove_parent(self, parent):
        """Remove a parent permission.

        :param str parent: Name of the permission group.
        """
        group = auth_manager.groups[parent]
        if group not in self.parents:
            return

        self.parents.remove(group)
        group.children.remove(self)
        if auth_manager.active_backend is not None:
            auth_manager.active_backend.parent_removed(self, parent)

    @staticmethod
    def _compile_permission(permission):
        """Compile a permission."""
        return re.compile(permission.replace('.', '\\.').replace('*', '(.*)'))

    def __contains__(self, permission, name_list=None):
        """Return True if the permission is granted by this object."""
        if name_list is None:
            name_list = []
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
            if permission.__contains__(parent, name_list):
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

    def __new__(cls, uniqueid):
        """Return a PlayerPermissions object.

        :param str uniqueid: The unique ID of a player.
        """
        if uniqueid in auth_manager.players:
            return auth_manager.players[uniqueid]

        self = auth_manager.players[uniqueid] = super().__new__(cls)
        return self


class GroupPermissions(PermissionBase):
    """A container for group permissions."""

    def __new__(cls, name):
        """Return a GroupPermissions object.

        :param str name: Name of the group.
        """
        if name in auth_manager.groups:
            return auth_manager.groups[name]

        self = auth_manager.groups[name] = super().__new__(cls)
        return self

    def __init__(self, name):
        """Initialize the object.

        :param str name: Name of the group.
        """
        super().__init__(name)
        self.children = set()


class PermissionDict(dict):
    """A permission storage."""

    def __init__(self, permission_type):
        """Initialize the object.

        :param PermissionBase permission_type: The permission class whose
            objects should be stored.
        """
        super().__init__()
        self.permission_type = permission_type

    def __missing__(self, key):
        instance = self[key] = self.permission_type(key)
        return instance
        
    def clear(self):
        for value in self.values():
            value.clear()
            
        super().clear()


class _AuthManager(dict):
    """Manages backends and configuration files."""

    def __init__(self):
        """Initialize the object."""
        self.groups = PermissionDict(GroupPermissions)
        self.players = PermissionDict(PlayerPermissions)
        self.active_backend = None

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
                if isinstance(var, PermissionSource):
                    self[var.name.casefold()] = var
                    break
            else:
                raise ValueError(
                    'Found no backend or multiple backends in "{}".'.format(
                        backend))

    def _load_config(self):
        """Load the configuration."""
        config = ConfigObj()
        config['Config'] = {
            'PermissionBackend': 'flatfile'
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

        self.set_active_backend(config['Config']['PermissionBackend'])

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
        """Return the permissions of a player.

        :param int index: Index of the player.
        :rtype: PlayerPermissions
        """
        return self.players[uniqueid_from_index(index)]

    def is_authorized(self, index, permission):
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

#: The singleton object of :class:`_AuthManager`.
auth_manager = _AuthManager()
