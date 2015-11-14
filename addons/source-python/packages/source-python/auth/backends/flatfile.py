import json

from path import Path

from paths import AUTH_CFG_PATH
from auth.base import PermissionSource
from auth.manager import auth_manager


class FlatfilePermissionSource(PermissionSource):
    name = "flatfile"
    options = {
        "admin_config_path": AUTH_CFG_PATH.joinpath("admins.json"),
        "group_config_path": AUTH_CFG_PATH.joinpath("groups.json"),
        "simple_config_path": AUTH_CFG_PATH.joinpath("simple.txt")
    }

    def load(self):
        self.load_config(auth_manager.players, self.options["admin_config_path"])
        self.load_config(auth_manager.groups, self.options["group_config_path"])
        self.load_simple_config(auth_manager.players, self.options["simple_config_path"])

    @staticmethod
    def load_config(store, path):
        path = Path(path)
        if not path.exists():
            with open(path, "w") as file:
                json.dump({}, file)
        with open(path) as file:
            try:
                nodes = json.load(file)
                for nodename, node in nodes.items():
                    nodename = nodename.strip()
                    for permission in node.get("permissions", set()):
                        if permission != "":
                            store[nodename].add(permission)
                    for group in node.get("parents", set()):
                        store[nodename].add_parent(group)
            except ValueError:
                print("Malformed permissions file: {}".format(path))

    @staticmethod
    def load_simple_config(store, path):
        path = Path(path)
        if not path.exists():
            open(path, "w").close()
        with open(path) as file:
            for uniqueid in file.readlines():
                store[uniqueid.strip()].add("*")

source = FlatfilePermissionSource()
