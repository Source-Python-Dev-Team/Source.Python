# ../config/cvar.py

"""Provides ConVar functionality in configuration files."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
#   Cvars
from cvars import ConVar
#   Translations
from translations.strings import TranslationStrings


# =============================================================================
# >> CLASSES
# =============================================================================
class _CvarManager(dict):

    """Class used to store a cvar instance."""

    def __init__(
            self, name, default, flags, description, min_value, max_value):
        """Called on instantiation."""
        # Initialize the dictionary
        super(_CvarManager, self).__init__()

        # Is the given description a TranslationStrings instance?
        if isinstance(description, TranslationStrings):

            # Store the description as the proper language string
            description = description.get_string()

        # Store the base attributes for the cvar
        self._name = name
        self._default = default
        self._description = description

        # Get the cvar instance
        self._cvar = ConVar(
            name, str(default), flags, description, min_value, max_value)

        # Set the attribute to show the default value
        self.show_default = True

        # Store a list to iterate over description fields and text
        self._order = list()

    def __getattr__(self, attr):
        """Return cvar attributes or items in the instance."""
        # Does the cvar instance have the given attribute?
        if hasattr(self.cvar, attr):

            # Return the attribute for the cvar instance
            return getattr(self.cvar, attr)

        # Return the item
        return self.__getitem__(attr)

    def __missing__(self, item):
        """Create the item as a _ListManager instance."""
        # Get the _ListManager instance for the given item
        value = self[item] = _ListManager(item)

        # Add the _ListManager instance to the ordered list
        self._order.append(value)

        # Return the _ListManager instance
        return value

    def __iter__(self):
        """Iterate over items in the ordered list."""
        # Loop through items in the ordered list
        for item in self._order:

            # Is the current item a _ListManager instance?
            if isinstance(item, _ListManager):

                # Yield the item's name
                yield item.name, 0

                # Loop through each line in the _ListManager instance
                for line in item:

                    # Yield the line
                    yield item.start + line, item.indent

            # Is the current item not a _ListManager instance?
            else:

                # Yield the item
                yield item, 0

    def text(self, text):
        """Add simple text or a TranslationStrings instance to the list."""
        # Is the item a TranslationStrings instance?
        if isinstance(text, TranslationStrings):

            # Get the proper text for the given item
            text = text.get_string()

        # Add the text to the ordered list
        self._order.append(text)

    @property
    def name(self):
        """Return the cvar's name."""
        return self._name

    @property
    def default(self):
        """Return the cvar's default value."""
        return self._default

    @property
    def description(self):
        """Return the cvar's description."""
        return self._description

    @property
    def cvar(self):
        """Return the cvar's ConVar instance."""
        return self._cvar


class _ListManager(list):

    """List class used to store text for a specific descriptor of a cvar."""

    def __init__(self, name):
        """Called on instantiation."""
        # Initialize the list
        super(_ListManager, self).__init__()

        # Is the given name a TranslationStrings instance?
        if isinstance(name, TranslationStrings):

            # Get the proper text for the given name
            name = name.get_string()

        # Store the base attributes for the list
        self._name = name
        self.start = '  * '
        self.indent = 9

    @property
    def name(self):
        """Return the name of the list."""
        return self._name

    def append(self, text):
        """Override append to add the proper text."""
        # Is the item a TranslationStrings instance?
        if isinstance(text, TranslationStrings):

            # Get the proper text for the given item
            text = text.get_string()

        # Add the item to the list
        super(_ListManager, self).append(text)
