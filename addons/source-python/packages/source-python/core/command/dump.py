# ../core/command/dump.py

"""Registers the sp dump sub-commands."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
#   Commands
from commands.typed import TypedServerCommand
#   Core
from core import dumps
from core.command import core_command


# =============================================================================
# >> sp dump
# =============================================================================
#: .. todo:: Make file_name optional
@core_command.server_sub_command(['dump', 'class_info'])
def _sp_dump_class_info(command_info, file_name):
    """Dump class info."""
    dumps.dump_class_info(file_name)

@core_command.server_sub_command(['dump', 'convars'])
def _sp_dump_convars(command_info, file_name):
    """Dump convars."""
    dumps.dump_convars(file_name)

@core_command.server_sub_command(['dump', 'datamaps'])
def _sp_dump_datamaps(command_info, file_name):
    """Dump datamaps."""
    dumps.dump_datamaps(file_name)

@core_command.server_sub_command(['dump', 'server_classes'])
def _sp_dump_server_classes(command_info, file_name):
    """Dump server classes."""
    dumps.dump_server_classes(file_name)

@core_command.server_sub_command(['dump', 'string_tables'])
def _sp_dump_string_tables(command_info, file_name):
    """Dump string tables."""
    dumps.dump_string_tables(file_name)

@core_command.server_sub_command(['dump', 'weapon_scripts'])
def _sp_dump_weapon_scripts(command_info, file_name):
    """Dump weapon scripts."""
    dumps.dump_weapon_scripts(file_name)


# =============================================================================
# >> DESCRIPTIONS
# =============================================================================
TypedServerCommand.parser.set_node_description(
    ['sp', 'dump'], 'Dump various data to files.')
