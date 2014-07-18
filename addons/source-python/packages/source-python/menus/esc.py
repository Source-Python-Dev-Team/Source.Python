# ../menus/esc.py

# =============================================================================
# >> IMPORTS
# =============================================================================
# Python Imports
#   Math
import math

# Source.Python Imports
#   Colors
from colors import White
#   Keyvalues
from keyvalues import KeyValues
#   Menus
from menus.base import _BaseMenu
from menus.base import _BaseOption
from menus.base import _translate_text
from menus.queue import ESC_SELECTION_CMD
from menus.queue import _esc_queues
#   Messages
from _messages import DialogType
from _messages import create_message
#   Players
from players.helpers import edict_from_index


# =============================================================================
# >> CLASSES
# =============================================================================
class PagedESCMenu(_BaseMenu):
    '''
    This class is used to create menus with an unlimited number of options. It
    will automatically add navigation options.
    '''

    def __init__(
            self, data=[], select_callback=None, build_callback=None,
            description=None, title=None, title_color=White):
        '''
        Initializes the PagedESCMenu instance.

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
        '''

        super(PagedESCMenu, self).__init__(
            data, select_callback, build_callback)
        self.description = description
        self.title = title
        self.title_color = title_color

    def _format_header(self, ply_index, page, data):
        '''
        Prepares the header for the menu.

        @param <ply_index>:
        A player index.

        @param <page>:
        A _PlayerPage instance.

        @param <data>:
        The current menu data.
        '''

        # Create the page info string
        info = '[{0}/{1}]'.format(page.index+1, self.page_count)

        if self.title:
            data.set_string('title', '{0} {1}'.format(
                _translate_text(self.title, ply_index), info))
        else:
            data.set_string('title', info)

        data.set_color('color', self.title_color)

    def _format_body(self, ply_index, page, data):
        '''
        Prepares the body for the menu.

        @param <ply_index>:
        A player index.

        @param <page>:
        A _PlayerPage instance.

        @param <data>:
        The current menu data.
        '''

        # Get all options of the current page
        page.options = tuple(self.options)[page.index*5:(page.index+1)*5]

        # Loop through all options of the current page
        for index, option in enumerate(page.options):
            index += 1
            button = data.find_key(str(index), True)
            button.set_string('msg', option._render(ply_index, index))
            button.set_string(
                'command', '{0} {1}'.format(ESC_SELECTION_CMD, index))

        # Fill the rest of the menu with empty options
        option_num = len(page.options)
        for index in range(5-option_num):
            index += option_num + 1
            button = data.find_key(str(index), True)
            button.set_string('msg', '')
            button.set_string(
                'command', '{0} {1}'.format(ESC_SELECTION_CMD, index))

    def _format_footer(self, ply_index, page, data):
        '''
        Prepares the footer for the menu.

        @param <ply_index>:
        A player index.

        @param <page>:
        A _PlayerPage instance.

        @param <data>:
        The current menu data.
        '''

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

    def _get_menu_data(self, ply_index):
        '''
        Returns all relevant menu data as a KeyValues instance.

        @param <ply_index>:
        A player index.
        '''

        data = KeyValues('menu')
        data.set_string(
            'msg', _translate_text(self.description or '', ply_index))

        # Get the player's current page
        page = self._player_pages[ply_index]

        # Format the menu
        self._format_header(ply_index, page, data)
        self._format_body(ply_index, page, data)
        self._format_footer(ply_index, page, data)

        # Return the menu data
        return data

    def _select(self, ply_index, choice):
        '''
        Handles a menu selection.

        @param <ply_index>:
        The index of the player who made the selection.

        @param <choice>:
        A numeric value that defines what was selected.
        '''

        # Do nothing if the menu is being closed
        if choice == 0:
            del self._player_pages[ply_index]
            return None

        # Get the player's current page
        page = self._player_pages[ply_index]

        # Display previous page?
        if choice == 6:
            self.set_player_page(ply_index, page.index - 1)
            return self

        # Display next page?
        if choice == 7:
            self.set_player_page(ply_index, page.index + 1)
            return self

        # Prevent an IndexError
        if choice > len(page.options):
            return self

        option = page.options[choice-1]
        if not option.selectable:
            return self

        return super(PagedESCMenu, self)._select(ply_index, option)

    def _send(self, ply_index):
        '''
        Builds and sends the menu to the given player via create_message().

        @param <ply_index>:
        A player index.
        '''

        queue = self.get_user_queue(ply_index)
        queue.priority -= 1

        # Build the menu
        data = self._build(ply_index)

        # Set priority and display time
        data.set_int('level', queue.priority)
        data.set_int('time', 10)

        # Send the menu
        create_message(
            edict_from_index(ply_index),
            DialogType.MENU,
            data
        )

    def _close(self, ply_index):
        '''
        Closes a menu by overriding it with an empty menu.

        @param <ply_index>:
        A player index.
        '''

        queue = self.get_user_queue(ply_index)
        queue.priority -= 1

        # Unfortunately, this doesn't hide the menu :(
        data = KeyValues('menu')
        data.set_string('title', '')
        data.set_int('level', queue.priority)
        data.set_int('time', 10)
        data.set_string('msg', '')
        create_message(edict_from_index(ply_index), DialogType.MENU, data)

    @staticmethod
    def _get_queue_holder():
        '''
        Returns the queue for ESC menus.
        '''

        return _esc_queues

    @property
    def last_page_index(self):
        '''
        Returns the last page index.
        '''

        return self.page_count - 1

    @property
    def page_count(self):
        '''
        Returns the number of pages the menu currently has.
        '''

        return int(math.ceil(len(tuple(self.options)) / 5.0)) or 1

    @property
    def options(self):
        '''
        Returns all ESCOption objects in the correct order.
        '''

        return filter(lambda op: isinstance(op, ESCOption), self)

    def set_player_page(self, ply_index, page_index):
        '''
        Sets the player's current page index.

        @param <ply_index>:
        A player index.

        @param <page_index>:
        An index that defines the current active page.

        If <page_index> is lower than 0, the page index will be set to 0.

        If <page_index> is greater than the last page index, it will be set to
        the last page index.
        '''

        page = self._player_pages[ply_index]
        if page_index < 0:
            page.index = 0
        elif page_index > self.last_page_index:
            page.index = self.last_page_index
        else:
            page.index = page_index

    def get_player_page(self, ply_index):
        '''
        Returns the current player page index.

        @param <ply_index>:
        A player index.
        '''

        return self._player_pages[ply_index].index


class ESCOption(_BaseOption):
    '''
    This class is used to display an enumerated option.
    '''

    def _render(self, ply_index, choice):
        '''
        Renders the data.

        @param <ply_index>:
        A player index.

        @param <choice>:
        A numeric value that defines the selection number.
        '''

        return '{0}. {1}'.format(choice, _translate_text(self.text, ply_index))
