# ../plugins/info.py

"""Provides plugin information storing."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Python Imports
#   Collections
from collections import OrderedDict


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('PluginInfo',
           )


# =============================================================================
# >> CLASSES
# =============================================================================
class PluginInfo(OrderedDict):

    """Stores information for a plugin."""

    def __getattr__(self, attribute):
        """Redirect to __getitem__."""
        # Is the attribute private?
        if attribute.startswith('_'):

            # Raise an error
            # This is done to fix an issue with OrderedDict.__init__
            raise AttributeError('Private attributes not allowed')

        # Redirect to __getitem__
        return self[attribute]

    def __setattr__(self, attribute, value):
        """Redirect to __setitem__."""
        # Is the attribute private?
        if attribute.startswith('_'):

            # Re-call __setattr__
            # This is done to fix an issue with OrderedDict.__init__
            super(PluginInfo, self).__setattr__(attribute, value)

            # No need to go further
            return

        # Redirect to __setitem__
        self[attribute] = value
