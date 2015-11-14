# ../messages/dialog.py

"""Provides wrapper classes for the _messages.create_message function."""
# ============================================================================
# >> IMPORTS
# ============================================================================
# Python Imports
#   Collections
from collections import defaultdict

# Source.Python Imports
#   Colors
from colors import WHITE
#   Filters
from filters.recipients import RecipientFilter
#   KeyValues
from keyvalues import KeyValues
#   Listeners
from listeners import LevelShutdown
from listeners.tick import tick_delays
#   Messages
from _messages import DialogType
from _messages import create_message
#   Players
from players.entity import Player
#   Translations
from translations.strings import TranslationStrings


# ============================================================================
# >> GLOBAL VARIABLES
# ============================================================================
# Create a dictionary to store player message levels
_player_levels = defaultdict(set)


# ============================================================================
# >> CLASSES
# ============================================================================
class _DialogBase(object):
    """Base Dialog class used to send Dialog messages to players."""

    # Set the base attributes
    time = 0
    title = ''

    def send(self, *args, **tokens):
        """Send the Dialog message to the appropriate recipients."""
        # Get a recipient filter of the given users
        recipients = RecipientFilter(*args)

        # Get the KeyValues instance
        keyvalues = KeyValues(self.message_type.name.lower())

        # Set the time for the message to display
        keyvalues.set_int('time', self.time)

        # Loop through all recipients
        for index in recipients:

            # Get a Player instance for the current player
            player = Player(index)

            # Is the player not human?
            if player.is_fake_client():
                continue

            # Get the level for the message
            level = self._get_player_level(player.userid)

            # Set the level for the player
            keyvalues.set_int('level', level)

            # Set the title (value should be server IP address)
            keyvalues.set_string(
                'title', self._get_text(self.title, player, **tokens))

            # Set any remaining keyvalues
            self._set_keyvalues(keyvalues, player, **tokens)

            # Send the message
            create_message(player.edict, self.message_type, keyvalues)

    @staticmethod
    def _get_text(message, player, **tokens):
        """Return the text for the given message."""
        # Does the message need to be translated?
        if isinstance(message, TranslationStrings):

            # Return the translated message
            return message.get_string(player.language, **tokens)

        # Return the message itself
        return message

    def _get_player_level(self, userid):
        """Return the current dialog message level for the given player."""
        # Get the player's next message level
        level = max(list(_player_levels[userid]) + [0]) + 1

        # Increase the player's level
        _player_levels[userid].add(level)

        # Remove the level after the time is up
        tick_delays.delay(self.time, _player_levels[userid].remove, level)

        # Return the level
        return level

    @property
    def message_type(self):
        """Raise an error if the inheriting class does not have their own."""
        raise NotImplementedError(
            'No message_type attribute defined for class.')

    @staticmethod
    def _set_keyvalues(keyvalues, player, **tokens):
        """Set any class specific KeyValues.

        As a base, this does nothing.  Inheriting classes need to
        define their own method to handle setting their specific values.
        """


class DialogAskConnect(_DialogBase):
    """Class used to send DialogType.ASKCONNECT messages to players.

    DialogType.ASKCONNECT shows a box in-game that requests the player to hit
    the key they have bound to "askconnect_accept" (default to F3) to connect
    to a different server, whose address is given as the title value.
    """

    message_type = DialogType.ASKCONNECT

    def __init__(self, title, time=10):
        """Set all the base attributes on instantiation."""
        self.title = title
        self.time = time


class DialogEntry(_DialogBase):
    """Class used to send DialogType.ENTRY messages to players.

    DialogType.ENTRY shows the title while in-game (TopText) in the given
    color, as well as the msg in a box when the player uses their ESC key
    and an entry box where they can enter any values they wish.

    The values the player enters are then sent as arguments for the given
    client command name.
    """

    message_type = DialogType.ENTRY

    def __init__(self, title, msg, command, color=WHITE, time=10):
        """Set all the base attributes on instantiation."""
        self.title = title
        self.msg = msg
        self._command = command
        self.color = color
        self.time = time

    @property
    def command(self):
        """Return the registered command for the Entry box."""
        return self._command

    def _set_keyvalues(self, keyvalues, player, **tokens):
        """Set any remaining values to the KeyValues object."""
        keyvalues.set_color('color', self.color)
        keyvalues.set_string('msg', self._get_text(self.msg, player, **tokens))
        keyvalues.set_string('command', self.command)


class DialogMenu(_DialogBase):
    """Class used to send DialogType.MENU messages to players.

    DialogType.MENU shows the title while in-game (TopText) in the given
    color, as well as the msg in a box when the player uses their ESC key
    and a menu for the player to choose an option from.

    The value the player chooses is then sent as an argument for the given
    client command name.
    """

    message_type = DialogType.MENU

    def __init__(
            self, title, msg, command, color=WHITE, time=10, options=None):
        """Set all the base attributes on instantiation."""
        self.title = title
        self.msg = msg
        self._command = command
        self.color = color
        self.time = time
        self.options = options

    @property
    def command(self):
        """Return the registered command for the Menu."""
        return self._command

    def _set_keyvalues(self, keyvalues, player, **tokens):
        """Set any remaining values to the KeyValues object."""
        # Are there any options for the menu?
        if self.options is None:
            raise ValueError(
                'Invalid value for options.  Attribute must be iterable.')

        # Set the base values
        keyvalues.set_color('color', self.color)
        keyvalues.set_string('msg', self._get_text(self.msg, player, **tokens))

        # Are the options in a dictionary?
        if isinstance(self.options, dict):
            for key, value in self.options.items():
                button = keyvalues.find_key(str(key), True)
                button.set_string(
                    'msg', self._get_text(value, player, **tokens))
                button.set_string(
                    'command', '{0} {1}'.format(self.command, key))
            return

        # The options are in a non-dictionary iterable
        for item in self.options:
            button = keyvalues.find_key(item, True)
            button.set_string('msg', item)
            button.set_string(
                'command', '{0} {1}'.format(self.command, item))


class DialogMsg(_DialogBase):
    """Class used to send DialogType.MSG messages to players.

    DialogType.MSG shows the title while in-game (TopText) in the given color.
    """

    message_type = DialogType.MSG

    def __init__(self, title, color=WHITE, time=10):
        """Set all the base attributes on instantiation."""
        self.title = title
        self.color = color
        self.time = time

    def _set_keyvalues(self, keyvalues, player, **tokens):
        """Set any remaining values to the KeyValues object."""
        keyvalues.set_color('color', self.color)


class DialogText(_DialogBase):
    """Class used to send DialogType.TEXT messages to players.

    DialogType.TEXT shows the title while in-game (TopText) in the given
    color, as well as the msg in a box when the player uses their ESC key.
    """

    message_type = DialogType.TEXT

    def __init__(self, title, msg, color=WHITE, time=10):
        """Set all the base attributes on instantiation."""
        self.title = title
        self.msg = msg
        self.color = color
        self.time = time

    def _set_keyvalues(self, keyvalues, player, **tokens):
        """Set any remaining values to the KeyValues object."""
        keyvalues.set_color('color', self.color)
        keyvalues.set_string('msg', self._get_text(self.msg, player, **tokens))


# ============================================================================
# >> LISTENERS
# ============================================================================
@LevelShutdown
def _level_shutdown():
    """Clear the player levels dictionary."""
    _player_levels.clear()
