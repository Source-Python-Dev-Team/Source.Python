# ../listeners/tick.py

"""Provides tick listener based functionality."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Python
import bisect
import math
import time

from contextlib import suppress
from enum import IntEnum
from threading import Thread
from warnings import warn

# Source.Python
from core import AutoUnload
from core import WeakAutoUnload
from hooks.exceptions import except_hooks
from listeners import (
    listeners_logger, on_tick_listener_manager, OnLevelEnd,
)


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = (
    'Delay',
    'GameThread',
    'Repeat',
    'RepeatStatus',
)


# =============================================================================
# >> GLOBAL VARIABLES
# =============================================================================
# Get the sp.listeners.tick logger
listeners_tick_logger = listeners_logger.tick


# =============================================================================
# >> THREAD WORKAROUND
# =============================================================================
class GameThread(WeakAutoUnload, Thread):
    """A subclass of :class:`threading.Thread` that throws a warning if the
    plugin that created the thread has been unloaded while the thread is still
    running.
    """

    def _add_instance(self, caller):
        super()._add_instance(caller)
        self._caller = caller

    def _unload_instance(self):
        if self.is_alive():
            warn(
                f'Thread "{self.name}" ({self.ident}) from "{self._caller}" '
                f'is running even though its plugin has been unloaded!')


# =============================================================================
# >> DELAY CLASSES
# =============================================================================
class _DelayManager(list):
    """A class that is responsible for executing delays."""

    def __init__(self):
        super().__init__()
        on_tick_listener_manager.register_listener(self._tick)

    def _tick(self):
        """Internal tick listener."""
        current_time = time.time()
        while self and self[0].exec_time <= current_time:
            try:
                self.pop(0).execute()
            except:
                except_hooks.print_exception()

    def add(self, delay):
        """Add a delay to the list.

        :param Delay delay:
            The delay to add.
        """
        bisect.insort_right(self, delay)

_delay_manager = _DelayManager()


class Delay(WeakAutoUnload):
    """Execute a callback after a given delay."""

    def __init__(
        self, delay, callback, args=(), kwargs=None, cancel_on_level_end=False
    ):
        """Initialize the delay.

        :param float delay:
            The delay in seconds.
        :param callback:
            A callable object that should be called after the delay expired.
        :param tuple args:
            Arguments that should be passed to the callback.
        :param dict kwargs:
            Keyword arguments that should be passed to the callback.
        :param bool cancel_on_level_end:
            Whether or not to cancel the delay at the end of the map.
        :raise ValueError:
            Raised if the given callback is not callable.
        """
        if not callable(callback):
            raise ValueError('Given callback is not callable.')

        #: Delay in seconds.
        self.delay = delay
        self._start_time = time.time()

        #: Time when the delay will be executed.
        self.exec_time = self._start_time + delay

        #: Callback to call when the delay expired.
        self.callback = callback

        #: Arguments to pass to the callback.
        self.args = args

        #: Keyword arguments to pass to the callback.
        self.kwargs = kwargs if kwargs is not None else dict()

        #: Whether or not to cancel the delay at the end of the map.
        self.cancel_on_level_end = cancel_on_level_end
        _delay_manager.add(self)

    def __lt__(self, other):
        """Return True if this :attr:`exec_time` is less than the other's.

        :rtype: bool
        """
        return self.exec_time < other.exec_time

    def __call__(self):
        """Cancel the delay and immediately call the callback.

        :return:
            The result of :meth:`execute`.
        """
        self.cancel()
        return self.execute()

    def execute(self):
        """Call the callback.

        :return:
            The result of :attr:`callback`.
        """
        return self.callback(*self.args, **self.kwargs)

    def cancel(self):
        """Cancel the delay.

        :raise ValueError: Raised if the delay is not running.
        """
        _delay_manager.remove(self)

    @property
    def running(self):
        """Return True if the delay running.

        :rtype: bool
        """
        return self in _delay_manager

    @property
    def time_remaining(self):
        """Return the remaining time (in seconds) until the Delay ends.

        :rtype: float
        """
        if not self.running:
            # TODO: what should we return here, or should we raise an error?
            return None
        return self.exec_time - time.time()

    @property
    def time_elapsed(self):
        """Return the amount of time (in seconds) since the Delay started.

        :rtype: float
        """
        if not self.running:
            # TODO: what should we return here, or should we raise an error?
            return None
        return time.time() - self._start_time

    def _unload_instance(self):
        with suppress(ValueError):
            self.cancel()


# =============================================================================
# >> REPEAT CLASSES
# =============================================================================
class RepeatStatus(IntEnum):
    """Class used to store RepeatStatus values."""

    STOPPED = 1
    RUNNING = 2
    PAUSED = 3


class Repeat(AutoUnload):
    """Class used to create and call repeats."""

    def __init__(
        self, callback, args=(), kwargs=None, cancel_on_level_end=False
    ):
        """Store all instance attributes.

        :param callback:
            A callable object that should be called at the end of each loop.
        :param tuple args:
            Arguments that should be passed to the callback.
        :param dict kwargs:
            Keyword arguments that should be passed to the callback.
        :param bool cancel_on_level_end:
            Whether or not to cancel the repeat at the end of the map.
        :raise ValueError:
            Raised if the given callback is not callable.
        """
        if not callable(callback):
            raise ValueError('Given callback is not callable.')

        # Store the base attributes
        self.callback = callback
        self.args = args
        self.kwargs = kwargs if kwargs is not None else dict()
        self.cancel_on_level_end = cancel_on_level_end

        # Log the __init__ message
        listeners_tick_logger.log_debug(
            'Repeat.__init__: <{self.callback}> <{self.args}>'
            ' <{self.kwargs}>'.format(
                self=self
            )
        )

        # Set up private attributes
        self._interval = 0
        self._original_loops = math.inf
        self._loops_elapsed = 0
        self._adjusted_loops = 0
        self._status = RepeatStatus.STOPPED
        self._delay = None
        self._loop_time_for_pause = None
        self._original_start_time = None

    @property
    def interval(self):
        """Return the interval in which the callback will be called.

        :rtype: int
        """
        return self._interval

    @property
    def adjusted_loops(self):
        """Return the number of loops that have been adjusted.

        :rtype: int
        """
        return self._adjusted_loops

    @property
    def loops_remaining(self):
        """Return the remaining number of loops in the repeat.

        :rtype: int
        """
        return self.total_loops - self.loops_elapsed

    @property
    def loops_elapsed(self):
        """Return the current number of loops made in the repeat.

        :rtype: int
        """
        return self._loops_elapsed

    @property
    def total_loops(self):
        """Return the total number of loops to be made.

        :rtype: int
        """
        return self._original_loops + self._adjusted_loops

    @property
    def original_loops(self):
        """Return the number of loops the repeat has been started with.

        :rtype: int
        """
        return self._original_loops

    @property
    def total_time_remaining(self):
        """Return the remaining time till the end of the repeat.

        :rtype: float
        """
        if self.delay_time_remaining is None:
            return None
        return (
            self.loops_remaining * self.interval +
            self.delay_time_remaining
        )

    @property
    def total_time_elapsed(self):
        """Return the elapsed time since the repeat started.

        :rtype: float
        """
        return time.time() - self._original_start_time

    @property
    def total_time(self):
        """Return the total time it will take to complete the repeat.

        :rtype: float
        """
        return self.total_loops * self.interval

    @property
    def delay_time_remaining(self):
        """Return the time remaining in the current loop.

        :rtype: float
        """
        return self._delay.time_remaining

    @property
    def delay_time_elapsed(self):
        """Return the time elapsed in the current loop.

        :rtype: float
        """
        return self._delay.time_elapsed

    @property
    def status(self):
        """Return the status of the repeat.

        :rtype: RepeatStatus
        """
        return self._status

    def start(self, interval, limit=math.inf, execute_on_start=False):
        """Start the repeat loop.

        :param float interval:
            The time (in seconds) for each loop.
        :param int limit:
            The maximum number of times to loop. If :data:`math.inf` is
            passed, there is no limit, and the Repeat will loop indefinitely.
        :param bool execute_on_start:
            Whether to execute the callback when the Repeat is started. Note
            that this does not affect the 'limit' as the number of loops will
            remain the same.
        """
        # Log the start message
        listeners_tick_logger.log_debug(
            'Repeat.start: <{interval}> <{limit}>'.format(
                interval=interval,
                limit=limit
            )
        )

        # Is the repeat already running?
        if self._status is RepeatStatus.RUNNING:

            # Log the status
            listeners_tick_logger.log_debug(
                'Repeat.start - RepeatStatus.RUNNING'
            )

            # Do not start the repeat
            return

        # Log starting the repeat
        listeners_tick_logger.log_debug(
            'Repeat.start - !RepeatStatus.RUNNING - Starting Repeat'
        )

        self._status = RepeatStatus.RUNNING
        self._interval = interval
        self._original_loops = limit
        self._loops_elapsed = 0
        self._adjusted_loops = 0
        self._original_start_time = time.time()

        # Start the delay
        self._delay = Delay(
            self.interval, self._execute,
            cancel_on_level_end=self.cancel_on_level_end
        )

        # Call the callback if set to execute on start
        if execute_on_start:
            self.callback(*self.args, **self.kwargs)

    def stop(self):
        """Stop the repeat loop."""
        # Log the stop message
        listeners_tick_logger.log_debug('Repeat.stop')

        # Is the repeat running?
        if self._status is not RepeatStatus.RUNNING:

            # Log the status
            listeners_tick_logger.log_debug(
                'Repeat.stop - !RepeatStatus.RUNNING'
            )

            # No need to stop it
            return

        # Log stopping the repeat
        listeners_tick_logger.log_debug(
            'Repeat.stop - RepeatStatus.RUNNING - Stopping Repeat'
        )

        # Set the status to stopped
        self._status = RepeatStatus.STOPPED

        # Cancel the delay
        self._delay.cancel()

    def restart(self):
        """Restart the repeat."""
        # Log restarting the repeat
        listeners_tick_logger.log_debug('Repeat.restart')

        # Stop the repeat
        self.stop()

        # Start the repeat
        self.start(self.interval, self.total_loops)

    def pause(self):
        """Pause the repeat.

        Pausing allows the repeat to be resumed.
        """
        # Log the pause message
        listeners_tick_logger.log_debug('Repeat.pause')

        # Is the repeat running?
        if self._status is not RepeatStatus.RUNNING:

            # Log the status
            listeners_tick_logger.log_debug(
                'Repeat.pause - !RepeatStatus.RUNNING'
            )

            # No need to pause
            return

        # Log pausing the repeat
        listeners_tick_logger.log_debug(
            'Repeat.pause - RepeatStatus.RUNNING - Pausing Repeat'
        )

        # Set the status to paused
        self._status = RepeatStatus.PAUSED

        # Set the remaining time in the current loop
        self._loop_time_for_pause = self._delay.time_remaining

        # Cancel the delay
        self._delay.cancel()

    def resume(self):
        """Resume the repeat.

        Can only resume if in paused status.
        """
        # Log the resume message
        listeners_tick_logger.log_debug('Repeat.resume')

        # Is the repeat paused?
        if self._status is not RepeatStatus.PAUSED:

            # Log the status
            listeners_tick_logger.log_debug(
                'Repeat.resume - !RepeatStatus.PAUSED'
            )

            # Do not resume
            return

        # Log resuming the repeat
        listeners_tick_logger.log_debug(
            'Repeat.resume - RepeatStatus.PAUSED - Resuming Repeat'
        )

        # Set the status to running
        self._status = RepeatStatus.RUNNING

        # Start the delay
        self._delay = Delay(
            self._loop_time_for_pause, self._execute,
            cancel_on_level_end=self.cancel_on_level_end
        )

    def extend(self, adjustment):
        """Add to the number of loops to be made.

        :param int adjustment:
            The number of loops to be added to the limit.
        :raise ValueError:
            Raised if given adjustment is not a positive integer.
        """
        listeners_tick_logger.log_debug('Repeat.extend')

        # Is there no limit for this repeat?
        if self.total_loops == math.inf:
            listeners_tick_logger.log_debug(
                'Unable to extend, Repeat instance has no limit.'
            )
            return

        # Was a positive integer given?
        if not isinstance(adjustment, int) or adjustment < 1:
            raise ValueError('Adjusted value must be a positive integer')

        # Add to the adjusted number
        self._adjusted_loops += adjustment

    def reduce(self, adjustment):
        """Reduce the number of loops to be made.

        :param int adjustment:
            The number of loops to be removed from the limit.
        :raises ValueError:
            Raised if given adjustment is not a positive integer.
        """
        listeners_tick_logger.log_debug('Repeat.reduce')

        # Is there no limit for this repeat?
        if self.total_loops == math.inf:
            listeners_tick_logger.log_debug(
                'Unable to reduce, Repeat instance has no limit.'
            )
            return

        # Was a positive integer given?
        if not isinstance(adjustment, int) or adjustment < 1:
            raise ValueError('Adjusted value must be a positive integer')

        # Subtract from the adjusted number
        self._adjusted_loops -= adjustment

        # Should the repeat be stopped?
        if (
            self.loops_remaining <= 0 and self.status is RepeatStatus.RUNNING
        ):
            listeners_tick_logger.log_debug(
                'Repeat.reduce - Reduce caused repeat to stop'
            )
            self.stop()

    def _execute(self):
        """Execute the repeat's callback with its arguments and keywords."""
        listeners_tick_logger.log_debug('Repeat._execute')
        self._loops_elapsed += 1

        # Are any more loops to be made?
        if self.loops_remaining > 0:
            listeners_tick_logger.log_debug(
                'Repeat._execute - Remaining - {remaining}'.format(
                    remaining=self.loops_remaining
                )
            )

            # Call the delay again
            self._delay = Delay(
                self.interval, self._execute,
                cancel_on_level_end=self.cancel_on_level_end
            )

        else:
            listeners_tick_logger.log_debug(
                'Repeat._execute - Stopping the loop'
            )

            # Set the status to stopped
            self._status = RepeatStatus.STOPPED

        # Call the repeat's callback for this loop
        self.callback(*self.args, **self.kwargs)

    def _unload_instance(self):
        """Stop the repeat with being unloaded."""
        self.stop()


# =============================================================================
# >> HELPER FUNCTIONS
# =============================================================================
@OnLevelEnd
def _cancel_delays_on_level_end():
    for delay in list(_delay_manager):
        if not delay.cancel_on_level_end:
            continue

        callback = delay.callback
        if (
            callback.__name__ == '_execute' and
            hasattr(callback, '__self__') and
            isinstance(callback.__self__, Repeat)
        ):
            callback.__self__.stop()
        else:
            delay.cancel()
