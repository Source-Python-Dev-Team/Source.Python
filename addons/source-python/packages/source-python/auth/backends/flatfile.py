# ../auth/backends/flatfile.py

"""Provides the flat file backend."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Python Imports
#   Json
import json
# Site-Package Imports
#   Path
from path import Path
# Source.Python Imports
#   Auth
from auth.base import Backend
from auth.manager import auth_manager
from auth.manager import PlayerPermissions
from auth.manager import GroupPermissions
#   Paths
from paths import AUTH_CFG_PATH


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('FlatfileBackend',
           'source',
    )


# =============================================================================
# >> CLASSES
# =============================================================================
class FlatfileBackend(Backend):
    """A backend that provides an admin and group file in JSON format and a
    simple text file.
    """

    name = 'flatfile'
    options = {
        'admin_config_path': AUTH_CFG_PATH / 'admins.json',
        'group_config_path': AUTH_CFG_PATH / 'groups.json',
        'simple_config_path': AUTH_CFG_PATH / 'simple.txt'
    }

    def load(self):
        """Load the backend."""
        self.load_json_config(
            auth_manager.players, self.options['admin_config_path'])
        self.load_json_config(
            auth_manager.groups, self.options['group_config_path'])
        self.load_simple_config(
            auth_manager.players, self.options['simple_config_path'])

    def save_admin_config(self):
        """Save the admin configuration file."""
        self._save_json_config(
            auth_manager.players, self.options['admin_config_path'])

    def save_simple_config(self):
        """Save the simple configuration file."""
        self._save_simple_config(
            auth_manager.players, self.options['simple_config_path'])

    def save_group_config(self):
        """Save the group configuration file."""
        self._save_json_config(
            auth_manager.groups, self.options['group_config_path'])

    @staticmethod
    def _save_json_config(store, path):
        """Save a store to the given path as JSON output."""
        path = Path(path)
        with path.open('w') as f:
            temp_dict = {}
            for name, permissions in store.items():
                node = temp_dict[name] = {}

                parents = permissions.parents
                if parents:
                    node['parents'] = list(
                        map(lambda parent: parent.name, parents))

                if permissions:
                    node['permissions'] = list(permissions)

            json.dump(temp_dict, f, indent=4, sort_keys=True)

    @staticmethod
    def _save_simple_config(store, path):
        """Save a store to the given path as simple output.

        Since this file is the 'simple' file, only items with full admin
        rights are written to the file.
        """
        path = Path(path)
        with path.open('w') as f:
            for name, permissions in sorted(store.items()):
                for permission in permissions:
                    if permission == '*':
                        f.write(name + '\n')
                        break

    @staticmethod
    def load_json_config(store, path):
        """Load a config from a file into the given store."""
        path = Path(path)

        # Make sure the file exists
        if not path.exists():
            with path.open('w') as file:
                json.dump({}, file)

        with path.open() as file:
            for node_name, node in json.load(file).items():
                node_store = store[node_name.strip()]
                for permission in node.get('permissions', set()):
                    if permission != '':
                        node_store.add(permission)

                for group_name in node.get('parents', set()):
                    node_store.add_parent(group_name)

    @staticmethod
    def load_simple_config(store, path):
        """Load a simple config file into the given store."""
        path = Path(path)

        # Make sure the file exists
        path.open('a').close()

        with path.open() as file:
            for uniqueid in file.readlines():
                store[uniqueid.strip()].add('*')

    def permission_added(self, node, permission, server_id):
        self._node_permission_changed(node, permission, server_id)

    def permission_removed(self, node, permission, server_id):
        self._node_permission_changed(node, permission, server_id)

    def parent_added(self, node, parent_name):
        self._node_parent_changed(node, parent_name)

    def parent_removed(self, node, parent_name):
        self._node_parent_changed(node, parent_name)

    def _node_permission_changed(self, node, permission, server_id):
        if not auth_manager.targets_this_server(server_id):
            return

        if isinstance(node, PlayerPermissions):
            self.save_admin_config()
            if permission == '*':
                self.save_simple_config()
        elif isinstance(node, GroupPermissions):
            self.save_group_config()
        else:
            raise TypeError(
                'Unexpected type "{}".'.format(type(node).__name__))

    def _node_parent_changed(self, node, parent_name):
        if isinstance(node, PlayerPermissions):
            self.save_admin_config()
        elif isinstance(node, GroupPermissions):
            self.save_group_config()
        else:
            raise TypeError(
                'Unexpected type "{}".'.format(type(node).__name__))

source = FlatfileBackend()
