# ../listeners/tick.py

"""Provides tick listener based functionality."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Python Imports
#   Contextlib
from contextlib import suppress
#   Enum
from enum import IntEnum
#   Threading
from threading import Thread
#   Time
import bisect
import time

# Source.Python Imports
#   Core
from core import AutoUnload
from core import WeakAutoUnload
#   Hooks
from hooks.exceptions import except_hooks
#   Listeners
from listeners import listeners_logger
from listeners import on_tick_listener_manager
from listeners import OnLevelEnd


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('Delay',
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
class GameThread(Thread):
    """Workaround for :class:`threading.Thread`."""

    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        on_tick_listener_manager.register_listener(self._tick)
        
    def __del__(self):
        on_tick_listener_manager.unregister_listener(self._tick)

    def _bootstrap_inner(self):
        try:
            super()._bootstrap_inner()
        finally:
            on_tick_listener_manager.unregister_listener(self._tick)

    def _tick(self):
        pass


# =============================================================================
# >> DELAY CLASSES
# =============================================================================
class _DelayManager(list):
    """A class that is responsible for executing delays."""

    def _tick(self):
        """Internal tick listener."""
        current_time = time.time()
        while self and self[0].exec_time <= current_time:
            try:
                self.pop(0).execute()
            except:
                except_hooks.print_exception()

        self._unregister_if_empty()

    def _register_if_empty(self):
        """Register the internal tick listener if the list is empty."""
        if not self:
            on_tick_listener_manager.register_listener(self._tick)

    def _unregister_if_empty(self):
        """Unregister the internal tick listener if the list is empty."""
        if not self:
            on_tick_listener_manager.unregister_listener(self._tick)

    def add(self, delay):
        """Add a delay to the list.

        :param Delay delay: The delay to add.
        """
        self._register_if_empty()
        bisect.insort_left(self, delay)

_delay_manager = _DelayManager()


class Delay(WeakAutoUnload):
    """Execute a callback after a given delay."""

    def __init__(
        self, delay, callback, args=(), kwargs=None, cancel_on_map_end=False
    ):
        """Initialize the delay.

        :param float delay: The delay in seconds.
        :param callback: A callable object that should be called after the
            delay expired.
        :param tuple args: Arguments that should be passed to the callback.
        :param dict kwargs: Keyword arguments that should be passed to the
            callback.
        :param bool cancel_on_map_end: Whether or not to cancel the delay at
            the end of the map.
        :raises ValueError: If the given callback is not callable.
        """
        if not callable(callback):
            raise ValueError('Given callback is not callable.')

        self.delay = delay
        self._start_time = time.time()
        self.exec_time = self._start_time + delay
        self.callback = callback
        self.args = args
        self.kwargs = kwargs if kwargs is not None else dict()
        self.cancel_on_map_end = cancel_on_map_end
        _delay_manager.add(self)

    def __lt__(self, other):
        """Return True if this :attr:`exec_time` is less than the other's."""
        return self.exec_time < other.exec_time

    def __call__(self):
        """Cancel the delay and immediately call the callback."""
        self.cancel()
        return self.execute()

    def execute(self):
        """Call the callback."""
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
        self, callback, args=(), kwargs=None, cancel_on_map_end=False
    ):
        """Store all instance attributes.

        :param callback: A callable object that should be called at the
            end of each loop.
        :param tuple args: Arguments that should be passed to the callback.
        :param dict kwargs: Keyword arguments that should be passed to the
            callback.
        :param bool cancel_on_map_end: Whether or not to cancel the repeat at
            the end of the map.
        """
        # Store the base attributes
        self.callback = callback
        self.args = args
        self.kwargs = kwargs if kwargs is not None else dict()
        self.cancel_on_map_end = cancel_on_map_end

        # Log the __init__ message
        listeners_tick_logger.log_debug(
            'Repeat.__init__: <{0}> <{1}> <{2}>'.format(
                self.callback, self.args, self.kwargs))

        # Set up private attributes
        self._interval = 0
        self._limit = 0
        self._count = 0
        self._adjusted = 0
        self._status = RepeatStatus.STOPPED
        self._delay = None
        self._loop_time = None

    def start(self, interval, limit, execute_on_start=False):
        """Start the repeat loop.

        :param float interval: The time (in seconds) for each loop.
        :param int limit: The maximum number of times to loop.  If 0 is
            passed, there is no limit, and the Repeat will loop indefinitely.
        :param bool execute_on_start: Whether to execute the callback when
            the Repeat is started.  Note that this does not affect the 'limit'
            as the number of loops will remain the same.
        """
        # Log the start message
        listeners_tick_logger.log_debug(
            'Repeat.start: <{0}> <{1}>'.format(interval, limit))

        # Is the repeat already running?
        if self._status is RepeatStatus.RUNNING:

            # Log the status
            listeners_tick_logger.log_debug(
                'Repeat.start - RepeatStatus.RUNNING')

            # Do not start the repeat
            return

        # Log starting the repeat
        listeners_tick_logger.log_debug(
            'Repeat.start - !RepeatStatus' +
            '.RUNNING - Starting Repeat')

        # Set the status to running
        self._status = RepeatStatus.RUNNING

        # Set the given attributes
        self._interval = interval
        self._limit = limit

        # Reset base counting attributes
        self._count = 0
        self._adjusted = 0

        # Start the delay
        self._delay = Delay(
            self._interval, self._execute,
            cancel_on_map_end=self.cancel_on_map_end
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
                'Repeat.stop - !RepeatStatus.RUNNING')

            # No need to stop it
            return

        # Log stopping the repeat
        listeners_tick_logger.log_debug(
            'Repeat.stop - RepeatStatus.RUNNING - Stopping Repeat')

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
        self.start(self._interval, self._limit)

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
                'Repeat.pause - !RepeatStatus.RUNNING')

            # No need to pause
            return

        # Log pausing the repeat
        listeners_tick_logger.log_debug(
            'Repeat.pause - RepeatStatus.RUNNING - Pausing Repeat')

        # Set the status to paused
        self._status = RepeatStatus.PAUSED

        # Set the remaining time in the current loop
        self._loop_time = self._delay.exec_time - time.time()

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
                'Repeat.resume - !RepeatStatus.PAUSED')

            # Do not resume
            return

        # Log resuming the repeat
        listeners_tick_logger.log_debug(
            'Repeat.resume - RepeatStatus.' +
            'PAUSED - Resuming Repeat')

        # Set the status to running
        self._status = RepeatStatus.RUNNING

        # Start the delay
        self._delay = Delay(
            self._loop_time, self._execute,
            cancel_on_map_end=self.cancel_on_map_end
        )

    def extend(self, adjustment):
        """Add to the number of loops to be made.

        :param int adjustment: The number of loops to be added to the limit.
        :raises ValueError: If given adjustment is not a positive integer.
        """
        # Log the extend message
        listeners_tick_logger.log_debug('Repeat.extend')

        # Is there a limit for this repeat?
        if not self.loop_limit:

            # Log a message about no reducing
            listeners_tick_logger.log_debug(
                'Unable to extend, Repeat instance has no limit.')

            # No need to go further
            return

        # Was a positive integer given?
        if not isinstance(adjustment, int) or adjustment < 1:

            # Raise an error
            raise ValueError('Adjusted value must be a positive integer')

        # Add to the adjusted number
        self._adjusted += adjustment

    def reduce(self, adjustment):
        """Reduce the number of loops to be made.

        :param int adjustment: The number of loops to be removed from
            the limit.
        :raises ValueError: If given adjustment is not a positive integer.
        """
        # Log the reduce message
        listeners_tick_logger.log_debug('Repeat.reduce')

        # Is there a limit for this repeat?
        if not self.loop_limit:

            # Log a message about no reducing
            listeners_tick_logger.log_debug(
                'Unable to reduce, Repeat instance has no limit.')

            # No need to go further
            return

        # Was a positive integer given?
        if not isinstance(adjustment, int) or adjustment < 1:

            # Raise an error
            raise ValueError('Adjusted value must be a positive integer')

        # Subtract from the adjusted number
        self._adjusted -= adjustment

        # Are no more loops to be made?
        if (self.remaining_loops <= 0 and
                self.status is RepeatStatus.RUNNING):

            # Log the reduce-stopping message
            listeners_tick_logger.log_debug(
                'Repeat.reduce - Reduce caused repeat to stop')

            # Stop the repeat
            self.stop()

    def _execute(self):
        """Execute the repeat's callback with its arguments and keywords."""
        # Log the _execute message
        listeners_tick_logger.log_debug('Repeat._execute')

        # Add one to the current count
        self._count += 1

        # Are any more loops to be made?
        if self.remaining_loops or not self._limit:

            # Is there no limit?
            if not self._limit:

                # Log continuing the loop
                listeners_tick_logger.log_debug(
                    'Repeat._execute - No limit')

            # Is there a limit?
            else:

                # Log continuing the loop
                listeners_tick_logger.log_debug(
                    'Repeat._execute - Remaining - {0}'.format(
                        self.remaining_loops))

            # Call the delay again
            self._delay = Delay(
                self._interval, self._execute,
                cancel_on_map_end=self.cancel_on_map_end
            )

        # Are no more loops to be made?
        else:

            # Log stopping the repeat
            listeners_tick_logger.log_debug(
                'Repeat._execute - Stopping the loop')

            # Set the status to stopped
            self._status = RepeatStatus.STOPPED

        # Call the repeat's callback
        self.callback(*self.args, **self.kwargs)

    @property
    def remaining_loops(self):
        """Return the remaining number of loops in the repeat.

        :rtype: int
        """
        # Is there no limit?
        if not self._limit:

            # Return the limit
            return self._limit

        # Return the remaining number of loops
        return self.loop_limit - self._count

    @property
    def loop_count(self):
        """Return the current number of loops made in the repeat.

        :rtype: int
        """
        return self._count

    @property
    def loop_limit(self):
        """Return the total number of loops to be made.

        :rtype: int
        """
        # Is there no limit?
        if not self._limit:

            # Return the limit
            return self._limit

        # Return the adjusted limit
        return self._limit + self._adjusted

    @property
    def time_left(self):
        """Return the remaining time till the end of the repeat.

        :rtype: float
        """
        return (
            self.remaining_loops * self._interval +
            self.delay_time_remaining
        )

    @property
    def time_elapsed(self):
        """Return the elapsed time since the repeat started.

        :rtype: float
        """
        return self.total_time - self.time_left

    @property
    def total_time(self):
        """Return the total time it will take to complete the repeat.

        :rtype: float
        """
        return self.loop_limit * self._interval

    @property
    def status(self):
        """Return the status of the repeat.

        :rtype: RepeatStatus
        """
        return self._status

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

    def _unload_instance(self):
        """Stop the repeat with being unloaded."""
        self.stop()


# =============================================================================
# >> HELPER FUNCTIONS
# =============================================================================
@OnLevelEnd
def _cancel_delays_on_level_end():
    for delay in _delay_manager:
        if delay.cancel_on_map_end:
            delay.cancel()
