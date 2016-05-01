# ../paths.py

"""Stores directory paths for commonly used directories."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Site-Package Imports
#   Path
from path import Path


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('BASE_PATH',
           'CFG_PATH',
           'CUSTOM_DATA_PATH',
           'CUSTOM_PACKAGES_DOCS_PATH',
           'CUSTOM_PACKAGES_PATH',
           'DOCS_PATH',
           'EVENT_PATH',
           'GAME_PATH',
           'LOG_PATH',
           'PACKAGES_PATH',
           'PLUGIN_DATA_PATH',
           'PLUGIN_DOCS_PATH',
           'PLUGIN_PATH',
           'SITE_PACKAGES_PATH',
           'SOUND_PATH',
           'SP_DATA_PATH',
           'SP_DOCS_PATH',
           'SP_PACKAGES_PATH',
           'TRANSLATION_PATH',
           )


# =============================================================================
# >> GLOBAL VARIABLES
# =============================================================================
# ../<game>
GAME_PATH = Path(Path(__file__).rsplit('addons', 1)[0])

# ../addons/source-python
BASE_PATH = GAME_PATH / 'addons' / 'source-python'

# ../addons/source-python/docs
DOCS_PATH = BASE_PATH / 'docs'

# ../addons/source-python/docs/source-python
SP_DOCS_PATH = DOCS_PATH / 'source-python'

# ../addons/source-python/docs/plugins
PLUGIN_DOCS_PATH = DOCS_PATH / 'plugins'

# ../addons/source-python/docs/custom
CUSTOM_PACKAGES_DOCS_PATH = DOCS_PATH / 'custom'

# ../addons/source-python/plugins
PLUGIN_PATH = BASE_PATH / 'plugins'

# ../addons/source-python/packages
PACKAGES_PATH = BASE_PATH / 'packages'

# ../addons/source-python/packages/source-python
SP_PACKAGES_PATH = PACKAGES_PATH / 'source-python'

# ../addons/source-python/packages/custom
CUSTOM_PACKAGES_PATH = PACKAGES_PATH / 'custom'

# ../addons/source-python/packages/site-packages
SITE_PACKAGES_PATH = PACKAGES_PATH / 'site-packages'

# ../cfg/source-python
CFG_PATH = GAME_PATH / 'cfg' / 'source-python'

# ../logs/source-python
LOG_PATH = GAME_PATH / 'logs' / 'source-python'

# ../resource/source-python/translations
TRANSLATION_PATH = GAME_PATH / 'resource' / 'source-python' / 'translations'

# ../resource/source-python/events
EVENT_PATH = GAME_PATH / 'resource' / 'source-python' / 'events'

# ../sound/source-python
SOUND_PATH = GAME_PATH / 'sound' / 'source-python'

# ../addons/source-python/data
DATA_PATH = BASE_PATH / 'data'

# ../addons/source-python/data/source-python
SP_DATA_PATH = DATA_PATH / 'source-python'

# ../addons/source-python/data/custom
CUSTOM_DATA_PATH = DATA_PATH / 'custom'

# ../addons/source-python/data/plugins
PLUGIN_DATA_PATH = DATA_PATH / 'plugins'
