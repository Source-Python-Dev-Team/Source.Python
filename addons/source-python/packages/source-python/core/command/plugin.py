# ../core/command/plugin.py

"""Registers the sp plugin sub-commands."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
#   Commands
from commands.typed import TypedClientCommand
from commands.typed import TypedServerCommand
#   Core
from core.command import core_command
from core.command import core_command_logger


# =============================================================================
# >> GLOBALS
# =============================================================================
logger = core_command_logger.plugin


# =============================================================================
# >> sp plugin
# =============================================================================
@core_command.server_sub_command(['plugin', 'load'])
@core_command.client_sub_command(['plugin', 'load'], 'sp.plugin.load')
def _sp_plugin_load(command_info, plugin):
    """Load a plugin."""
    core_command.load_plugin(plugin)


@core_command.server_sub_command(['plugin', 'unload'])
@core_command.client_sub_command(['plugin', 'unload'], 'sp.plugin.unload')
def _sp_plugin_unload(command_info, plugin):
    """Unload a plugin."""
    core_command.unload_plugin(plugin)


@core_command.server_sub_command(['plugin', 'reload'])
@core_command.client_sub_command(['plugin', 'reload'], 'sp.plugin.reload')
def _sp_plugin_reload(command_info, plugin):
    """Reload a plugin."""
    core_command.reload_plugin(plugin)


@core_command.server_sub_command(['plugin', 'list'])
@core_command.client_sub_command(['plugin', 'list'], 'sp.plugin.list')
def _sp_plugin_list(command_info):
    """List all currently loaded plugins."""
    core_command.print_plugins()


# =============================================================================
# >> DESCRIPTIONS
# =============================================================================
TypedServerCommand.parser.set_node_description(
    ['sp', 'plugin'], 'Plugin specific commands.')
TypedClientCommand.parser.set_node_description(
    ['sp', 'plugin'], 'Plugin specific commands.')
