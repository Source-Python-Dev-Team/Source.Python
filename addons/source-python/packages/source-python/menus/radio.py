# ../menus/radio.py

# =============================================================================
# >> IMPORTS
# =============================================================================
# Python Imports
#   Math
import math

# Source.Python Imports
#   Menus
from menus.base import Text
from menus.base import _BaseMenu
from menus.base import _BaseOption
from menus.base import _translate_text
from menus.queue import _radio_queues
#   Messages
from messages import ShowMenu


# =============================================================================
# >> CLASSES
# =============================================================================
class SimpleRadioMenu(_BaseMenu):
    '''
    This class creates a basic radio menu.
    '''

    def _get_menu_data(self, ply_index):
        '''
        This method needs to be implemented by a subclass!

        It should return the required data to send a menu.

        @param <ply_index>:
        A player index.
        '''

        # Always enable 0
        slots = {0}
        choice = 0
        buffer = ''
        page = self._player_pages[ply_index]
        page.options = []
        for raw_data in self:
            # Handle Text objects
            if isinstance(raw_data, Text):
                buffer += raw_data._render(ply_index)

            # Handle RadioOption objects
            elif isinstance(raw_data, RadioOption):
                page.options.append(raw_data)
                choice += 1
                buffer += raw_data._render(ply_index, choice)
                if raw_data.selectable:
                    slots.add(choice)

            # Handle every other object type
            else:
                buffer += str(_translate_text(raw_data, ply_index)) + '\n'

        # Return the menu data
        return dict(
            slots=self._slots_to_bin(slots),
            time=4,  # TODO: Dynamically get the value
            message=buffer[:-1] if buffer else buffer
        )

    def _slots_to_bin(self, slots):
        '''
        Converts an iterable of slots to the binary slot representation.
        '''

        # Keys are enabled in that order: 0987654321
        buffer = list('0000000000')
        for slot in slots:
            if 0 <= slot <= 9:
                buffer[~(slot - 1)] = '1'

        return int(''.join(buffer), 2)

    def _select(self, ply_index, choice):
        '''
        Handles a menu selection.

        @param <ply_index>:
        The index of the player who made the selection.

        @param <choice>:
        A numeric value that defines what was selected.
        '''

        if choice == 0:
            return None

        return super(SimpleRadioMenu, self)._select(
            ply_index, self._player_pages[ply_index].options[choice - 1])

    def _send(self, ply_index):
        '''
        Builds and sends the menu to the given player via ShowMenu.

        @param <ply_index>:
        A player index.
        '''

        ShowMenu(**self._build(ply_index)).send(ply_index)

    def _close(self, ply_index):
        '''
        Closes a menu by overriding it with an empty menu.

        @param <ply_index>:
        A player index.
        '''

        # Send an empty menu
        ShowMenu(
            slots=0b0000000000,
            time=0,
            message='',
        ).send(ply_index)

    @staticmethod
    def _get_queue_holder():
        '''
        Returns the queue for radio menus.
        '''

        return _radio_queues


class PagedRadioMenu(SimpleRadioMenu):
    '''
    This class is used to create menus with an unlimited number of options. It
    will automatically add navigation options.
    '''

    def __init__(
            self, data=[], select_callback=None,
            build_callback=None, description=None,
            title=None, top_seperator='-' * 30, bottom_seperator='-' * 30):
        '''
        Initializes the PagedRadioMenu instance.

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

        @param <top_seperator>:
        A seperator that is displayed right after the title/description.

        @param <bottom_seperator>:
        A seperator that is displayed right after the body.
        '''

        super(PagedRadioMenu, self).__init__(
            data, select_callback, build_callback
        )

        self.title = title
        self.description = description
        self.top_seperator = top_seperator
        self.bottom_seperator = bottom_seperator

    def _format_header(self, ply_index, page, slots):
        '''
        Prepares the header for the menu.

        @param <ply_index>:
        A player index.

        @param <page>:
        A _PlayerPage instance.

        @param <slots>:
        A set to which slots can be added.
        '''

        # Create the page info string
        info = '[{0}/{1}]\n'.format(page.index + 1, self.page_count)
        buffer = (_translate_text(self.title or '', ply_index)).ljust(
            len(self.top_seperator) - len(info)) + info

        # Set description if present
        if self.description is not None:
            buffer += _translate_text(self.description, ply_index) + '\n'

        # Set the top seperator if present
        if self.top_seperator is not None:
            buffer += self.top_seperator + '\n'

        return buffer

    def _format_body(self, ply_index, page, slots):
        '''
        Prepares the body for the menu.

        @param <ply_index>:
        A player index.

        @param <page>:
        A _PlayerPage instance.

        @param <slots>:
        A set to which slots can be added.
        '''

        buffer = ''

        # Get all options for the current page
        options = page.options = self[page.index * 7: (page.index + 1) * 7]

        # Loop through all options of the current page
        for index, option in enumerate(options, 1):
            if not isinstance(option, RadioOption):
                raise TypeError('Expected a RadioOption instance.')

            buffer += option._render(ply_index, index)
            if option.selectable:
                slots.add(index)

        # Fill the rest of the menu
        buffer += ' \n' * (7 - len(options))

        return buffer

    def _format_footer(self, ply_index, page, slots):
        '''
        Prepares the footer for the menu.

        @param <ply_index>:
        A player index.

        @param <page>:
        A _PlayerPage instance.

        @param <slots>:
        A set to which slots can be added.
        '''

        buffer = ''

        # Set the bottom seperator if present
        if self.bottom_seperator is not None:
            buffer += self.bottom_seperator + '\n'

        # TODO: Add translations
        # Add "Back" option
        back_selectable = page.index > 0
        buffer += RadioOption(
            'Back', highlight=back_selectable)._render(ply_index, 8)
        if back_selectable:
            slots.add(8)

        # Add "Next" option
        next_selectable = page.index < self.last_page_index
        buffer += RadioOption(
            'Next', highlight=next_selectable)._render(ply_index, 9)
        if next_selectable:
            slots.add(9)

        # Add "Close" option
        buffer += RadioOption('Close', highlight=False)._render(ply_index, 0)

        # Return the buffer
        return buffer

    def _get_menu_data(self, ply_index):
        '''
        Returns all relevant menu data as a dictionary.

        @param <ply_index>:
        A player index.
        '''

        # Get the player's current page
        page = self._player_pages[ply_index]

        # Always enable slot 0
        slots = {0}

        # Format the menu
        buffer = self._format_header(ply_index, page, slots)
        buffer += self._format_body(ply_index, page, slots)
        buffer += self._format_footer(ply_index, page, slots)

        # Return the menu data
        return dict(
            slots=self._slots_to_bin(slots),
            time=4,  # TODO: Dynamically get the value
            message=buffer[:-1]
        )

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
        if choice == 8:
            self.set_player_page(ply_index, page.index - 1)
            return self

        # Display next page?
        if choice == 9:
            self.set_player_page(ply_index, page.index + 1)
            return self

        return super(PagedRadioMenu, self)._select(ply_index, choice)

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

        return int(math.ceil(len(self) / 7.0)) or 1

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


class RadioOption(_BaseOption):
    '''
    This class is used to display an enumerated option.
    '''

    def _get_highlight_prefix(self):
        '''
        Returns the hightlight prefix if <highlight> was set. Else it will
        return an empty string.
        '''

        return '->' if self.highlight else ''

    def _render(self, ply_index, choice):
        '''
        Renders the data.

        @param <ply_index>:
        A player index.

        @param <choice>:
        A numeric value that defines the selection number.
        '''

        return '{0}{1}. {2}\n'.format(
            self._get_highlight_prefix(),
            choice,
            _translate_text(self.text, ply_index)
        )
