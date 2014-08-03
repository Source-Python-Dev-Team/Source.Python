# ../core/manager.py

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
#   Core
from core import CoreLogger
#   Plugins
from plugins.manager import PluginManager


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('SPPluginManager',
           )


# =============================================================================
# >> GLOBAL VARIABLES
# =============================================================================
# Get the sp.core.manager logger
CoreManagerLogger = CoreLogger.manager


# =============================================================================
# >> CLASSES
# =============================================================================
class _SPPluginManager(PluginManager):
    '''Plugin Manager class used to load "sp" plugins'''
    logger = CoreManagerLogger

# Get the _SPPluginManager instance
SPPluginManager = _SPPluginManager()
