# ../listeners/_run_command.py

"""Provides a run command listener."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
#   Core
from core import SOURCE_ENGINE_BRANCH
#   Entities
from entities.hooks import EntityPreHook
from entities.hooks import EntityCondition
#   Listeners
from listeners import on_player_run_command_listener_manager
from listeners import on_button_state_changed_listener_manager
#   Players
from players import UserCmd
from players.entity import Player
#   Memory
from memory import make_object
from memory import get_object_pointer
from memory import get_size


# =============================================================================
# >> CONSTANTS
# =============================================================================
USER_CMD_SIZE = get_size(UserCmd)


# =============================================================================
# >> CALLBACKS
# =============================================================================
@EntityPreHook(EntityCondition.is_player, 'run_command')
def _pre_player_run_command(args):
    if (not on_player_run_command_listener_manager
            and not on_button_state_changed_listener_manager):
        return

    player = make_object(Player, args[0])

    # https://github.com/Source-Python-Dev-Team/Source.Python/issues/149
    use_rtti_fix = SOURCE_ENGINE_BRANCH == 'tf2' and player.is_fake_client()
    if use_rtti_fix:
        user_cmd = UserCmd()
        user_cmd_ptr = get_object_pointer(user_cmd)
        args[1].copy(user_cmd_ptr, USER_CMD_SIZE)
    else:
        user_cmd = make_object(UserCmd, args[1])

    if on_player_run_command_listener_manager:
        on_player_run_command_listener_manager.notify(player, user_cmd)

    if on_button_state_changed_listener_manager:
        _handle_button_state_changed(player, player.buttons, user_cmd.buttons)

    if use_rtti_fix:
        user_cmd_ptr.copy(args[1], USER_CMD_SIZE)


def _handle_button_state_changed(player, old_buttons, new_buttons):
    if old_buttons == new_buttons:
        return

    on_button_state_changed_listener_manager.notify(
        player, old_buttons, new_buttons)
