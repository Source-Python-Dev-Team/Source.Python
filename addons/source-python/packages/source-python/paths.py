# ../paths.py

# =============================================================================
# >> IMPORTS
# =============================================================================
# Site Package Imports
#   Path
from path import Path


# =============================================================================
# >> GLOBAL VARIABLES
# =============================================================================
# Get the game's base path
GAME_PATH = Path(Path(__file__).rsplit('addons', 1)[0])

# Get the base path to ../addons/source-python
ADDON_PATH = GAME_PATH.joinpath('addons', 'source-python')

# Get the plugin's base path
PLUGIN_PATH = ADDON_PATH.joinpath('plugins')

# Get the cfg's base path
CFG_PATH = GAME_PATH.joinpath('cfg', 'source-python')

# Get the log's base path
LOG_PATH = GAME_PATH.joinpath('logs', 'source-python')

# Get the translation's base path
TRANSLATION_PATH = GAME_PATH.joinpath(
    'resource', 'source-python', 'translations')

# Get the custom event's base path
EVENT_PATH = GAME_PATH.joinpath('resource', 'source-python', 'events')

# Get the sound's base path
SOUND_PATH = GAME_PATH.joinpath('sound', 'source-python')

# Get the base path to the data
_DATA_PATH = ADDON_PATH.joinpath('data')

# Get the SP base data path
SP_DATA_PATH = _DATA_PATH.joinpath('source-python')

# Get the Custom data base path
CUSTOM_DATA_PATH = _DATA_PATH.joinpath('custom')

# Get the plugin specific data path
PLUGIN_DATA_PATH = _DATA_PATH.joinpath('plugins')

# Add all paths to __all__
__all__ = [x for x in globals() if x.isupper() and not x.startswith('_')]
