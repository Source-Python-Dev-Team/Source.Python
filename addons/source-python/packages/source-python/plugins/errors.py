# ../plugins/errors.py

"""Contains custom exceptions that can be raised within the plugins package."""

# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('PluginFileNotFoundError',
           'PluginInstanceError',
           'PluginManagerError',
           )


# =============================================================================
# >> CLASSES
# =============================================================================
class PluginFileNotFoundError(FileNotFoundError):
    """Plugin file not found."""


class PluginInstanceError(TypeError):
    """Improper plugin instance class assigned."""

PluginInstanceError.__doc__ += (
    '  Class must be inherited from plugins.instance.LoadedPlugin')


class PluginManagerError(TypeError):
    """Improper plugin manager assigned."""

PluginManagerError.__doc__ += (
    '  Object must be an instance of a class that inherits ' +
    'from plugins.manager.PluginManager')
