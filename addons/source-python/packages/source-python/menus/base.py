# ../menus/base.py

"""Contains base menu functionality."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Python Imports
#   Collections
from collections import defaultdict
#   Math
import math
#   Weakref
from weakref import WeakValueDictionary

# Source.Python Imports
#   Core
from core import WeakAutoUnload
#   Filters
from filters.recipients import RecipientFilter
#   Listeners
from listeners import OnClientDisconnect
#   Players
from players.helpers import get_client_language
#   Translations
from translations.strings import TranslationStrings


# =============================================================================
# >> CLASSES
# =============================================================================
class _PlayerPage(object):
    """Stores information about the player's current active page."""

    def __init__(self):
        """Initialize the object."""
        self.index = 0
        self.options = {}


class _BaseMenu(WeakAutoUnload, list):
    """The base menu. Every menu class should inherit from this class."""

    _instances = WeakValueDictionary()

    def __init__(self, data=None, select_callback=None, build_callback=None, close_callback=None):
        """Initialize the menu.

        :param iterable|None data: Data that should be added to the menu.
        :param callable|None select_callback: A function that gets called
            whenever a selection was made.

            The callback will receive 3 parameters:
                1. The instance of this menu.
                2. The player's index who made the selection.
                3. The player's choice.

        :param callable|None build_callback: A function that gets called
            before a menu is displayed.

            The callback will receive 2 parameters:
                1. The instance of this menu.
                2. The index of the player who will receive this menu.

        :param callable|None close_callback: A function that gets called
            when a menu is closed by a player.

            The callback will receive 2 parameters:
                1. The instance of this menu.
                2. The index of the player who will close this menu.
        """
        super().__init__(list() if data is None else data)

        self.select_callback = select_callback
        self.build_callback = build_callback
        self.close_callback = close_callback
        self._player_pages = defaultdict(_PlayerPage)
        self._instances[id(self)] = self

    def _unload_instance(self):
        """Close this menu object for every player."""
        # Just close all open menus, which will remove all instances from the
        # queues.
        self.close()

        # Also remove the instance from the _instances dict.
        # This process is necessary because there is no guarantee that
        # the instance will be destroyed when the plugin is unloaded.
        del self._instances[id(self)]

    def _unload_player(self, player_index):
        """Remove every player specific information.

        :param int player_index: A player index.
        """
        self._player_pages.pop(player_index, 0)

    def _refresh(self, player_index):
        """Re-send the menu to a player.

        :param int player_index: The index of the player whose menu should be
            refreshed.
        """
        self._send(player_index)

    def _build(self, player_index):
        """Call the build callback and return all relevant menu data.

        :param int player_index: The index of the player whose menu should be
            built.
        """
        # Call the build callback if there is one
        if self.build_callback is not None:
            self.build_callback(self, player_index)

        return self._get_menu_data(player_index)

    def _select(self, player_index, choice_index):
        """Handle a menu selection.

        :param int player_index: The index of the player who made the
            selection.
        :param int choice_index: Defines what was selected.
        """
        if self.select_callback is not None:
            return self.select_callback(self, player_index, choice_index)

    def _select_close(self, player_index):
        """Handle the close menu selection.

        :param int player_index: The index of the player who made the
            selection.
        """
        if self.close_callback is not None:
            return self.close_callback(self, player_index)

    def send(self, *ply_indexes, **tokens):
        """Send the menu to the given player indexes.

        If no indexes were given, the menu will be sent to every player.

        :param ply_indexes:
            Player indexes that should receive the menu.
        :param tokens:
            Translation tokens for menu options, title and description.
        """
        ply_indexes = tuple(RecipientFilter(*ply_indexes))
        if not ply_indexes:
            return

        # Update option tokens
        for option in self:
            try:
                option.text.tokens.update(**tokens)
            except AttributeError:
                # Not a _MenuData or TranslationStrings instance
                pass

        # Update title if existant
        try:
            self.title.tokens.update(**tokens)
        except AttributeError:
            pass

        # Update description if existant
        try:
            self.description.tokens.update(**tokens)
        except AttributeError:
            pass

        for player_index in ply_indexes:
            queue = self.get_user_queue(player_index)
            queue.append(self)
            queue._refresh()

    def close(self, *ply_indexes):
        """Close the menu for the given player indexes.

        If no indexes were given, the menu will be closed for all players.
        """
        ply_indexes = RecipientFilter(*ply_indexes)
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

        :param int player_index: A player index.
        """
        return self.get_user_queue(player_index).active_menu is self

    @classmethod
    def get_user_queue(cls, player_index):
        """Return the menu queue for the given player.

        :param int player_index: A player index.
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

        :param int player_index: A player index.
        """
        raise NotImplementedError

    def _send(self, player_index):
        """Send a menu to the player.

        This method needs to be implemented by a subclass!

        :param int player_index: A player index.
        """
        raise NotImplementedError

    def _close(self, player_index):
        """Close a menu for the player by sending an empty menu.

        This method needs to be implemented by a subclass!

        :param int player_index: A player index.
        """
        raise NotImplementedError

    def register_select_callback(self, callback):
        """Register a select callback for the menu.

        Can and should be used as a decorator.

        :param callable callback: A function that gets called
            whenever a selection was made.

            The callback will receive 3 parameters:
                1. The instance of this menu.
                2. The player's index who made the selection.
                3. The player's choice.
        """
        self.select_callback = callback
        return callback

    def register_build_callback(self, callback):
        """Register a build callback for the menu.

        Can and should be used as a decorator.

        :param callable callback: A function that gets called
            before a menu is displayed.

            The callback will receive 2 parameters:
                1. The instance of this menu.
                2. The index of the player who will receive this menu.
        """
        self.build_callback = callback
        return callback

    def register_close_callback(self, callback):
        """Register a close callback for the menu.

        Can and should be used as a decorator.

        :param callable callback: A function that gets called
            when a menu is closed by a player.

            The callback will receive 2 parameters:
                1. The instance of this menu.
                2. The index of the player who will receive this menu.
        """
        self.close_callback = callback
        return callback


class _MenuData(object):
    """Base class for menu data.

    All data types should inherit from this class.
    """

    def __init__(self, text):
        """Initialize the instance.

        :param str text: The text that should be displayed.
        """
        self.text = text

    def _render(self, player_index, choice_index=None):
        """Render the data.

        :param int player_index: A player index.
        :param int|None choice_index: The number that was selected.
            It depends on the menu type if this parameter gets passed.
        """
        raise NotImplementedError


class _PagedMenuBase(object):
    """Implements the base of every page based menu."""

    @staticmethod
    def _get_max_item_count():
        """Return the maximum possible item count per page."""
        raise NotImplementedError

    def _get_options(self, page_index):
        """Return a tuple containing the options for the given page index.

        :param int page_index: The index of the page.
        """
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

        :param int player_index: A player index.
        :param int page_index: The current active page index.

        If ``page_index`` is lower than 0, the page index will be set to 0.

        If ``page_index`` is greater than the last page index, it will be set
        to the last page index.
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

        :param int player_index: A player index.
        """
        return self._player_pages[player_index].index


class Text(_MenuData):
    """Display plain text."""

    def _render(self, player_index, choice_index=None):
        """See :meth:`_MenuData._render`."""
        return str(_translate_text(self.text, player_index)) + '\n'


class _BaseOption(_MenuData):
    """This class is used to display an enumerated option."""

    def __init__(self, text, value=None, highlight=True, selectable=True):
        """Initialize the option.

        :param str text: The text that should be displayed.
        :param value: The value that should be passed to the menu's
            selection callback.
        :param bool highlight: Set this to true if the text should be
            highlighted.
        :param bool selectable: Set this to True if the option should be
            selectable.
        """
        super().__init__(text)
        self.value = value
        self.highlight = highlight
        self.selectable = selectable

    def _render(self, player_index, choice_index=None):
        """See :meth:`_MenuData._render`."""
        raise NotImplementedError


# =============================================================================
# >> HELPER FUNCTIONS
# =============================================================================
def _translate_text(text, player_index):
    """Translate the given ``text``.

    Only translate if ``text`` is an instance of
    :class:`translations.strings.TranslationStrings`. Otherwise the original
    text will be returned.
    """
    if isinstance(text, TranslationStrings):
        return text.get_string(get_client_language(player_index))

    return text


# =============================================================================
# >> LISTENERS
# =============================================================================
@OnClientDisconnect
def on_player_disconnect(player_index):
    """Called whenever a player left the server."""
    for instance in _BaseMenu._instances.values():
        instance._unload_player(player_index)
