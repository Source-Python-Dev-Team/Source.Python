# =============================================================================
# >> IMPORTS
# =============================================================================
# Python
from collections import defaultdict

# Source.Python
#   core
from core import AutoUnload
#   events
from events import Event
#   filters
from filters.players import PlayerIter
#   engines
from engines.server import EngineServer
#   translations
from translations.strings import TranslationStrings
#   players
from players.helpers import index_from_userid


# =============================================================================
# >> CLASSES
# =============================================================================
class _PlayerPage(object):
    '''
    This class is used to save information about the player's current
    active page.
    '''

    def __init__(self):
        '''
        Initializes the _PlayerPage instance.
        '''

        self.index = 0
        self.options = []


class _BaseMenu(AutoUnload, list):
    '''
    This is the base menu. Every menu class should inherit from this class.
    '''

    _instances = {}

    def __init__(self, data=[], select_callback=None, build_callback=None):
        '''
        Initializes the menu.

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
        '''

        super(_BaseMenu, self).__init__(data)

        self.select_callback = select_callback
        self.build_callback = build_callback
        self._player_pages = defaultdict(_PlayerPage)
        self._instances[id(self)] = self

    def _unload_instance(self):
        '''
        Unloads this instance. It will close this menu instance for every
        player.
        '''

        # Just close all open menus, which will remove all instances from the
        # queues
        self.close()

        # Also remove the instance from the _instances dict
        del self._instances[id(self)]

    def _unload_player(self, ply_index):
        '''
        Removes every player specific information.

        @param <ply_index>:
        A player index.
        '''

        self._player_pages.pop(ply_index, 0)

    def _refresh(self, ply_index):
        '''
        Re-sends the menu to a player.

        @param <ply_index>:
        The index of the player whose menu should be refreshed.
        '''

        self._send(ply_index)

    def _build(self, ply_index):
        '''
        Returns all relevant menu data, but calls the build callback before.

        @param <ply_index>:
        The index of the player whose menu should be built.
        '''

        # Call the build callback if there is one
        if self.build_callback is not None:
            self.build_callback(self, ply_index)

        return self._get_menu_data(ply_index)

    def _select(self, ply_index, choice):
        '''
        Handles a menu selection.

        @param <ply_index>:
        The index of the player who made the selection.

        @param <choice>:
        A numeric value that defines what was selected.
        '''

        if self.select_callback is not None:
            return self.select_callback(self, ply_index, choice)

    def send(self, *ply_indexes):
        '''
        Sends the menu to the given player indexes.

        If no indexes were given, the menu will be sent to every player.
        '''

        if not ply_indexes:
            ply_indexes = PlayerIter('human')

        for ply_index in ply_indexes:
            queue = self.get_user_queue(ply_index)
            queue.append(self)
            queue._refresh()

    def close(self, *ply_indexes):
        '''
        Closes the menu for the given player indexes.

        If no indexes were given, the menu will be closed for all players.
        '''

        if not ply_indexes:
            ply_indexes = PlayerIter('human')

        for ply_index in ply_indexes:
            queue = self.get_user_queue(ply_index)

            # Try to remove this menu from the queue
            try:
                queue.remove(self)
            except ValueError:
                # If it fails, do nothing
                pass
            else:
                # If the queue is now empty, send an empty menu to hide
                # the last menu
                if not queue:
                    # Send an empty menu
                    self._close(ply_index)

                else:
                    # There is at least one menu in the queue, so refresh to
                    # display it.
                    queue._refresh()

    def is_active_menu(self, ply_index):
        '''
        Returns True if this menu is currently the first menu in the user's
        queue.

        @param <ply_index>:
        A player index.
        '''

        return self.get_user_queue(ply_index).get_active_menu() is self

    @classmethod
    def get_user_queue(cls, ply_index):
        '''
        Returns the menu queue for the given player.

        @param <ply_index>:
        A player index.
        '''

        return cls._get_queue_holder()[ply_index]

    @staticmethod
    def _get_queue_holder():
        '''
        This method needs to be implemented by a subclass!

        It should return a _QueueHolder object.
        '''

        raise NotImplementedError

    def _get_menu_data(self, ply_index):
        '''
        This method needs to be implemented by a subclass!

        It should return the required data to send a menu.

        @param <ply_index>:
        A player index.
        '''

        raise NotImplementedError

    def _send(self, ply_index):
        '''
        This method needs to be implemented by a subclass!

        This method should directly send a menu to the player.

        @param <ply_index>:
        A player index.
        '''

        raise NotImplementedError

    def _close(self, ply_index):
        '''
        This method needs to be implemented by a subclass!

        This method should close a menu by overriding it with an empty menu.

        @param <ply_index>:
        A player index.
        '''

        raise NotImplementedError


class _MenuData(object):
    '''
    This is the base class for menu data.

    All data types should inherit from this class.
    '''

    def __init__(self, text):
        '''
        Initializes the instance.

        @param <text>:
        The text that should be displayed.
        '''

        self.text = text

    def _render(self, ply_index):
        '''
        Renders the data.

        @param <ply_index>:
        A player index.
        '''

        raise NotImplementedError


class Text(_MenuData):
    '''
    This class is used to display plain text.

    NOTE:
    ESC menus will ignore this data type as it is not possible to show plain
    text with that menu type.
    '''

    def _render(self, ply_index):
        '''
        Renders the data.

        @param <ply_index>:
        A player index.
        '''

        return str(_translate_text(self.text, ply_index)) + '\n'


class _BaseOption(_MenuData):
    '''
    This class is used to display an enumerated option.
    '''

    def __init__(self, text, value=None, highlight=True, selectable=True):
        '''
        Initializes the option.

        @param <text>:
        The text that should be displayed.

        @param <value>:
        The value that should be passed to the menu's selection callback.

        @param <hightlight>:
        Set this to true if the text should be hightlighted.

        NOTE: This only works with radio menus.

        @param <selectable>:
        Set this to True if the option should be selectable.
        '''

        super(_BaseOption, self).__init__(text)
        self.value = value
        self.highlight = highlight
        self.selectable = selectable

    def _render(self, ply_index, choice):
        '''
        Renders the data.

        @param <ply_index>:
        A player index.

        @param <choice>:
        A numeric value that defines the selection number.
        '''

        raise NotImplementedError


# =============================================================================
# >> HELPER FUNCTIONS
# =============================================================================
def _translate_text(text, ply_index):
    '''
    Translates <text> if it is an instance of TranslationStrings. Otherwise
    the original text will be returned.
    '''

    if isinstance(text, TranslationStrings):
        return text.get_string(
            EngineServer.get_client_convar_value(ply_index, 'cl_language'))

    return text


# =============================================================================
# >> GAME EVENTS
# =============================================================================
@Event
def player_disconnect(event):
    '''
    Called whenever a player left the server.
    '''

    index = index_from_userid(event.get_int('userid'))
    for instance in _BaseMenu._instances.values():
        instance._unload_player(index)