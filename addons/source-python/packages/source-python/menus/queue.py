# ../menus/queue.py

# =============================================================================
# >> IMPORTS
# =============================================================================
# Python Imports
#   Collections
from collections import deque

# Source.Python Imports
#   Commands
from commands import CommandReturn
from commands.client import ClientCommand
#   Events
from events import Event
#   Listeners
from listeners.tick.repeat import TickRepeat
#   Menus
from menus.base import _BaseMenu
#   Players
from players.helpers import index_from_playerinfo
from players.helpers import index_from_userid


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ()


# =============================================================================
# >> CONSTANTS
# =============================================================================
# The name of the client command that used for ESC menus
ESC_SELECTION_CMD = 'escselect'


# =============================================================================
# >> CLASSES
# =============================================================================
class _UserQueue(deque):
    '''
    This class controls the user's menu queue. It handles selections and
    decides which menu should be displayed.
    '''

    def __init__(self, index):
        '''
        Initializes the queue.

        @param <index>:
        A valid player index.
        '''

        self._index = index

    def append(self, menu):
        # Make sure we don't add a menu twice
        if menu not in self:
            super(_UserQueue, self).append(menu)

    def appendleft(self, menu):
        # Make sure we don't add a menu twice
        if menu not in self:
            super(_UserQueue, self).appendleft(menu)

    def extend(self, menus):
        # Make sure we don't add a menu twice
        for menu in menus:
            self.append(menu)

    def extendleft(self, menus):
        # Make sure we don't add a menu twice
        for menu in menus:
            self.appendleft(menu)

    def __iadd__(self, menus):
        # Make sure we don't add a menu twice
        self.extend(menus)

    def __setitem__(self, index, menu):
        # Make sure we don't add a menu twice
        if self[index] == menu or menu not in self:
            super(_UserQueue, self).__setitem__(index, menu)

    def _refresh(self):
        '''
        Re-sends the current active menu.

        If there is no active menu, nothing will be done.
        '''

        menu = self.get_active_menu()
        if menu is not None:
            menu._refresh(self._index)

    def _select(self, choice):
        '''
        Removes the current active menu from the queue and forwards the
        selection to that menu.

        If the selection callback returns a menu, it will be append to the
        left side of the queue. After that the queue will be refreshed, so the
        next menu will be displayed.
        '''

        # Remove the current active menu
        try:
            active_menu = self.popleft()
        except IndexError:
            return

        # Forward the selection to the menu
        next_menu = active_menu._select(self._index, choice)

        # Is the return value a menu?
        if isinstance(next_menu, _BaseMenu):
            # Get the queue of the returned menu. Maybe it is a different menu
            # type. In that case it needs to be appended to the other queue.
            queue = next_menu.get_user_queue(self._index)

            # Append the queue to the left, so it will be directly displayed.
            queue.appendleft(next_menu)

            # If the queue belongs to a different queue, refresh that queue.
            if self is not queue:
                queue._refresh()

        # Always refresh this queue if the queue is not empty
        if self:
            self._refresh()

    def get_active_menu(self):
        '''
        Returns the first menu instance in this queue.

        If there is no menu, None will be returned.
        '''

        return self[0] if self else None


class _ESCUserQueue(_UserQueue):
    '''
    This class represents a queue for ESC menus.
    '''

    def __init__(self, index):
        '''
        Initializes the ESC queue.
        '''

        super(_ESCUserQueue, self).__init__(index)

        # TODO: Set this to the highest possible value.
        self.priority = 0


class _QueueHolder(dict):
    '''
    This class is a default dictionary, which creates an _UserQueue object for
    every missing key.
    '''

    def __init__(self, cls):
        '''
        Initializes the queue holder.

        @param <cls>:
        This is the queue type this class will hold.
        '''

        self._cls = cls

    def __missing__(self, index):
        '''
        Creates a new _UserQueue object for the given index and saves the
        result in this dict.
        '''

        obj = self[index] = self._cls(index)
        return obj


# =============================================================================
# >> GLOBAL VARIABLES
# =============================================================================
# {<user index>: <_UserQueue>}
_radio_queues = _QueueHolder(_UserQueue)
_esc_queues = _QueueHolder(_ESCUserQueue)


# =============================================================================
# >> FUNCTIONS
# =============================================================================
def _validate_selection(player_info, command, max_choice):
    '''
    Validates a selection command and returns the index and the player's
    selection index.

    @param <player_infor>:
    A PlayerInfo instance.

    @param <command>:
    A Command instance.

    @param <max_choice>:
    The highest possible selection index. If the index was greater,
    <max_choice> will be used.
    '''

    try:
        choice = int(command.get_arg(1))
    except ValueError:
        # Catch errors caused by e.g. "menuselect a"
        return (None, None)

    # Don't allow negative selections
    if choice < 0:
        return (None, None)

    return (index_from_playerinfo(
        player_info), 0 if choice > max_choice else choice)


# =============================================================================
# >> REPEATS
# =============================================================================
@TickRepeat
def _radio_refresh():
    '''
    Updates every queue in the queue dict.
    '''

    for queue in _radio_queues.values():
        queue._refresh()

# TODO: Get the refresh repeat dynamically
_radio_refresh.start(1, 0)


@TickRepeat
def _esc_refresh():
    '''
    Updates every queue in the queue dict.
    '''

    for queue in _esc_queues.values():
        queue._refresh()

# TODO: Get the refresh repeat dynamically
_esc_refresh.start(1, 0)


# =============================================================================
# >> CLIENT COMMANDS
# =============================================================================
@ClientCommand('menuselect')
def _menuselect_callback(player_info, command):
    '''
    Forwards the selection to the proper user queue.
    '''

    index, choice = _validate_selection(player_info, command, 9)
    if index is not None:
        _radio_queues[index]._select(choice)


@ClientCommand(ESC_SELECTION_CMD)
def _escselect_callback(player_info, command):
    '''
    Forwards the selection to the proper user queue.
    '''

    index, choice = _validate_selection(player_info, command, 7)
    if index is not None:
        _esc_queues[index]._select(choice)

    return CommandReturn.BLOCK


# =============================================================================
# >> GAME EVENTS
# =============================================================================
@Event
def player_disconnect(event):
    '''
    Removes the user queue for the disconnected player.
    '''

    index = index_from_userid(event.get_int('userid'))
    _radio_queues.pop(index, 0)
    _esc_queues.pop(index, 0)
