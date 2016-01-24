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


# =============================================================================
# >> sp dump
# =============================================================================
#: .. todo:: Make file_name optional
@TypedServerCommand(['sp', 'dump', 'class_info'])
def _sp_dump_class_info(command_info, file_name):
    """Dump class info."""
    dumps.dump_class_info(file_name)

@TypedServerCommand(['sp', 'dump', 'convars'])
def _sp_dump_convars(command_info, file_name):
    """Dump convars."""
    dumps.dump_convars(file_name)

@TypedServerCommand(['sp', 'dump', 'datamaps'])
def _sp_dump_datamaps(command_info, file_name):
    """Dump datamaps."""
    dumps.dump_datamaps(file_name)

@TypedServerCommand(['sp', 'dump', 'server_classes'])
def _sp_dump_server_classes(command_info, file_name):
    """Dump server classes."""
    dumps.dump_server_classes(file_name)

@TypedServerCommand(['sp', 'dump', 'string_tables'])
def _sp_dump_string_tables(command_info, file_name):
    """Dump string tables."""
    dumps.dump_string_tables(file_name)

@TypedServerCommand(['sp', 'dump', 'weapon_scripts'])
def _sp_dump_weapon_scripts(command_info, file_name):
    """Dump weapon scripts."""
    dumps.dump_weapon_scripts(file_name)


# =============================================================================
# >> DESCRIPTIONS
# =============================================================================
_sp_dump = TypedServerCommand.parser.get_node(['sp', 'dump'])
_sp_dump.description = 'Dump various data to files.'
