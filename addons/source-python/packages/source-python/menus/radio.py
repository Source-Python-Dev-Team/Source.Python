# ../menus/radio.py

"""Provides ShowMenu based functionality."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
#   Core
from core import SOURCE_ENGINE
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
__all__ = ('ListRadioMenu',
           'ListRadioOption',
           'PagedRadioMenu',
           'PagedRadioOption',
           'SimpleRadioMenu',
           'SimpleRadioOption',
           )


# =============================================================================
# >> CONSTANTS
# =============================================================================
if SOURCE_ENGINE in ('csgo', 'bms'):
    BUTTON_BACK = 7
    BUTTON_NEXT = 8
    BUTTON_CLOSE = 9
    BUTTON_CLOSE_SLOT = BUTTON_CLOSE
    MAX_ITEM_COUNT = 6
    VALID_CHOICES = range(1, 10)
else:
    BUTTON_BACK = 8
    BUTTON_NEXT = 9
    BUTTON_CLOSE = 10
    BUTTON_CLOSE_SLOT = 0
    MAX_ITEM_COUNT = 7
    VALID_CHOICES = range(1, 11)


# =============================================================================
# >> CLASSES
# =============================================================================
class SimpleRadioMenu(_BaseMenu):
    """This class creates a basic radio menu."""

    def _get_menu_data(self, player_index):
        """Return the menu string, enabled slots, and time to show the menu.

        :param int player_index: See
            :meth:`menus.base._BaseMenu._get_menu_data`.
        """
        # Always enable BUTTON_CLOSE_SLOT
        slots = {BUTTON_CLOSE_SLOT}
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
        return (buffer[:-1] if buffer else '', self._slots_to_bin(slots), 1)

    @staticmethod
    def _slots_to_bin(slots):
        """Convert an iterable of slots to the binary slot representation.

        :param iterable slots: Slots that should be enabled.
        :raise ValueError: Raised if a slot is out of range.
        """
        # Keys are enabled in that order: 0987654321
        buffer = list('0000000000')
        for slot in slots:
            if 0 <= slot <= 9:
                buffer[~(slot - 1)] = '1'
            else:
                raise ValueError('Slot out of range: {}'.format(slot))

        return int(''.join(buffer), 2)

    def _select(self, player_index, choice_index):
        """See :meth:`menus.base._BaseMenu._select`."""
        if choice_index == BUTTON_CLOSE:
            return self._select_close(player_index)

        return super()._select(
            player_index,
            self._player_pages[player_index].options[choice_index])

    def _send(self, player_index):
        """Build and sends the menu to the given player via ShowMenu.

        :param int player_index: See :meth:`menus.base._BaseMenu._send`.
        """
        ShowMenu(*self._build(player_index)).send(player_index)

    @staticmethod
    def _close(player_index):
        """See :meth:`menus.base._BaseMenu._close`."""
        # Send an empty menu
        ShowMenu('', 0b0000000000, 0).send(player_index)

    @staticmethod
    def _get_queue_holder():
        """Return the queue for radio menus."""
        return _radio_queues


class PagedRadioMenu(SimpleRadioMenu, _PagedMenuBase):
    """Create menus with an unlimited number of options.

    Navigation options will be added automatically.
    """

    def __init__(
            self, data=None, select_callback=None,
            build_callback=None, close_callback=None, description=None,
            title=None, top_separator='-' * 30, bottom_separator='-' * 30,
            fill=True, parent_menu=None):
        """Initialize the object.

        :param iterable|None data: See :meth:`menus.base._BaseMenu.__init__`.
        :param callable|None select_callback: See
            :meth:`menus.base._BaseMenu.__init__`.
        :param callable|None build_callback: See
            :meth:`menus.base._BaseMenu.__init__`.
        :param callable|None close_callback: See
            :meth:`menus.base._BaseMenu.__init__`.
        :param str|None description: A description that is displayed under the
            title.
        :param str|None title: A title that is displayed at the top of the
            menu.
        :param str top_separator: A separator that is displayed right after
            the title/description.
        :param str bottom_separator: A separator that is displayed right after
            the body.
        :param bool fill: If True the menu will be filled so that it will
            always have the same  size.
        :param _BaseMenu parent_menu: A menu that will be displayed when
            hitting 'Back' on the first page.
        """
        super().__init__(data, select_callback, build_callback, close_callback)

        self.title = title
        self.description = description
        self.top_separator = top_separator
        self.bottom_separator = bottom_separator
        self.fill = fill
        self.parent_menu = parent_menu

    @staticmethod
    def _get_max_item_count():
        """Return the maximum possible item count per page."""
        return MAX_ITEM_COUNT

    def _format_header(self, player_index, page, slots):
        """Prepare the header for the menu.

        :param int player_index: A player index.
        :param _PlayerPage page: The player's current page.
        :param slots: A set to which slots can be added.
        :type slots: :class:`set`
        """
        # Create the page info string
        info = '[{0}/{1}]\n'.format(page.index + 1, self.page_count)

        buffer = '{0} {1}'.format(_translate_text(
            self.title, player_index), info) if self.title else info

        # Set description if present
        if self.description is not None:
            buffer += _translate_text(self.description, player_index) + '\n'

        # Set the top separator if present
        if self.top_separator is not None:
            buffer += self.top_separator + '\n'

        return buffer

    def _format_body(self, player_index, page, slots):
        """Prepare the body for the menu.

        :param int player_index: A player index.
        :param _PlayerPage page: The player's current page.
        :param slots: A set to which slots can be added.
        :type slots: :class:`set`
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
            buffer += ' \n' * (self._get_max_item_count() - len(options))

        return buffer

    def _format_footer(self, player_index, page, slots):
        """Prepare the footer for the menu.

        :param int player_index: A player index.
        :param _PlayerPage page: The player's current page.
        :param slots: A set to which slots can be added.
        :type slots: :class:`set`
        """
        buffer = ''

        # Set the bottom separator if present
        if self.bottom_separator is not None:
            buffer += self.bottom_separator + '\n'

        # TODO: Add translations
        # Add "Back" option
        back_selectable = page.index > 0 or self.parent_menu is not None
        buffer += PagedRadioOption(
            'Back', highlight=back_selectable)._render(
                player_index, BUTTON_BACK)
        if back_selectable:
            slots.add(BUTTON_BACK)

        # Add "Next" option
        next_selectable = page.index < self.last_page_index
        buffer += PagedRadioOption(
            'Next', highlight=next_selectable)._render(
                player_index, BUTTON_NEXT)
        if next_selectable:
            slots.add(BUTTON_NEXT)

        # Add "Close" option
        buffer += PagedRadioOption(
            'Close', highlight=False)._render(player_index, BUTTON_CLOSE_SLOT)

        # Return the buffer
        return buffer

    def _get_menu_data(self, player_index):
        """Return all relevant menu data as a dictionary.

        :param int player_index: A player index.
        """
        # Get the player's current page
        page = self._player_pages[player_index]

        # Always enable BUTTON_CLOSE_SLOT
        slots = {BUTTON_CLOSE_SLOT}

        # Format the menu
        buffer = self._format_header(player_index, page, slots)
        buffer += self._format_body(player_index, page, slots)
        buffer += self._format_footer(player_index, page, slots)

        # Return the menu data
        return (buffer[:-1] if buffer else '', self._slots_to_bin(slots), 1)

    def _select(self, player_index, choice_index):
        """See :meth:`menus.base._BaseMenu._select`."""
        if choice_index == BUTTON_CLOSE:
            del self._player_pages[player_index]
            return self._select_close(player_index)

        # Get the player's current page
        page = self._player_pages[player_index]

        # Display previous page?
        if choice_index == BUTTON_BACK:
            # Is the player on the first page, and do we have a parent menu?
            if not page.index and self.parent_menu is not None:
                return self.parent_menu

            self.set_player_page(player_index, page.index - 1)
            return self

        # Display next page?
        if choice_index == BUTTON_NEXT:
            self.set_player_page(player_index, page.index + 1)
            return self

        return super()._select(player_index, choice_index)


class ListRadioMenu(PagedRadioMenu):
    """Creates a list-like radio menu.

    Navigation options are added automatically."""

    def __init__(
            self, data=None, select_callback=None, build_callback=None, close_callback=None,
            description=None, title=None, top_separator='-' * 30,
            bottom_separator='-' * 30, fill=True, parent_menu=None,
            items_per_page=10):
        """Initialize the object.

        :param iterable|None data: See :meth:`menus.base._BaseMenu.__init__`.
        :param callable|None select_callback: See
            :meth:`menus.base._BaseMenu.__init__`.
        :param callable|None build_callback: See
            :meth:`menus.base._BaseMenu.__init__`.
        :param callable|None close_callback: See
            :meth:`menus.base._BaseMenu.__init__`.
        :param str|None description: See :meth:`PagedRadioMenu.__init__`.
        :param str|None title: See :meth:`PagedRadioMenu.__init__`.
        :param str top_separator: See :meth:`PagedRadioMenu.__init__`.
        :param str bottom_separator: See :meth:`PagedRadioMenu.__init__`.
        :param bool fill: See :meth:`PagedRadioMenu.__init__`.
        :param _BaseMenu parent_menu: See :meth:`PagedRadioMenu.__init__`.
        :param int items_per_page: Number of options that should be displayed
            on a single page.
        """
        super().__init__(data, select_callback, build_callback, close_callback, description,
            title, top_separator, bottom_separator, fill, parent_menu)
        self.items_per_page = items_per_page

    def _get_max_item_count(self):
        return self.items_per_page


class _BaseRadioOption(_BaseOption):
    """Base class for radio options."""

    def _get_highlight_prefix(self):
        """Return the hightlight prefix if ``highlight`` was set.

        Else it will return an empty string.
        """
        return '->' if self.highlight else ''


class SimpleRadioOption(_BaseRadioOption):
    """Provides options for :class:`SimpleRadioMenu` objects."""

    def __init__(
            self, choice_index, text, value=None,
            highlight=True, selectable=True):
        """Initialize the option.

        :param int choice_index: The number that is required to select the
            option.
        :param str text: See :meth:`menus.base._BaseOption.__init__`.
        :param value: See :meth:`menus.base._BaseOption.__init__`.
        :param bool hightlight: If True the text will be highlighted.
        :param bool selectable: If True the option will be selectable.
        """
        super().__init__(text, value, highlight, selectable)
        self.choice_index = choice_index

    def _render(self, player_index, choice_index=None):
        """See :meth:`menus.base._MenuData._render`."""
        return '{0}{1}. {2}\n'.format(
            self._get_highlight_prefix(),
            self.choice_index,
            _translate_text(self.text, player_index)
        )


class PagedRadioOption(_BaseRadioOption):
    """Provides options for :class:`PagedRadioMenu` objects."""

    def _render(self, player_index, choice_index):
        """See :meth:`menus.base._MenuData._render`."""
        return '{0}{1}. {2}\n'.format(
            self._get_highlight_prefix(),
            choice_index,
            _translate_text(self.text, player_index)
        )


class ListRadioOption(PagedRadioOption):
    """Provides options for :class:`ListRadioMenu` objects."""

    def __init__(self, text, highlight=True, enumerated=True):
        """Initialize the option.

        :param str text: See :meth:`menus.base._BaseOption.__init__`.
        :param bool hightlight: If True the text will be highlighted.
        :param bool enumerated: If True the number of the option will be added
            in front of the text.

        .. note:: ``highlight`` only works if ``enumerated`` is set to True.
        """
        super().__init__(text, None, highlight, False)
        self.enumerated = enumerated

    def _render(self, player_index, choice_index):
        """See :meth:`menus.base._MenuData._render`."""
        if self.enumerated:
            return super()._render(player_index, choice_index)

        return '{}{}\n'.format(self._get_highlight_prefix(),
            _translate_text(self.text, player_index))
