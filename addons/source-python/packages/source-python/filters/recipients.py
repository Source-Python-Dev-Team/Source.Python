# ../filters/recipients.py

"""Provides Recipient Filtering functionality."""

# ============================================================================
# >> IMPORTS
# ============================================================================
# Source.Python Imports
#   Filters
from _filters._recipients import _RecipientFilter
from _filters._recipients import BaseRecipientFilter
#   Players
from players.entity import Player


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('RecipientFilter',
           'BaseRecipientFilter',
           )


# ============================================================================
# >> CLASSES
# ============================================================================
class RecipientFilter(_RecipientFilter):
    """Class used to improve the ease of use of a _RecipientFilter instance."""

    def __init__(self, *filters):
        """Initialize and update the recipient filter."""
        # Initialize the _RecipientFilter
        super().__init__()

        # Store the given filters
        self.filters = filters

        # Update the recipient matching the given filters
        self.update(*self.filters)

    def __getitem__(self, item):
        """Return the index at the given recipient slot."""
        # Slicing?
        if isinstance(item, slice):

            # Return a mapping matching the given slice
            return list(map(
                self.get_recipient_index, range(*item.indices(len(self)))))

        # Return the index at the given recipient slot
        return self.get_recipient_index[item]

    def __iter__(self):
        """Iterate over the recipient filter."""
        # Loop through each index in the filter
        for index in range(len(self)):

            # Yield the recipient
            yield self.get_recipient_index(index)

    def __repr__(self):
        """Return a readable representation of the recipient filter."""
        return '({0})'.format(', '.join(map(str, self)))

    def merge(self, iterable):
        """Merge the given recipient."""
        # Loop through all indexes of the given recipient
        for index in iterable:

            # Get the index if a Player object was passed
            # This is also a fix for PlayerIter now
            #   only iterating over Player instances.
            if isinstance(index, Player):
                index = index.index

            # Add the current index to the recipient filter
            self.add_recipient(index)

    def update(self, *args, clear=True):
        """Update the recipient filter matching the given filters."""
        # Clear the recipient first?
        if clear:

            # Remove all players from the recipient filter
            self.remove_all_players()

        # No given filters?
        if not args:

            # Use the filters we got on initialization
            args = self.filters

        # Still no filters?
        if not args:

            # Add all players to the recipient
            self.add_all_players()

        # Otherwise
        else:

            # Loop through all filters
            for filter_ in args:

                # Is the current filter an index?
                if isinstance(filter_, int):

                    # Add the current filter as an index
                    self.add_recipient(filter_)

                # Otherwise, is the current filter a Player instance?
                elif isinstance(filter_, Player):

                    # Add the player to the recipient filter...
                    self.add_recipient(filter_.index)

                # Otherwise
                else:

                    # Assume it is an iterable and merge it to the recipient
                    #   filter
                    self.merge(filter_)
