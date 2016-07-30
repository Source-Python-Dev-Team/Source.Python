# ../core/command/plugin.py

"""Registers the sp plugin sub-commands."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Python Imports
#   Warnings
from warnings import warn
# Source.Python Imports
#   Commands
from commands.typed import TypedServerCommand
#   Core
from core.command import _core_command
from core.command import core_command_logger


# =============================================================================
# >> GLOBALS
# =============================================================================
logger = core_command_logger.plugin


# =============================================================================
# >> sp plugin
# =============================================================================
@_core_command.server_sub_command(['plugin', 'load'])
def _sp_plugin_load(command_info, plugin):
    """Load a plugin."""
    _core_command.load_plugin(plugin)
    
@_core_command.server_sub_command(['load'])
def _sp_load(command_info, plugin):
    """Load a plugin."""
    warn('"sp load" will be removed soon. Use "sp plugin load" instead.',
        DeprecationWarning)
    _core_command.load_plugin(plugin)

@_core_command.server_sub_command(['plugin', 'unload'])
def _sp_plugin_unload(command_info, plugin):
    """Unload a plugin."""
    _core_command.unload_plugin(plugin)

@_core_command.server_sub_command(['unload'])
def _sp_unload(command_info, plugin):
    """Unload a plugin."""
    warn('"sp unload" will be removed soon. Use "sp plugin unload" instead.',
        DeprecationWarning)
    _core_command.unload_plugin(plugin)

@_core_command.server_sub_command(['plugin', 'reload'])
def _sp_plugin_reload(command_info, plugin):
    """Reload a plugin."""
    _core_command.reload_plugin(plugin)

@_core_command.server_sub_command(['reload'])
def _sp_reload(command_info, plugin):
    """Reload a plugin."""
    warn('"sp reload" will be removed soon. Use "sp plugin reload" instead.',
        DeprecationWarning)
    _core_command.reload_plugin(plugin)

@_core_command.server_sub_command(['plugin', 'list'])
def _sp_plugin_list(command_info):
    """List all currently loaded plugins."""
    _core_command.print_plugins()

@_core_command.server_sub_command(['list'])
def _sp_list(command_info):
    """List all currently loaded plugins."""
    warn('"sp list" will be removed soon. Use "sp plugin list" instead.',
        DeprecationWarning)
    _core_command.print_plugins()


# =============================================================================
# >> DESCRIPTIONS
# =============================================================================
_sp_plugin = TypedServerCommand.parser.get_node(['sp', 'plugin'])
_sp_plugin.description = 'Plugin specific commands.'
