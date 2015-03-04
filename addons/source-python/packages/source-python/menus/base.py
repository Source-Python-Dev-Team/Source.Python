# ../menus/base.py

"""Contains base menu functionality."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Python Imports
#   Collections
import math

from collections import defaultdict

# Source.Python Imports
#   Core
from core import AutoUnload
#   Filters
from filters.players import PlayerIter
#   Listeners
from listeners import ClientDisconnect
#   Players
from players.helpers import get_client_language
#   Translations
from translations.strings import TranslationStrings


# =============================================================================
# >> CLASSES
# =============================================================================
class _PlayerPage(object):

    """Saves information about the player's current active page."""

    def __init__(self):
        """Initialize the _PlayerPage instance."""
        self.index = 0
        self.options = {}


class _BaseMenu(AutoUnload, list):

    """The base menu. Every menu class should inherit from this class."""

    _instances = {}

    def __init__(self, data=None, select_callback=None, build_callback=None):
        """Initialize the menu.

        @param <data>:
        An iterable which contains data that should be added to the menu.

        @param <select_callback>:
        A callable object that gets called whenever a selection was made.

        The callback will recieve 3 parameters:
            1. The instance of this menu.
            2. The player's index who made the selection.
            3. The player's choice.

        @param <build_callback>:
        A callable object that gets called before a menu is displayed.

        The callback will recieve 2 parameters:
            1. The instance of this menu.
            2. The index of the player who will recieve this menu.
        """
        super(_BaseMenu, self).__init__(list() if data is None else data)

        self.select_callback = select_callback
        self.build_callback = build_callback
        self._player_pages = defaultdict(_PlayerPage)
        self._instances[id(self)] = self

    def _unload_instance(self):
        """Unload the instance.

        This will close this menu instance for every player.
        """
        # Just close all open menus, which will remove all instances from the
        # queues
        self.close()

        # Also remove the instance from the _instances dict
        del self._instances[id(self)]

    def _unload_player(self, player_index):
        """Remove every player specific information.

        @param <player_index>:
        A player index.
        """
        self._player_pages.pop(player_index, 0)

    def _refresh(self, player_index):
        """Re-send the menu to a player.

        @param <player_index>:
        The index of the player whose menu should be refreshed.
        """
        self._send(player_index)

    def _build(self, player_index):
        """Call the build callback and return all relevant menu data.

        @param <player_index>:
        The index of the player whose menu should be built.
        """
        # Call the build callback if there is one
        if self.build_callback is not None:
            self.build_callback(self, player_index)

        return self._get_menu_data(player_index)

    def _select(self, player_index, choice_index):
        """Handle a menu selection.

        @param <player_index>:
        The index of the player who made the selection.

        @param <choice_index>:
        A numeric value that defines what was selected.
        """
        if self.select_callback is not None:
            return self.select_callback(self, player_index, choice_index)

    def send(self, *ply_indexes):
        """Send the menu to the given player indexes.

        If no indexes were given, the menu will be sent to every player.
        """
        if not ply_indexes:
            ply_indexes = PlayerIter('human')

        for player_index in ply_indexes:
            queue = self.get_user_queue(player_index)
            queue.append(self)
            queue._refresh()

    def close(self, *ply_indexes):
        """Close the menu for the given player indexes.

        If no indexes were given, the menu will be closed for all players.
        """
        if not ply_indexes:
            ply_indexes = PlayerIter('human')

        for player_index in ply_indexes:
            queue = self.get_user_queue(player_index)

            # Try to remove this menu from the queue
            try:
                queue.remove(self)
            except ValueError:
                # If it fails, do nothing
                pass
            else:
                # If the queue is now empty, send an empty menu to hide
                # the last menu
                if not queue:
                    # Send an empty menu
                    self._close(player_index)

                else:
                    # There is at least one menu in the queue, so refresh to
                    # display it.
                    queue._refresh()

    def is_active_menu(self, player_index):
        """Return True if this menu is the first menu in the user's queue.

        @param <player_index>:
        A player index.
        """
        return self.get_user_queue(player_index).get_active_menu() is self

    @classmethod
    def get_user_queue(cls, player_index):
        """Return the menu queue for the given player.

        @param <player_index>:
        A player index.
        """
        return cls._get_queue_holder()[player_index]

    @staticmethod
    def _get_queue_holder():
        """Return a _QueueHolder object.

        This method needs to be implemented by a subclass!
        """
        raise NotImplementedError

    def _get_menu_data(self, player_index):
        """Return the required data to send a menu.

        This method needs to be implemented by a subclass!

        @param <player_index>:
        A player index.
        """
        raise NotImplementedError

    def _send(self, player_index):
        """Send a menu to the player.

        This method needs to be implemented by a subclass!

        @param <player_index>:
        A player index.
        """
        raise NotImplementedError

    def _close(self, player_index):
        """Close a menu for the player by sending an empty menu.

        This method needs to be implemented by a subclass!

        @param <player_index>:
        A player index.
        """
        raise NotImplementedError


class _MenuData(object):

    """Base class for menu data.

    All data types should inherit from this class.
    """

    def __init__(self, text, **kwargs):
        """Initialize the instance.

        @param <text>:
        The text that should be displayed.

        @param <kwars>:
        Keyword arguments passed to the _translate_text function.
        """
        self.text = text
        self.kwargs = kwargs

    def _render(self, player_index, choice_index=None):
        """Render the data.

        @param <player_index>:
        A player index.

        @param <choice_index>:
        The number that was selected. It depends on the menu type if this
        parameter gets passed.
        """
        raise NotImplementedError


class _PagedMenuBase(object):

    """Implements the base of every page based menu."""

    def _get_max_item_count(self):
        """Return the maximum possible item count per page."""
        raise NotImplementedError

    def _get_options(self, page_index):
        """Return a tuple containing the options for the given page index."""
        item_count = self._get_max_item_count()
        return self[page_index * item_count: (page_index + 1) * item_count]

    @property
    def last_page_index(self):
        """Return the index of the last page."""
        return self.page_count - 1

    @property
    def page_count(self):
        """Return the number of pages the menu currently has."""
        return int(math.ceil(len(self) / self._get_max_item_count())) or 1

    def set_player_page(self, player_index, page_index):
        """Set the player's current page index.

        @param <player_index>:
        A player index.

        @param <page_index>:
        An index that defines the current active page.

        If <page_index> is lower than 0, the page index will be set to 0.

        If <page_index> is greater than the last page index, it will be set to
        the last page index.
        """
        page = self._player_pages[player_index]
        if page_index < 0:
            page.index = 0
        elif page_index > self.last_page_index:
            page.index = self.last_page_index
        else:
            page.index = page_index

    def get_player_page(self, player_index):
        """Return the current player page index.

        @param <player_index>:
        A player index.
        """
        return self._player_pages[player_index].index


class Text(_MenuData):

    """Display plain text."""

    def _render(self, player_index, choice_index=None):
        """Render the data.

        @param <player_index>:
        A player index.

        @param <choice_index>:
        The number should be required to select this item. It depends on the
        menu type if this parameter gets passed.
        """
        return '{0}\n'.format(
            _translate_text(self.text, player_index, **self.kwargs)
        )


class _BaseOption(_MenuData):

    """This class is used to display an enumerated option."""

    def __init__(
            self, text, value=None, highlight=True,
            selectable=True, **kwargs):
        """Initialize the option.

        @param <text>:
        The text that should be displayed.

        @param <value>:
        The value that should be passed to the menu's selection callback.

        @param <hightlight>:
        Set this to true if the text should be hightlighted.

        @param <selectable>:
        Set this to True if the option should be selectable.

        @param <kwars>:
        Keyword arguments passed to the _translate_text function.
        """
        super(_BaseOption, self).__init__(text, **kwargs)
        self.value = value
        self.highlight = highlight
        self.selectable = selectable

    def _render(self, player_index, choice_index=None):
        """Render the data.

        @param <player_index>:
        A player index.

        @param <choice_index>:
        The number should be required to select this item. It depends on the
        menu type if this parameter gets passed.
        """
        raise NotImplementedError


# =============================================================================
# >> HELPER FUNCTIONS
# =============================================================================
def _translate_text(text, player_index, **kwargs):
    """Translate <text> if it is an instance of TranslationStrings.

    Otherwise the original text will be returned.
    """
    if isinstance(text, TranslationStrings):
        return text.get_string(get_client_language(player_index), **kwargs)

    return text


# =============================================================================
# >> LISTENERS
# =============================================================================
@ClientDisconnect
def on_player_disconnect(player_index):
    """Called whenever a player left the server."""
    for instance in _BaseMenu._instances.values():
        instance._unload_player(player_index)
