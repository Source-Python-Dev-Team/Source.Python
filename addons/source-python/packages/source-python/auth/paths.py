from paths import CFG_PATH

AUTH_CFG_PATH = CFG_PATH.joinpath("auth")

if not AUTH_CFG_PATH.exists():
    AUTH_CFG_PATH.mkdir()

BACKEND_CONFIG_FILE = AUTH_CFG_PATH.joinpath("permissions.ini")
