# ../listeners/_run_command.py

"""Provides a run command listener."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
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


# =============================================================================
# >> CALLBACKS
# =============================================================================
@EntityPreHook(EntityCondition.is_player, 'run_command')
def _pre_player_run_command(args):
    if (not on_player_run_command_listener_manager
            and not on_button_state_changed_listener_manager):
        return

    player = make_object(Player, args[0])
    user_cmd = make_object(UserCmd, args[1])

    if on_player_run_command_listener_manager:
        on_player_run_command_listener_manager.notify(player, user_cmd)

    if on_button_state_changed_listener_manager:
        _handle_button_state_changed(player, player.buttons, user_cmd.buttons)


def _handle_button_state_changed(player, old_buttons, new_buttons):
    if old_buttons == new_buttons:
        return

    on_button_state_changed_listener_manager.notify(
        player, old_buttons, new_buttons)

