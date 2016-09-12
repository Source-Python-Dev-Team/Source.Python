# ../core/command/plugin.py

"""Registers the sp plugin sub-commands."""

# =============================================================================
# >> IMPORTS
# =============================================================================
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

@_core_command.server_sub_command(['plugin', 'unload'])
def _sp_plugin_unload(command_info, plugin):
    """Unload a plugin."""
    _core_command.unload_plugin(plugin)

@_core_command.server_sub_command(['plugin', 'reload'])
def _sp_plugin_reload(command_info, plugin):
    """Reload a plugin."""
    _core_command.reload_plugin(plugin)

@_core_command.server_sub_command(['plugin', 'list'])
def _sp_plugin_list(command_info):
    """List all currently loaded plugins."""
    _core_command.print_plugins()


# =============================================================================
# >> DESCRIPTIONS
# =============================================================================
_sp_plugin = TypedServerCommand.parser.get_node(['sp', 'plugin'])
_sp_plugin.description = 'Plugin specific commands.'
