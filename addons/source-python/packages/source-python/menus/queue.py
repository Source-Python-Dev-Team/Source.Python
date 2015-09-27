# ../menus/queue.py

"""Contains menu queues for individual players."""

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
#   Listeners
from listeners import ClientDisconnect
from listeners.tick.repeat import TickRepeat
#   Menus
from menus.base import _BaseMenu


# =============================================================================
# >> CONSTANTS
# =============================================================================
# The name of the client command that used for ESC menus
ESC_SELECTION_CMD = 'escselect'


# =============================================================================
# >> CLASSES
# =============================================================================
class _UserQueue(deque):

    """Controls the user's menu queue.

    Handles selections and decides which menu should be displayed.
    """

    def __init__(self, index):
        """Initialize the queue.

        @param <index>:
        A valid player index.
        """
        super(_UserQueue, self).__init__()
        self._index = index

    def append(self, menu):
        """Make sure we don't add a menu twice."""
        if menu not in self:
            super(_UserQueue, self).append(menu)

    def appendleft(self, menu):
        """Make sure we don't add a menu twice."""
        if menu not in self:
            super(_UserQueue, self).appendleft(menu)

    def extend(self, menus):
        """Make sure we don't add a menu twice."""
        for menu in menus:
            self.append(menu)

    def extendleft(self, menus):
        """Make sure we don't add a menu twice."""
        for menu in menus:
            self.appendleft(menu)

    def __iadd__(self, menus):
        """Make sure we don't add a menu twice."""
        self.extend(menus)

    def __setitem__(self, index, menu):
        """Make sure we don't add a menu twice."""
        if self[index] == menu or menu not in self:
            super(_UserQueue, self).__setitem__(index, menu)

    def _refresh(self):
        """Re-send the current active menu.

        If there is no active menu, nothing will be done.
        """
        menu = self.get_active_menu()
        if menu is not None:
            menu._refresh(self._index)

    def _select(self, choice):
        """Handle a menu selection.

        Removes the current active menu from the queue and forwards the
        selection to that menu.

        If the selection callback returns a menu, it will be append to the
        left side of the queue. After that the queue will be refreshed, so the
        next menu will be displayed.
        """
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
        """Return the first menu instance in this queue.

        If there is no menu, None will be returned.
        """
        return self[0] if self else None


class _ESCUserQueue(_UserQueue):

    """Represents a queue for ESC menus."""

    def __init__(self, index):
        """Initialize the ESC queue."""
        super(_ESCUserQueue, self).__init__(index)

        # TODO: Set this to the highest possible value.
        self.priority = 0


class _QueueHolder(dict):

    """Creates a _UserQueue object for every missing key."""

    def __init__(self, cls, repeat):
        """Initialize the queue holder.

        @param <cls>:
        This is the queue type this class will hold.
        @param <repeat>:
        Global refresh repeat instance.
        """
        super(_QueueHolder, self).__init__()
        self._cls = cls
        self._repeat = repeat

    def __missing__(self, index):
        """Create a new _UserQueue object for the given index.

        Saves the result in this dict.
        """
        # Is the dictionary currently empty?
        if not self:

            # If so, start the refresh repeat...
            self._repeat.start(1, 0)

        obj = self[index] = self._cls(index)
        return obj

    def pop(self, key, default=None):
        """Remove and return the given key's value."""
        return_value = super(_QueueHolder, self).pop(key, default)

        # Is the dictionary currently empty?
        if not self:

            # If so, stop the refresh repeat...
            self._repeat.stop()

        # Return the popped key's value...
        return return_value


# =============================================================================
# >> FUNCTIONS
# =============================================================================
def _validate_selection(command, index, valid_choices):
    """Validate a selection command.

    @param <command>:
    A Command instance.

    @param <index>:
    The player index that issued the command.

    @param <valid_choices>:
    A list of integers that defines all valid choices
    """
    try:
        choice = int(command.get_arg(1))
    except ValueError:
        # Catch errors caused by e.g. "menuselect a"
        return (None, None)

    if choice in valid_choices:
        return (index, choice)

    return (None, None)


# =============================================================================
# >> REPEATS
# =============================================================================
@TickRepeat
def _radio_refresh():
    """Update every queue in the queue dict."""
    for queue in _radio_queues.values():
        queue._refresh()


@TickRepeat
def _esc_refresh():
    """Update every queue in the queue dict."""
    for queue in _esc_queues.values():
        queue._refresh()


# =============================================================================
# >> GLOBAL VARIABLES
# =============================================================================
# {<user index>: <_UserQueue>}
_radio_queues = _QueueHolder(_UserQueue, _radio_refresh)
_esc_queues = _QueueHolder(_ESCUserQueue, _esc_refresh)


# =============================================================================
# >> CLIENT COMMANDS
# =============================================================================
@ClientCommand('menuselect')
def _menuselect_callback(command, index):
    """Forward the selection to the proper user queue."""
    from menus.radio import VALID_CHOICES

    index, choice = _validate_selection(command, index, VALID_CHOICES)
    if index is not None:
        _radio_queues[index]._select(choice)


@ClientCommand(ESC_SELECTION_CMD)
def _escselect_callback(command, index):
    """Forward the selection to the proper user queue."""
    from menus.esc import VALID_CHOICES

    index, choice = _validate_selection(command, index, VALID_CHOICES)
    if index is not None:
        _esc_queues[index]._select(choice)

    return CommandReturn.BLOCK


# =============================================================================
# >> LISTENERS
# =============================================================================
@ClientDisconnect
def on_player_disconnect(player_index):
    """Remove the user queue for the disconnected player."""
    _radio_queues.pop(player_index, 0)
    _esc_queues.pop(player_index, 0)
