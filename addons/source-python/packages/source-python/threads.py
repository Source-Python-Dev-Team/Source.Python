# ../threads.py

"""Provides threads functionality.

.. data:: sp_thread_yielding

    If enabled, yields remaining cycles to Python threads every frame.

    .. note::

        Not all games are currently supported.
        See also: :func:`ThreadYielder.is_implemented`
"""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Python Imports
#   FuncTools
from functools import partial
from functools import wraps
#   Threading
from threading import Event
from threading import Thread
from threading import current_thread
from threading import main_thread
#   Queue
from queue import Queue
#   Sys
from sys import getswitchinterval
#   Time
from time import sleep
#   Warnings
from warnings import warn

# Source.Python Imports
#   Core
from core import get_calling_plugin
from core import WeakAutoUnload
from core import autounload_disabled
from core.cache import cached_property
#   Cvars
from cvars import ConVar
#   Engines
from engines.server import global_vars
#   Hooks
from hooks.exceptions import except_hooks
#   Listeners
from listeners.tick import Delay


# =============================================================================
# >> FORWARD IMPORTS
# =============================================================================
# Source.Python Imports
#   Core
from _threads import ThreadYielder


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = (
    'GameThread',
    'InfiniteThread',
    'Partial',
    'Queued',
    'ThreadYielder',
    'queued',
    'sp_thread_yielding',
    'threaded',
)


# =============================================================================
# >> GLOBAL VARIABLES
# =============================================================================
sp_thread_yielding = ConVar('sp_thread_yielding')


# =============================================================================
# >> CLASSES
# =============================================================================
class GameThread(WeakAutoUnload, Thread):
    """A subclass of :class:`threading.Thread` that throws a warning if the
    plugin that created the thread has been unloaded while the thread is still
    running.

    Example:

    .. code:: python

        from threads import GameThread

        def function():
            ...

        thread = GameThread(function).start()

    .. warning::

        Multiple active threads or threading routines that are heavy on CPU
        can have a huge impact on the networking of the server.
    """

    def __init__(self, target=None, *args, **kwargs):
        """Initializes the thread.

        :param callable target:
            The target function to execute.
        :param tuple *args:
            The arguments to pass to ``Thread.__init__``.
        :param dict **kwargs:
            The keyword arguments to pass to ``Thread.__init__``.
        """
        super().__init__(None, target, *args, **kwargs)

    def _add_instance(self, caller):
        """Adds the instance and store the caller."""
        super()._add_instance(caller)
        self._caller = caller

    def _unload_instance(self):
        """Unloads the instance."""
        # Give it at least a frame and a switch to conclude
        if self._started.is_set():
            self.join(global_vars.interval_per_tick + getswitchinterval())

        # Raise awarness if that was not enough time to conclude
        if self.is_alive():
            warn(
                f'Thread "{self.name}" ({self.ident}) from "{self._caller}" '
                f'is running even though its plugin has been unloaded!')

    @cached_property
    def yielder(self):
        """Returns the yielder for this thread.

        :rtype:
            ThreadYielder
        """
        return ThreadYielder()

    def start(self):
        """Starts the thread.

        :return:
            Return itself so that it can be inlined.
        """
        super().start()
        return self

    def run(self):
        """Runs the thread."""
        with self.yielder:
            super().run()


class InfiniteThread(GameThread):
    """Thread that runs infinitely.

    Example:

    .. code:: python

        from threads import InfiniteThread

        class MyInfiniteThread(InfiniteThread):
            def __call__(self):
                with self.yielder:
                    ...

        thread = MyInfiniteThread().start(1)
    """

    @cached_property
    def wait_signal(self):
        """When clear, the current iteration is done waiting.

        :rtype:
            threading.Event
        """
        return Event()

    @cached_property
    def exit_signal(self):
        """When set, the thread has been stopped and terminated.

        :rtype:
            threading.Event
        """
        return Event()

    @cached_property
    def interval(self):
        """The time between each iteration.

        :rtype:
            float
        """
        return getswitchinterval()

    def __call__(self, *args, **kwargs):
        """Calls the target with the given arguments.

        :param tuple *args:
            The arguments to pass to the target function.
        :param dict **kwargs:
            The keyword arguments to pass to the target function.

        :return:
            The value returned by the target function.

        .. note::

            The thread will stop looping if a ``SystemExit`` is caught.
        """
        with self.yielder:
            return self._target(*args, **kwargs)

    def start(self, interval=None, wait=False):
        """Starts iterating.

        :param float interval:
            The time between each iteration.
        :param bool wait:
            Whether we should wait an interval for the first iteration.

        :return:
            Return itself so that it can be inlined.
        """
        # Set our interval if any was given
        if interval is not None:
            type(self).interval.set_cached_value(self, interval)

        # Set our wait signal so that we start iterating immediately
        if not wait:
            self.wait_signal.set()

        # Start iterating
        super().start()

        # Clear our wait signal so that we start waiting again
        if not wait:
            self.wait_signal.clear()

        # Return ourself so that it can be inlined
        return self

    def stop(self):
        """Stops iterating."""
        # Set our exit signal so that we stop iterating
        self.exit_signal.set()

        # Stop waiting for the next iteration
        self.wait_signal.set()

    def run(self):
        """Runs the thread.

        :raise SystemExit:
            When the asynchronous state is being terminated.
        """
        # Get our interval
        interval = self.interval

        # Get our signals
        exit_signal, wait_signal = self.exit_signal, self.wait_signal

        # Get our arguments
        args, kwargs = self._args, self._kwargs

        # Start iterating
        while True:

            # Wait the interval
            if interval:
                wait_signal.wait(interval)

            # Have we been stopped since the last iteration?
            if exit_signal.is_set():

                # Delete our target and arguments
                del self._target, self._args, self._kwargs

                # Terminate the asynchronous state
                raise SystemExit(f'{self.name} was terminated.')

            # Call the target with the given arguments
            try:
                self(*args, **kwargs)
            except SystemExit:
                self.stop()
            except Exception:
                except_hooks.print_exception()

    def _unload_instance(self):
        """Stops iterating and unloads the instance."""
        # Stop iterating
        self.stop()

        # Unload the instance
        super()._unload_instance()


class Partial(partial):
    """Represents a partial that can have a callback bound to it."""

    def waitable(self):
        """Makes the partial waitable when called from a parallel thread.

        :return:
            Return itself so that it can be inlined.
        """
        self._waitable = True
        return self

    def unloadable(self):
        """Makes the partial unloadable by the calling plugin.

        :return:
            Return itself so that it can be inlined.
        """
        # Get the module name of the calling plugin
        caller = get_calling_plugin()

        # Flag the partial as unloadable
        if caller is not None:
            WeakAutoUnload._module_instances[caller][id(self)] = self

        # Return ourself so that it can be inlined
        return self

    def with_callback(self, callback, *args, **kwargs):
        """Binds the given callback to the instance.

        :param callable callback:
            The callback to bind to this partial.
        :param tuple *args:
            The argument to pass to the callback.
        :param dict **kwargs:
            The keyword arguments to pass to the callback.

        :return:
            Return itself so that it can be inlined.

        .. note::

            The callback will be called in the main thread.
        """
        self.callback = partial(callback, *args, **kwargs)
        return self

    def __call__(self, *args, **kwargs):
        """Calls the partial and pass the result to its callback.

        :param tuple *args:
            The argument to pass to the partial.
        :param dict **kwargs:
            The keyword arguments to pass to the partial.

        :raise RuntimeError:
            If the plugin that owns this partial was unloaded.
        """
        # Sleep it off until the next context switch
        try:
            if self._waitable and current_thread() is not main_thread():
                sleep(getswitchinterval())
        except AttributeError:
            pass

        # Raise if we were unloaded
        try:
            if self._unloaded:
                raise RuntimeError('Partial was unloaded.')
        except AttributeError:
            pass

        # Call the partial and get the result
        result = super().__call__(*args, **kwargs)

        # Try to resolve its callback
        try:
            callback = self.callback
        except AttributeError:
            return result

        # Call the callback with the result in the main thread
        Delay(0, callback, args=(result,))

    def _unload_instance(self):
        """Flags the instance as unloaded."""
        self._unloaded = True


class Queued(WeakAutoUnload, Queue):
    """Callables added to this queue are invoked one at a time
    from a parallel thread.

    Example:

    .. code:: python

        from threads import Queued

        queue = Queued()
        queue(print, 'This print was queued!')

    .. note::

        If you don't need to manage your own queue, you should consider
        using :func:`threads.queued` instead.

    .. warning::

        If you mutate the internal queue, you are responsible to manage
        the internal thread as well.
    """

    @cached_property
    def thread(self):
        """The internal thread that processes the queue.

        .. warning::

            This should be left alone unless you manually mutate
            the internal queue.
        """
        with autounload_disabled():
            return InfiniteThread(self.process).start(wait=True)

    @thread.deleter
    def _(self):
        """Stops the internal thread."""
        self.thread.stop()

    @wraps(Queue.put)
    def put(self, *args, **kwargs):
        """Wrapper around `Queue.put` to ensure the thread is running."""
        super().put(*args, **kwargs)
        self.thread

    @wraps(Queue.get)
    def get(self, *args, **kwargs):
        """Wrapper around `Queue.get` to stop the thread when empty."""
        result = super().get(*args, **kwargs)
        if self.empty():
            del self.thread
        self.task_done()
        return result

    def clear(self):
        """Empties the internal queue and invalidates the internal thread."""
        with self.mutex:
            self.queue.clear()
        del self.thread

    def  __call__(self, function, *args, **kwargs):
        """Adds the given function and argument to the queue.

        :param callable function:
            The function to add to the queue.
        :param tuple *args:
            The arguments to pass to the given function.
        :param dict **kwargs:
            The keyword arguments to pass to the given function.

        :return:
            The partial generated from the given function and arguments.

        :rtype:
            Partial
        """
        partial = Partial(function, *args, **kwargs)
        self.put(partial)
        return partial

    def process(self):
        """Calls the next partial in the queue.

        :return:
            The value returned by the called partial.
        """
        partial = self.get()
        if partial is None:
            return
        try:
            if partial._unloaded:
                return
        except AttributeError:
            pass
        return partial()

    def _unload_instance(self):
        """Empties the internal queue and invalidates the internal thread."""
        self.clear()


# =============================================================================
# >> FUNCTIONS
# =============================================================================
def queued(function, *args, **kwargs):
    """Queues a call to the given function.

    :param callable function:
        The function to call.
    :param tuple *args:
        The arguments to pass to the given function.
    :param dict **kwargs:
        The keyword arguments to pass to the given function.

    :rtype:
        Partial

    Example:

    .. code:: python

        import time
        from threads import queued

        def done_sleeping(start, result):
            print(f'Done waiting and sleeping for {time.time() - start} seconds!')

        queued(time.sleep, 3).with_callback(done_sleeping, time.time())
    """
    global _queued
    try:
        _queued
    except NameError:
        with autounload_disabled():
            _queued = Queued()
    return _queued(function, *args, **kwargs).unloadable()


def threaded(function, *args, **kwargs):
    """Calls the given function with the given arguments in a new thread.

    :param callable function:
        The function to call.
    :param tuple *args:
        The arguments to pass to the given function.
    :param dict **kwargs:
        The keyword arguments to pass to the given function.

    :rtype:
        Partial

    Example:

    .. code:: python

        import time
        from threads import threaded

        def sleep(seconds):
            time.sleep(seconds)
            return seconds

        def done_sleeping(result):
            print(f'Done sleeping for {result} seconds!')

        threaded(sleep, 2).with_callback(done_sleeping)

    .. note::

        If the call can wait its turn, consider :func:`threads.queued` instead.
    """
    partial = Partial(function, *args, **kwargs).waitable()
    GameThread(partial).start()
    return partial
