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
        self[permission] = self._compile_permission(permission)

    def remove(self, permission):
        """Remove a permission.

        :param str permission: The permission to remove.
        """
        del self[permission]

    @staticmethod
    def _compile_permission(permission):
        """Compile a permission."""
        return re.compile(permission.replace('.', '\\.').replace('*', '(.*)'))

    def __contains__(self, permission):
        """Return True if the permission is granted by this object."""
        for re_perm in self.values():
            if re_perm.match(permission):
                return True

        for parent in self.parents:
            if permission in parent:
                return True

        return False

    def flatten(self):
        """Return all permissions flattened recursively.

        :rtype: generator
        """
        yield from self
        for parent in self.parents:
            yield from parent

    def add_parent(self, parent):
        """Add a parent permission.

        :param str parent: Name of the permission group.
        """
        group = auth_manager.groups[parent]
        self.parents.add(group)
        group.children.add(self)

    def remove_parent(self, parent):
        """Remove a parent permission.

        :param str parent: Name of the permission group.
        """
        group = auth_manager.groups[parent]
        self.parents.remove(group)
        group.children.remove(self)


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


class _AuthManager(object):
    """Manages backends and configuration files."""

    def __init__(self):
        """Initialize the object."""
        self.groups = PermissionDict(GroupPermissions)
        self.players = PermissionDict(PlayerPermissions)
        self.available_backends = []
        self.active_backend = None

    def _find_available_backends(self):
        """Find all available backends."""
        for backend in BACKENDS_PATH.glob('*.py'):
            name = 'auth.backend.' + backend.basename().splitext()[0]
            loader = SourceFileLoader(name, str(backend))
            module = loader.load_module(name)
            for var in vars(module).values():
                if isinstance(var, PermissionSource):
                    self.available_backends.append(var)
                    break

    def _load_config(self):
        """Load the configuration."""
        config = ConfigObj()
        config['Config'] = {
            'PermissionBackend': 'flatfile'
        }

        backends_config = {}
        for backend in self.available_backends:
            backends_config[backend.name] = backend.options

        config['backends'] = backends_config
        config.filename = BACKEND_CONFIG_FILE

        if BACKEND_CONFIG_FILE.exists():
            user_config = ConfigObj(BACKEND_CONFIG_FILE)
            config.merge(user_config)

        config.write()

        for backend in self.available_backends:
            backend.options = config['backends'][backend.name]

        self.load_backend(config['Config']['PermissionBackend'])

    def load(self):
        """Load the auth manager."""
        self._find_available_backends()
        self._load_config()

    def load_backend(self, backend_name):
        """Load a backend.

        :param str backend_name: Name of the backend.
        """
        for backend in self.available_backends:
            if backend.name.casefold() != backend_name.casefold():
                continue

            self.active_backend = backend
            self.groups.clear()
            self.players.clear()
            backend.load()
            return True

        return False

    def get_player_permissions(self, index):
        """Return the permissions of a player.

        :param int index: Index of the player.
        :rtype: PlayerPermissions
        """
        return self.players[uniqueid_from_index(index)]

    def get_group_permissions(self, group_name):
        """Return the group permissions.

        :param str group_name: Name of the group.
        :rtype: GroupPermissions
        """
        return self.groups[group_name]

#: The singleton object of :class:`_AuthManager`.
auth_manager = _AuthManager()
