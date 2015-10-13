# ../menus/esc.py

"""Provides ESC Menu functionality."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
#   Colors
from colors import WHITE
#   Entities
from entities.helpers import edict_from_index
#   Keyvalues
from keyvalues import KeyValues
#   Menus
from menus.base import _BaseMenu
from menus.base import _PagedMenuBase
from menus.base import _BaseOption
from menus.base import _translate_text
from menus.queue import ESC_SELECTION_CMD
from menus.queue import _esc_queues
#   Messages
from _messages import DialogType
from _messages import create_message


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('PagedESCMenu',
           'PagedESCOption',
           'SimpleESCMenu',
           'SimpleESCOption',
           )


# =============================================================================
# >> CONSTANTS
# =============================================================================
VALID_CHOICES = range(8)


# =============================================================================
# >> CLASSES
# =============================================================================
class SimpleESCMenu(_BaseMenu):
    """This class creates basic ESC menus."""

    def __init__(
            self, data=None, select_callback=None, build_callback=None,
            description=None, title=None, title_color=WHITE):
        """Initialize the SimpleESCMenu instance.

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

        @param <description>:
        A description that is displayed under the title.

        @param <title>:
        A title that is displayed at the top of the menu.

        @param <title_color>:
        The color of the title.
        """
        super(SimpleESCMenu, self).__init__(
            data, select_callback, build_callback)
        self.description = description
        self.title = title
        self.title_color = title_color

    def _get_menu_data(self, player_index):
        """Return all relevant menu data as a KeyValues instance.

        @param <player_index>:
        A player index.
        """
        data = KeyValues('menu')
        data.set_string(
            'msg', _translate_text(self.description or '', player_index))

        if self.title is not None:
            data.set_string(
                'title', _translate_text(self.title, player_index))

        data.set_color('color', self.title_color)

        page = self._player_pages[player_index]
        page.options = {}

        # Loop through all options of the current page
        for raw_data in self:
            if isinstance(raw_data, SimpleESCOption):
                page.options[raw_data.choice_index] = raw_data
                button = data.find_key(str(raw_data.choice_index), True)
                button.set_string('msg', raw_data._render(player_index))
                button.set_string('command', '{0} {1}'.format(
                    ESC_SELECTION_CMD, raw_data.choice_index))

        close = SimpleESCOption(0, 'Close')
        button = data.find_key(str(close.choice_index), True)
        button.set_string('msg', close._render(player_index))
        button.set_string('command', '{0} {1}'.format(
            ESC_SELECTION_CMD, close.choice_index))

        # Return the menu data
        return data

    def _select(self, player_index, choice_index):
        """Handle a menu selection.

        @param <player_index>:
        The index of the player who made the selection.

        @param <choice_index>:
        A numeric value that defines what was selected.
        """
        if choice_index == 0:
            return None

        option = self._player_pages[player_index].options[choice_index]
        if not option.selectable:
            return self

        return super(SimpleESCMenu, self)._select(player_index, option)

    def _send(self, player_index):
        """Build and send the menu to the given player via create_message().

        @param <player_index>:
        A player index.
        """
        queue = self.get_user_queue(player_index)
        queue.priority -= 1

        # Build the menu
        data = self._build(player_index)

        # Set priority and display time
        data.set_int('level', queue.priority)
        data.set_int('time', 10)

        # Send the menu
        create_message(
            edict_from_index(player_index),
            DialogType.MENU,
            data
        )

    def _close(self, player_index):
        """Close a menu by overriding it with an empty menu.

        @param <player_index>:
        A player index.
        """
        queue = self.get_user_queue(player_index)
        queue.priority -= 1

        # Unfortunately, this doesn't hide the menu :(
        data = KeyValues('menu')
        data.set_string('title', '')
        data.set_int('level', queue.priority)
        data.set_int('time', 10)
        data.set_string('msg', '')
        create_message(edict_from_index(player_index), DialogType.MENU, data)

    @staticmethod
    def _get_queue_holder():
        """Return the queue for ESC menus."""
        return _esc_queues


class PagedESCMenu(SimpleESCMenu, _PagedMenuBase):
    """Create menus with an unlimited number of options.

    will automatically add navigation options.
    """

    def __init__(
            self, data=None, select_callback=None, build_callback=None,
            description=None, title=None, title_color=WHITE, fill=True):
        """Initialize the PagedESCMenu instance."""
        super(PagedESCMenu, self).__init__(
            data, select_callback, build_callback,
            description, title, title_color)
        self.fill = fill

    @staticmethod
    def _get_max_item_count():
        """Return the maximum possible item count per page."""
        return 5

    def _format_header(self, player_index, page, data):
        """Prepare the header for the menu.

        @param <player_index>:
        A player index.

        @param <page>:
        A _PlayerPage instance.

        @param <data>:
        The current menu data.
        """
        # Create the page info string
        info = '[{0}/{1}]'.format(page.index + 1, self.page_count)

        if self.title is not None:
            data.set_string('title', '{0} {1}'.format(
                _translate_text(self.title, player_index), info))
        else:
            data.set_string('title', info)

        data.set_color('color', self.title_color)

    def _format_body(self, player_index, page, data):
        """Prepare the body for the menu.

        @param <player_index>:
        A player index.

        @param <page>:
        A _PlayerPage instance.

        @param <data>:
        The current menu data.
        """
        # Loop through all options of the current page
        for index, option in enumerate(self._get_options(page.index), 1):
            if isinstance(option, PagedESCOption):
                page.options[index] = option
                button = data.find_key(str(index), True)
                button.set_string('msg', option._render(player_index, index))
                button.set_string(
                    'command', '{0} {1}'.format(ESC_SELECTION_CMD, index))

        # Fill the rest of the menu with empty options
        if self.fill:
            option_num = len(page.options)
            for index in range(5 - option_num):
                index += option_num + 1
                button = data.find_key(str(index), True)
                button.set_string('msg', '')

    @staticmethod
    def _format_footer(player_index, page, data):
        """Prepare the footer for the menu.

        @param <player_index>:
        A player index.

        @param <page>:
        A _PlayerPage instance.

        @param <data>:
        The current menu data.
        """
        # TODO: Add translations
        # Add "Back" option
        button = data.find_key('6', True)
        button.set_string('msg', '6. Back')
        button.set_string('command', '{0} 6'.format(ESC_SELECTION_CMD))

        # Add "Next" option
        button = data.find_key('7', True)
        button.set_string('msg', '7. Next')
        button.set_string('command', '{0} 7'.format(ESC_SELECTION_CMD))

        # Add "Close" option
        button = data.find_key('0', True)
        button.set_string('msg', '0. Close')
        button.set_string('command', '{0} 0'.format(ESC_SELECTION_CMD))

    def _get_menu_data(self, player_index):
        """Return all relevant menu data as a KeyValues instance.

        @param <player_index>:
        A player index.
        """
        data = KeyValues('menu')
        data.set_string(
            'msg', _translate_text(self.description or '', player_index))

        # Get the player's current page
        page = self._player_pages[player_index]
        page.options = {}

        # Format the menu
        self._format_header(player_index, page, data)
        self._format_body(player_index, page, data)
        self._format_footer(player_index, page, data)

        # Return the menu data
        return data

    def _select(self, player_index, choice_index):
        """Handle a menu selection.

        @param <player_index>:
        The index of the player who made the selection.

        @param <choice_index>:
        A numeric value that defines what was selected.
        """
        # Do nothing if the menu is being closed
        if choice_index == 0:
            del self._player_pages[player_index]
            return None

        # Get the player's current page
        page_index = self.get_player_page(player_index)

        # Display previous page?
        if choice_index == 6:
            self.set_player_page(player_index, page_index - 1)
            return self

        # Display next page?
        if choice_index == 7:
            self.set_player_page(player_index, page_index + 1)
            return self

        return super(PagedESCMenu, self)._select(player_index, choice_index)


class SimpleESCOption(_BaseOption):
    """Provides options for SimpleESCMenu objects."""

    def __init__(
            self, choice_index, text, value=None,
            highlight=True, selectable=True):
        """Initialize the option.

        @param <choice_index>:
        The number that is required to select the option.

        @param <text>:
        The text that should be displayed.

        @param <value>:
        The value that should be passed to the menu's selection callback.

        @param <hightlight>:
        Does not work with ESC menus.

        @param <selectable>:
        Does not work with ESC menus.
        """
        super(SimpleESCOption, self).__init__(
            text, value, highlight, selectable)
        self.choice_index = choice_index

    def _render(self, player_index, choice_index=None):
        """Render the data.

        @param <player_index>:
        A player index.

        @param <choice_index>:
        The number should be required to select this item. It depends on the
        menu type if this parameter gets passed.
        """
        return '{0}. {1}'.format(
            self.choice_index, _translate_text(self.text, player_index))


class PagedESCOption(_BaseOption):
    """Display an enumerated option."""

    def _render(self, player_index, choice_index=None):
        """Render the data.

        @param <player_index>:
        A player index.

        @param <choice_index>:
        The number should be required to select this item. It depends on the
        menu type if this parameter gets passed.
        """
        return '{0}. {1}'.format(
            choice_index, _translate_text(self.text, player_index))
