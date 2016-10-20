# ../core/command/auth.py

"""Registers the sp auth sub-commands."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
#   Auth
from auth.manager import auth_manager
#   Commands
from commands.typed import filter_str
from commands.typed import TypedServerCommand
#   Core
from core.command import _core_command
from core.command import core_command_logger
#   Filters
from filters.players import PlayerIter


# =============================================================================
# >> GLOBALS
# =============================================================================
logger = core_command_logger.auth


# =============================================================================
# >> sp auth backend
# =============================================================================
@_core_command.server_sub_command(['auth', 'backend', 'set'])
def _sp_auth_load(command_info, backend):
    """Set the active backend."""
    try:
        auth_manager.set_active_backend(backend)
    except ValueError:
        logger.log_message('Backend "{}" does not exist.'.format(backend))
    else:
        logger.log_message(
            'Backend "{}" has been loaded sucessfully!'.format(backend))

@_core_command.server_sub_command(['auth', 'backend', 'list'])
def _sp_auth_list(command_info):
    """List all available backends and marks the active backend."""
    if not auth_manager:
        logger.log_message('No available backends!')
    else:
        result = 'Available backends:'
        for backend in sorted(auth_manager):
            if auth_manager.is_backend_loaded(backend):
                backend = backend.ljust(15) + ' [ACTIVE]'

            result += '\n  ' + backend

        logger.log_message(result)


# =============================================================================
# >> sp auth permission player
# =============================================================================
@_core_command.server_sub_command(['auth', 'permission', 'player', 'add'])
def _sp_auth_permission_player_add(
        command_info, players:filter_str, permission, server_id:int=None):
    """Grant a permission to players."""
    for player in players:
        if player.permissions is None:
            logger.log_message(
                'Failed to retrieve permissions for {} ({}).'.format(
                    player.name, player.steamid))
            continue

        player.permissions.add(permission, server_id)
        logger.log_message('Granted permission "{}" to {}.'.format(
            permission, player.name))

@_core_command.server_sub_command(['auth', 'permission', 'player', 'remove'])
def _sp_auth_permission_player_remove(
        command_info, players:filter_str, permission, server_id:int=None):
    """Remove a permission from players."""
    for player in players:
        if player.permissions is None:
            logger.log_message(
                'Failed to retrieve permissions for {} ({}).'.format(
                    player.name, player.steamid))
            continue

        player.permissions.remove(permission, server_id)
        logger.log_message(
            'Removed permission "{}" from {}.'.format(
                permission, player.name))

@_core_command.server_sub_command([
    'auth', 'permission', 'player', 'add_parent'
])
def _sp_auth_permission_player_add_parent(
        command_info, players:filter_str, parent):
    """Add a parent to players."""
    for player in players:
        if player.permissions is None:
            logger.log_message(
                'Failed to retrieve permissions for {} ({}).'.format(
                    player.name, player.steamid))
            continue

        player.permissions.add_parent(parent)
        logger.log_message(
            'Added parent "{}" to {}.'.format(parent, player.name))

@_core_command.server_sub_command([
    'auth', 'permission', 'player', 'remove_parent'
])
def _sp_auth_permission_player_remove_parent(
        command_info, players:filter_str, parent):
    """Remove a parent from players."""
    for player in players:
        if player.permissions is None:
            logger.log_message(
                'Failed to retrieve permissions for {} ({}).'.format(
                    player.name, player.steamid))
            continue

        player.permissions.remove_parent(parent)
        logger.log_message(
            'Removed parent "{}" from {}.'.format(parent, player.name))

@_core_command.server_sub_command(['auth', 'permission', 'player', 'test'])
def _sp_auth_permission_player_test(command_info, permission):
    """Test which players are granted a permission."""
    logger.log_message(
        '"{}" is granted to the following players:'.format(permission))
    count = 0
    for player in PlayerIter('human'):
        if player.permissions is None:
            logger.log_message(
                'Failed to retrieve permissions for {} ({}).'.format(
                    player.name, player.steamid))
            continue

        if permission not in player.permissions:
            continue

        count += 1
        logger.log_message('  {} ({})'.format(player.name, player.steamid))

    logger.log_message('{} players are granted the permission "{}".'.format(
        count, permission))


# =============================================================================
# >> sp auth permission parent
# =============================================================================
@_core_command.server_sub_command(['auth', 'permission', 'parent', 'add'])
def _sp_auth_permission_parent_add(
        command_info, parent, permission, server_id:int=None):
    """Add a permission to a parent."""
    auth_manager.get_parent_permissions(parent).add(permission, server_id)
    logger.log_message(
        'Added permission "{}" to parent "{}".'.format(permission, parent))

@_core_command.server_sub_command(['auth', 'permission', 'parent', 'remove'])
def _sp_auth_permission_parent_remove(
        command_info, parent, permission, server_id:int=None):
    """Remove a permission from a parent."""
    auth_manager.get_parent_permissions(parent).remove(permission, server_id)
    logger.log_message('Removed permission "{}" from parent "{}".'.format(
            permission, parent))

@_core_command.server_sub_command([
    'auth', 'permission', 'parent', 'add_parent'
])
def _sp_auth_permission_parent_add_parent(
        command_info, parent, parent_to_add):
    """Add a parent to a parent."""
    auth_manager.get_parent_permissions(parent).add_parent(parent_to_add)
    logger.log_message(
        'Added parent "{}" to parent "{}".'.format(parent_to_add, parent))

@_core_command.server_sub_command([
    'auth', 'permission', 'parent', 'remove_parent'
])
def _sp_auth_permission_parent_remove_parent(
        command_info, parent, parent_to_remove):
    """Remove a parent from a parent."""
    auth_manager.get_parent_permissions(parent).remove_parent(
        parent_to_remove)
    logger.log_message(
        'Removed parent "{}" from parent "{}".'.format(
            parent_to_remove, parent))

@_core_command.server_sub_command([
    'auth', 'permission', 'parent', 'list'
])
def _sp_auth_permission_parent_list(
        command_info, name=None):
    """List all parents and permissions of a given parent or all parents."""
    for parent in auth_manager.parents.values():
        if name is not None and name != parent.name:
            continue

        logger.log_message('{}:'.format(parent.name))
        logger.log_message('  Parents:')
        for parent2 in parent.parents:
            logger.log_message('    {}'.format(parent2.name))

        logger.log_message('  Permissions:')
        for permission in parent:
            logger.log_message('    {}'.format(permission))

    if not auth_manager.parents:
        logger.log_message('No parents found.')


# =============================================================================
# >> DESCRIPTIONS
# =============================================================================
_sp_auth = TypedServerCommand.parser.get_node(['sp', 'auth'])
_sp_auth.description = 'Authorization specific commands.'

_sp_auth_backend = TypedServerCommand.parser.get_node(
    ['sp', 'auth', 'backend'])
_sp_auth_backend.description = 'Authorization backend specific commands.'

_sp_auth_permission = TypedServerCommand.parser.get_node(
    ['sp', 'auth', 'permission'])
_sp_auth_permission.description = 'Commands to modify permissions.'

_sp_auth_permission_parent = TypedServerCommand.parser.get_node(
    ['sp', 'auth', 'permission', 'parent'])
_sp_auth_permission_parent.description = 'Commands to modify parent permissions.'

_sp_auth_permission_player = TypedServerCommand.parser.get_node(
    ['sp', 'auth', 'permission', 'player'])
_sp_auth_permission_player.description = 'Commands to modify player permissions.'
