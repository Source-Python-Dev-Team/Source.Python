# ../commands/manager.py

"""Provides a base class for registering commands and filters."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Python
from collections import defaultdict


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
        self._callback_proxies = defaultdict(list)

    def _prepare_command_names(self, names):
        """Validate and prepare the given command names.

        :param str/list/tuple names:
            A single command or multiple commands that should be validated.
        :raise TypeError:
            Raised if ``names`` is not a :class:`str`, :class:`list` or
            class:`tuple`.
        :rtype: tuple
        """
        # Was a single command name given?
        if isinstance(names, str):
            names = [names]

        # Are the names of a proper iterable type?
        if not isinstance(names, (list, tuple)):
            raise TypeError(
                '{0} commands must be passed as a list, tuple, or string,'
                ' not "{1}"'.format(type(self).__name__, type(names).__name__))

        return tuple(names)

    def _get_command_proxy(self, name, callback):
        """Return the command proxy for the given callback.

        :param str name:
            Name of the command.
        :param callable callback:
            The callback that should be used to search the proxy.
        :raise ValueError:
            Raised if the proxy wasn't found.
        """
        for proxy in self._callback_proxies.get(name, ()):
            if proxy.callback == callback:
                return proxy

        raise ValueError('Unable to find a proxy for the given callback.')

    def register_commands(self, names, callback, *args, **kwargs):
        """Register the given commands to the given callback.

        :param str/list/tuple names:
            Command names that should be registered.
        :param callable callback:
            A callback that should get called when one of the commands were
            issued.
        :param args:
            Additional arguments that get passed to the proxy (if existant)
            and to :meth:`_register_command`.
        :param kwargs:
            Additional keywords that get passed to the proxy (if existant)
            and to :meth:`_register_command`.
        """
        names = self._prepare_command_names(names)

        if self._callback_proxy is None:
            for name in names:
                self._register_command(name, callback, *args, **kwargs)
        else:
            proxy = self._callback_proxy(callback, *args, **kwargs)

            for name in names:
                self._callback_proxies[name].append(proxy)
                self._register_command(name, proxy, *args, **kwargs)

    def _register_command(self, name, callback, *args, **kwargs):
        """Register a command.

        :param str name:
            A single command name that should be registred.
        :param callable callback:
            The callback that should get called when the command has been issued.
        :param args:
            Additional arguments passed from :meth:`register_commands`.
        :param kwargs:
            Additional keywords passed from :meth:`register_commands`.
        """
        self._get_command(name).add_callback(callback)

    def unregister_commands(self, names, callback):
        """Unregister the given commands from the given callback.

        :param str/list/tuple names:
            Command names to unregister.
        :param callable callback:
            The callback that was registered for the commands.
        """
        names = self._prepare_command_names(names)

        if self._callback_proxy is None:
            for name in names:
                self._unregister_command(name, callback)
        else:
            for name in names:
                proxy = self._get_command_proxy(name, callback)
                self._callback_proxies[name].remove(proxy)

                if not self._callback_proxies[name]:
                    del self._callback_proxies[name]

                self._unregister_command(name, proxy)

    def _unregister_command(self, name, callback):
        """Unregister a command.

        :param str name:
            Name of the command to unregister.
        :param callable callback:
            The callback that is assigned to the command.
        """
        self._get_command(name).remove_callback(callback)
