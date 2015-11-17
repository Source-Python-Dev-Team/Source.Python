# ../core/table.py

"""Provides an object oriented way to print out tables."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Python Imports
#   Enum
from enum import Enum
#   Itertools
from itertools import zip_longest


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('Alignment',
           'AsciiTable',
           'Column',
           'Item',
           )


# =============================================================================
# >> CLASSES
# =============================================================================
class Alignment(Enum):
    """Contains constants to specify the aligment of table items."""

    LEFT = str.ljust
    RIGHT = str.rjust
    CENTER = str.center


class Column(list):
    """Represents a column of a table."""

    def __init__(
            self, name, alignment=Alignment.CENTER,
            left_padding=2, right_padding=2):
        """Intialize the column.

        :param str name: Name of the column.
        :param int alignment: The alignment of the column name.
        :param int left_padding: Number of spaces for left padding.
        :param int right_padding: Number of spaces for right padding.
        """
        super().__init__()
        self.name = Item(name, alignment)
        self.left_padding = left_padding
        self.right_padding = right_padding

    def _get_max_padding(self):
        """Return the length of the longest item in the column.

        The name of the column is used to determine the length, as well.
        """
        length = len(self.name)
        if not self:
            return length

        return max(len(max(self, key=len)), length)

    def _format(self):
        """Return the name of the column and all items of the column.

        All items (including the name) will be
        formatted properly before being returned.
        """
        padding = self._get_max_padding()
        return (self.name._format(
            padding, self.left_padding, self.right_padding), tuple(
                item._format(padding, self.left_padding, self.right_padding)
                for item in self))


class Item(object):
    """Represents a value/item of a column."""

    def __init__(self, value, alignment=Alignment.LEFT):
        """Initialize the item.

        :param value: The value this item should have.
        :param int aligment: The aligment of the item in the table.
        """
        self.value = str(value)
        self.alignment = alignment

    def __len__(self):
        """Return the length of the value."""
        return len(self.value)

    def _format(self, padding, left_padding, right_padding):
        """Format the item.

        :param int padding: The length of the longest item in the column.
        :param int left_padding: A number that defines how many spaces should
            be added to the left of the item.
        :param int right_padding: A number that defines how many spaces should
            be added to the right of the item.
        """
        return '{0}{1}{2}'.format(
            ' ' * left_padding, self.alignment(self.value, padding),
            ' ' * right_padding)


class AsciiTable(object):
    """Represents a table."""

    def __init__(self, *columns):
        """Add the given objects as columns to the table.

        If a given column is not a Column object, it will be created.
        """
        if len(columns) == 0:
            raise ValueError('Table must have at least one column.')

        self._columns = []
        for column in columns:
            if not isinstance(column, Column):
                column = Column(column)

            self._columns.append(column)

    def __len__(self):
        """Return the number of columns."""
        return len(self._columns)

    def __iter__(self):
        """Return an iterator for all columns."""
        return iter(self._columns)

    def __getitem__(self, index):
        """Return the column at the given index."""
        return self._columns[index]

    def add_row(self, *items):
        """Append the given items to the table's columns."""
        if len(items) != len(self):
            raise ValueError(
                'You must add exactly the same number of items' +
                ' like the number of columns.')

        for index, item in enumerate(items):
            if not isinstance(item, Item):
                item = Item(item)

            self[index].append(item)

    def format(self, header_separator='=', column_separator='|'):
        """Format the table and returns an ASCII table string.

        :param str header_separator: A single character that defines the
            character that should be used to create the horizontal separator.
        :param str column_separator: A single character that defines the
            character that should be used to create the vertical separators.
        """
        if not isinstance(header_separator, str) or len(header_separator) != 1:
            raise ValueError('Header separator must be a single character.')

        columns = []
        rows = []
        for column in self:
            column, row = column._format()
            columns.append(column)
            rows.append(row)

        header = column_separator.join(columns)
        return '{0}\n{1}\n{2}'.format(
            header,
            header_separator * len(header),
            '\n'.join(column_separator.join(row) for row in zip_longest(
                *rows, fillvalue='')))
