# ../plugins/errors.py

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
from public import public


# =============================================================================
# >> CLASSES
# =============================================================================
@public
class PluginFileNotFoundError(Exception):
    '''Plugin file not found.'''


@public
class PluginInstanceError(Exception):
    '''Improper plugin instance class assigned.'''

PluginInstanceError.__doc__ += (
    '  Class must be inherited from plugins.instance.LoadedPlugin')


@public
class PluginManagerError(Exception):
    '''Improper plugin manager assigned.'''

PluginManagerError.__doc__ += (
    '  Object must be an instance of a class that inherits ' +
    'from plugins.manager.PluginManager')
