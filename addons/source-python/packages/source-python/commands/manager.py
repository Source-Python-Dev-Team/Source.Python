# ../commands/manager.py

"""Provides a base class for registering commands and filters."""


# =============================================================================
# >> CLASSES
# =============================================================================
class _BaseCommandManager(object):
    """Class used to (un)register commands."""

    # Can be set to a class that acts like a proxy for commands. It will be
    # registered instead of a callback. If the command was issued, the
    # callback manager will be called and it's its responsibility to call the
    # actual callback.
    _callback_proxy = None

    def __init__(self):
        """Initialize the command manager."""
        # This will store all created callback proxies
        self._callback_proxies = []

    def _prepare_command_names(self, names):
        """Validate and prepare the given command names.

        The given argument can be a string, list or tuple. A TypeError is
        raised if it does not meet the requirements.
        """
        # Was a single command name given?
        if isinstance(names, str):
            names = [names]

        # Are the names of a proper iterable type?
        if not isinstance(names, (list, tuple)):
            raise TypeError(
                '{0} commands must be passed as a list, tuple, or string,'
                ' not "{1}"'.format(type(self).__name__, type(names).__name__))

        return names

    def _get_command_proxy(self, callback):
        """Return the command proxy for the given callback.

        Raise a ValueError when the proxy wasn't found.
        """
        for proxy in self._callback_proxies:
            if proxy.callback is callback:
                return proxy

        raise ValueError('Unable to find a proxy for the given callback.')

    def register_commands(self, names, callback, *args, **kwargs):
        """Register the given commands to the given callback."""
        names = self._prepare_command_names(names)

        if self._callback_proxy is not None:
            # Create a new callback proxy for this callback
            callback = self._callback_proxy(callback, *args, **kwargs)
            self._callback_proxies.append(callback)

        # Register all command names
        for name in names:
            # TODO: Server commands can also set a help text and flags.
            #       This isn't considered here...
            self._get_command(name).add_callback(callback)

    def unregister_commands(self, names, callback):
        """Unregister the given commands from the given callback."""
        names = self._prepare_command_names(names)

        if self._callback_proxy is not None:
            callback = self._get_command_proxy(callback)
            self._callback_proxies.remove(callback)

        # Unregister all command names
        for name in names:
            self._get_command(name).remove_callback(callback)
