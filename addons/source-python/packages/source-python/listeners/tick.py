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
#   Time
import bisect
import time

# Source.Python Imports
#   Core
from core import AutoUnload
#   Hooks
from hooks.exceptions import except_hooks
#   Listeners
from listeners import listeners_logger
from listeners import on_tick_listener_manager


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('Delay',
           'TickRepeat',
           'TickRepeatStatus',
           )


# =============================================================================
# >> GLOBAL VARIABLES
# =============================================================================
# Get the sp.listeners.tick logger
listeners_tick_logger = listeners_logger.tick


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


class Delay(AutoUnload):
    """Execute a callback after a given delay."""

    def __init__(self, delay, callback, *args, **kwargs):
        """Initialize the delay.

        :param int delay: The delay in seconds.
        :param callback: A callable object that should be called after the
            delay expired.
        :param args: Arguments that should be passed to the callback.
        :param kwargs: Keyword arguments that should be passed to the
            callback.
        """
        if not callable(callback):
            raise ValueError('Given callback is not callable.')

        self.delay = delay
        self.exec_time = time.time() + delay
        self.callback = callback
        self.args = args
        self.kwargs = kwargs
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
        """Return True if the delay running."""
        return self in _delay_manager

    def _unload_instance(self):
        with suppress(ValueError):
            self.cancel()


# =============================================================================
# >> REPEAT CLASSES
# =============================================================================
class TickRepeatStatus(IntEnum):
    """Class used to store TickRepeatStatus values."""

    STOPPED = 1
    RUNNING = 2
    PAUSED = 3


class TickRepeat(AutoUnload):
    """Class used to create and call repeats."""

    def __init__(self, callback, *args, **kwargs):
        """Store all instance attributes."""
        # Store the base attributes
        self.callback = callback
        self.args = args
        self.kwargs = kwargs

        # Log the __init__ message
        listeners_tick_logger.log_debug(
            'TickRepeat.__init__: <{0}> <{1}> <{2}>'.format(
                self.callback, self.args, self.kwargs))

        # Set up private attributes
        self._interval = 0
        self._limit = 0
        self._count = 0
        self._adjusted = 0
        self._status = TickRepeatStatus.STOPPED
        self._delay = None
        self._loop_time = None

    def start(self, interval, limit):
        """Start the repeat loop."""
        # Log the start message
        listeners_tick_logger.log_debug(
            'TickRepeat.start: <{0}> <{1}>'.format(interval, limit))

        # Is the repeat already running?
        if self._status is TickRepeatStatus.RUNNING:

            # Log the status
            listeners_tick_logger.log_debug(
                'TickRepeat.start - TickRepeatStatus.RUNNING')

            # Do not start the repeat
            return

        # Log starting the repeat
        listeners_tick_logger.log_debug(
            'TickRepeat.start - !TickRepeatStatus' +
            '.RUNNING - Starting TickRepeat')

        # Set the status to running
        self._status = TickRepeatStatus.RUNNING

        # Set the given attributes
        self._interval = interval
        self._limit = limit

        # Reset base counting attributes
        self._count = 0
        self._adjusted = 0

        # Start the delay
        self._delay = Delay(self._interval, self._execute)

    def stop(self):
        """Stop the repeat loop."""
        # Log the stop message
        listeners_tick_logger.log_debug('TickRepeat.stop')

        # Is the repeat running?
        if self._status is not TickRepeatStatus.RUNNING:

            # Log the status
            listeners_tick_logger.log_debug(
                'TickRepeat.stop - !TickRepeatStatus.RUNNING')

            # No need to stop it
            return

        # Log stopping the repeat
        listeners_tick_logger.log_debug(
            'TickRepeat.stop - TickRepeatStatus.RUNNING - Stopping TickRepeat')

        # Set the status to stopped
        self._status = TickRepeatStatus.STOPPED

        # Cancel the delay
        self._delay.cancel()

    def restart(self):
        """Restart the repeat."""
        # Log restarting the repeat
        listeners_tick_logger.log_debug('TickRepeat.restart')

        # Stop the repeat
        self.stop()

        # Start the repeat
        self.start(self._interval, self._limit)

    def pause(self):
        """Pause the repeat.

        Pausing allows the repeat to be resumed.
        """
        # Log the pause message
        listeners_tick_logger.log_debug('TickRepeat.pause')

        # Is the repeat running?
        if self._status is not TickRepeatStatus.RUNNING:

            # Log the status
            listeners_tick_logger.log_debug(
                'TickRepeat.pause - !TickRepeatStatus.RUNNING')

            # No need to pause
            return

        # Log pausing the repeat
        listeners_tick_logger.log_debug(
            'TickRepeat.pause - TickRepeatStatus.RUNNING - Pausing TickRepeat')

        # Set the status to paused
        self._status = TickRepeatStatus.PAUSED

        # Set the remaining time in the current loop
        self._loop_time = self._delay.exec_time - time.time()

        # Cancel the delay
        self._delay.cancel()

    def resume(self):
        """Resume the repeat.

        Can only resume if in paused status.
        """
        # Log the resume message
        listeners_tick_logger.log_debug('TickRepeat.resume')

        # Is the repeat paused?
        if self._status is not TickRepeatStatus.PAUSED:

            # Log the status
            listeners_tick_logger.log_debug(
                'TickRepeat.resume - !TickRepeatStatus.PAUSED')

            # Do not resume
            return

        # Log resuming the repeat
        listeners_tick_logger.log_debug(
            'TickRepeat.resume - TickRepeatStatus.' +
            'PAUSED - Resuming TickRepeat')

        # Set the status to running
        self._status = TickRepeatStatus.RUNNING

        # Start the delay
        self._delay = Delay(self._loop_time, self._execute)

    def extend(self, adjustment):
        """Add to the number of loops to be made."""
        # Log the extend message
        listeners_tick_logger.log_debug('TickRepeat.extend')

        # Is there a limit for this repeat?
        if not self.limit:

            # Log a message about no reducing
            listeners_tick_logger.log_debug(
                'Unable to extend, TickRepeat instance has no limit.')

            # No need to go further
            return

        # Was a positive integer given?
        if adjustment < 1 or not isinstance(adjustment, int):

            # Raise an error
            raise ValueError('Adjusted value must be a positive integer')

        # Add to the adjusted number
        self._adjusted += adjustment

    def reduce(self, adjustment):
        """Reduce the number of loops to be made."""
        # Log the reduce message
        listeners_tick_logger.log_debug('TickRepeat.reduce')

        # Is there a limit for this repeat?
        if not self.limit:

            # Log a message about no reducing
            listeners_tick_logger.log_debug(
                'Unable to reduce, TickRepeat instance has no limit.')

            # No need to go further
            return

        # Was a positive integer given?
        if adjustment < 1 or not isinstance(adjustment, int):

            # Raise an error
            raise ValueError('Adjusted value must be a positive integer')

        # Subtract from the adjusted number
        self._adjusted -= adjustment

        # Are no more loops to be made?
        if (self.remaining <= 0 and
                self.status is TickRepeatStatus.RUNNING):

            # Log the reduce-stopping message
            listeners_tick_logger.log_debug(
                'TickRepeat.reduce - Reduce caused repeat to stop')

            # Stop the repeat
            self.stop()

    def _execute(self):
        """Execute the repeat's callback with its arguments and keywords."""
        # Log the _execute message
        listeners_tick_logger.log_debug('TickRepeat._execute')

        # Add one to the current count
        self._count += 1

        # Are any more loops to be made?
        if self.remaining or not self._limit:

            # Is there no limit?
            if not self._limit:

                # Log continuing the loop
                listeners_tick_logger.log_debug(
                    'TickRepeat._execute - No limit')

            # Is there a limit?
            else:

                # Log continuing the loop
                listeners_tick_logger.log_debug(
                    'TickRepeat._execute - Remaining - {0}'.format(
                        self.remaining))

            # Call the delay again
            self._delay = Delay(self._interval, self._execute)

        # Are no more loops to be made?
        else:

            # Log stopping the repeat
            listeners_tick_logger.log_debug(
                'TickRepeat._execute - Stopping the loop')

            # Set the status to stopped
            self._status = TickRepeatStatus.STOPPED

        # Call the repeat's callback
        self.callback(*self.args, **self.kwargs)

    @property
    def remaining(self):
        """Return the remaining number of loops in the repeat."""
        # Is there no limit?
        if not self._limit:

            # Return the limit
            return self._limit

        # Return the remaining number of loops
        return self.limit - self._count

    @property
    def count(self):
        """Return the current number of loops made in the repeat."""
        return self._count

    @property
    def limit(self):
        """Return the total number of loops to be made."""
        # Is there no limit?
        if not self._limit:

            # Return the limit
            return self._limit

        # Return the adjusted limit
        return self._limit + self._adjusted

    @property
    def timeleft(self):
        """Return the remaining time till the end of the repeat."""
        return self.remaining * self._interval

    @property
    def elapsed(self):
        """Return the elapsed time since the repeat started."""
        return self._count * self._interval

    @property
    def total_time(self):
        """Return the total time it will take to complete the repeat."""
        return self.limit * self._interval

    @property
    def status(self):
        """Return the status of the repeat."""
        return self._status

    def _unload_instance(self):
        """Stop the repeat with being unloaded."""
        self.stop()
