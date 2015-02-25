# ../menus/radio.py

"""Provides ShowMenu based functionality."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
#   Menus
from menus.base import Text
from menus.base import _BaseMenu
from menus.base import _PagedMenuBase
from menus.base import _BaseOption
from menus.base import _translate_text
from menus.queue import _radio_queues
#   Messages
from messages import ShowMenu


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('PagedRadioMenu',
           'PagedRadioOption',
           'SimpleRadioMenu',
           'SimpleRadioOption',
           )


# =============================================================================
# >> CLASSES
# =============================================================================
class SimpleRadioMenu(_BaseMenu):

    """This class creates a basic radio menu."""

    def _get_menu_data(self, player_index):
        """Return the required data to send a menu.

        This method needs to be implemented by a subclass!

        @param <player_index>:
        A player index.
        """
        # Always enable 0
        slots = {0}
        buffer = ''
        page = self._player_pages[player_index]
        page.options = {}
        for raw_data in self:
            # Handle Text objects
            if isinstance(raw_data, Text):
                buffer += raw_data._render(player_index)

            # Handle _BaseOption objects
            elif isinstance(raw_data, SimpleRadioOption):
                buffer += raw_data._render(player_index)
                if raw_data.selectable:
                    slots.add(raw_data.choice_index)
                    page.options[raw_data.choice_index] = raw_data

            # Handle every other object type as a text
            else:
                buffer += Text(raw_data)._render(player_index)

        # Return the menu data
        return dict(
            slots=self._slots_to_bin(slots),
            time=4,
            message=buffer[:-1] if buffer else ''
        )

    def _slots_to_bin(self, slots):
        """Convert an iterable of slots to the binary slot representation."""
        # Keys are enabled in that order: 0987654321
        buffer = list('0000000000')
        for slot in slots:
            if 0 <= slot <= 9:
                buffer[~(slot - 1)] = '1'

        return int(''.join(buffer), 2)

    def _select(self, player_index, choice_index):
        """Handle a menu selection.

        @param <player_index>:
        The index of the player who made the selection.

        @param <choice_index>:
        A numeric value that defines what was selected.
        """
        if choice_index == 0:
            return None

        return super(SimpleRadioMenu, self)._select(
            player_index,
            self._player_pages[player_index].options[choice_index])

    def _send(self, player_index):
        """Build and sends the menu to the given player via ShowMenu.

        @param <player_index>:
        A player index.
        """
        ShowMenu(**self._build(player_index)).send(player_index)

    def _close(self, player_index):
        """
        Close a menu by overriding it with an empty menu.

        @param <player_index>:
        A player index.
        """
        # Send an empty menu
        ShowMenu(
            slots=0b0000000000,
            time=0,
            message='',
        ).send(player_index)

    @staticmethod
    def _get_queue_holder():
        """Return the queue for radio menus."""
        return _radio_queues


class PagedRadioMenu(SimpleRadioMenu, _PagedMenuBase):

    """Create menus with an unlimited number of options.

    Navigation options will automatically be added.
    """

    def __init__(
            self, data=None, select_callback=None,
            build_callback=None, description=None,
            title=None, top_seperator='-' * 30, bottom_seperator='-' * 30,
            fill=True):
        """Initialize the PagedRadioMenu instance.

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

        @param <fill>:
        If True the menu will be filled so that it will always have the same
        size.
        """
        super(PagedRadioMenu, self).__init__(
            data, select_callback, build_callback
        )

        self.title = title
        self.description = description
        self.top_seperator = top_seperator
        self.bottom_seperator = bottom_seperator
        self.fill = fill

    def _get_max_item_count(self):
        """Return the maximum possible item count per page."""
        return 7

    def _format_header(self, player_index, page, slots):
        """Prepare the header for the menu.

        @param <player_index>:
        A player index.

        @param <page>:
        A _PlayerPage instance.

        @param <slots>:
        A set to which slots can be added.
        """
        # Create the page info string
        info = '[{0}/{1}]\n'.format(page.index + 1, self.page_count)

        buffer = '{0} {1}'.format(_translate_text(
            self.title, player_index), info) if self.title else info

        # Set description if present
        if self.description is not None:
            buffer += _translate_text(self.description, player_index) + '\n'

        # Set the top seperator if present
        if self.top_seperator is not None:
            buffer += self.top_seperator + '\n'

        return buffer

    def _format_body(self, player_index, page, slots):
        """Prepare the body for the menu.

        @param <player_index>:
        A player index.

        @param <page>:
        A _PlayerPage instance.

        @param <slots>:
        A set to which slots can be added.
        """
        buffer = ''

        # Get all options for the current page
        options = tuple(enumerate(self._get_options(page.index), 1))
        page.options = dict(options)

        # Loop through all options of the current page
        for choice_index, option in options:
            if isinstance(option, PagedRadioOption):
                buffer += option._render(player_index, choice_index)
                if option.selectable:
                    slots.add(choice_index)
            elif isinstance(option, Text):
                buffer += option._render(player_index, choice_index)
            else:
                buffer += Text(option)._render(player_index, choice_index)

        # Fill the rest of the menu
        if self.fill:
            buffer += ' \n' * (7 - len(options))

        return buffer

    def _format_footer(self, player_index, page, slots):
        """Prepare the footer for the menu.

        @param <player_index>:
        A player index.

        @param <page>:
        A _PlayerPage instance.

        @param <slots>:
        A set to which slots can be added.
        """
        buffer = ''

        # Set the bottom seperator if present
        if self.bottom_seperator is not None:
            buffer += self.bottom_seperator + '\n'

        # TODO: Add translations
        # Add "Back" option
        back_selectable = page.index > 0
        buffer += PagedRadioOption(
            'Back', highlight=back_selectable)._render(player_index, 8)
        if back_selectable:
            slots.add(8)

        # Add "Next" option
        next_selectable = page.index < self.last_page_index
        buffer += PagedRadioOption(
            'Next', highlight=next_selectable)._render(player_index, 9)
        if next_selectable:
            slots.add(9)

        # Add "Close" option
        buffer += PagedRadioOption(
            'Close', highlight=False)._render(player_index, 0)

        # Return the buffer
        return buffer

    def _get_menu_data(self, player_index):
        """Return all relevant menu data as a dictionary.

        @param <player_index>:
        A player index.
        """
        # Get the player's current page
        page = self._player_pages[player_index]

        # Always enable slot 0
        slots = {0}

        # Format the menu
        buffer = self._format_header(player_index, page, slots)
        buffer += self._format_body(player_index, page, slots)
        buffer += self._format_footer(player_index, page, slots)

        # Return the menu data
        return dict(
            slots=self._slots_to_bin(slots),
            time=4,
            message=buffer[:-1] if buffer else ''
        )

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
        page = self._player_pages[player_index]

        # Display previous page?
        if choice_index == 8:
            self.set_player_page(player_index, page.index - 1)
            return self

        # Display next page?
        if choice_index == 9:
            self.set_player_page(player_index, page.index + 1)
            return self

        return super(PagedRadioMenu, self)._select(player_index, choice_index)


class _BaseRadioOption(_BaseOption):

    """Base class for radio options."""

    def _get_highlight_prefix(self):
        """Return the hightlight prefix if <highlight> was set.

        Else it will return an empty string.
        """
        return '->' if self.highlight else ''


class SimpleRadioOption(_BaseRadioOption):

    """Provides options for SimpleRadioMenu objects."""

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
        Set this to true if the text should be hightlighted.

        @param <selectable>:
        Set this to True if the option should be selectable.
        """
        super(SimpleRadioOption, self).__init__(
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
        return '{0}{1}. {2}\n'.format(
            self._get_highlight_prefix(),
            self.choice_index,
            _translate_text(self.text, player_index)
        )


class PagedRadioOption(_BaseRadioOption):

    """Provides options for PagedRadioMenu objects."""

    def _render(self, player_index, choice_index):
        """Render the data.

        @param <player_index>:
        A player index.

        @param <choice_index>:
        The number should be required to select this item. It depends on the
        menu type if this parameter gets passed.
        """
        return '{0}{1}. {2}\n'.format(
            self._get_highlight_prefix(),
            choice_index,
            _translate_text(self.text, player_index)
        )
