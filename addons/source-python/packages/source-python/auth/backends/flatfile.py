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
from auth.base import PermissionSource
from auth.manager import auth_manager
#   Paths
from paths import AUTH_CFG_PATH


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('FlatfilePermissionSource',
           'source',
    )


# =============================================================================
# >> CLASSES
# =============================================================================
class FlatfilePermissionSource(PermissionSource):
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
        self.load_config(
            auth_manager.players, self.options['admin_config_path'])
        self.load_config(
            auth_manager.groups, self.options['group_config_path'])
        self.load_simple_config(
            auth_manager.players, self.options['simple_config_path'])

    @staticmethod
    def load_config(store, path):
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

source = FlatfilePermissionSource()
