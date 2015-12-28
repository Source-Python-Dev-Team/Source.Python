# ../listeners/tick.py

"""Provides tick listener based functionality."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Python Imports
#   Enum
from enum import IntEnum
#   Time
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
           '_TickDelays',
           'tick_delays',
           )


# =============================================================================
# >> GLOBAL VARIABLES
# =============================================================================
# Get the sp.listeners.tick logger
listeners_tick_logger = listeners_logger.tick


# =============================================================================
# >> DELAY CLASSES
# =============================================================================
class Delay(object):
    """Stores a callback to be called at a later time."""

    def __init__(self, seconds, callback, *args, **kwargs):
        """Called when an instance is instantiated."""
        # Log the init message
        listeners_tick_logger.log_debug(
            'Delay.__init__ <{0}> <{1}> <{2}> <{3}>'.format(
                seconds, callback, args, kwargs))

        if not callable(callback):
            raise ValueError('Given callback is not callable.')

        # Store the time to execute the callback
        self._exec_time = time.time() + seconds

        # Store the callback, arguments, and keywords
        self.callback = callback
        self.args = args
        self.kwargs = kwargs

    def __call__(self):
        """Call the delay with the proper arguments and keywords."""
        # Log the call message
        listeners_tick_logger.log_debug(
            'Delay.__call__ - Try to call - <{0}> <{1}> <{2}>'.format(
                self.callback, self.args, self.kwargs))

        # Use try/except in case an error is encountered
        try:

            # Execute the callback with the arguments and keywords
            self.callback(*self.args, **self.kwargs)

        # Was an error encountered?
        except:

            # Print the exception to the console
            except_hooks.print_exception()

    @property
    def exec_time(self):
        """Return the time to execute the delayed function."""
        return self._exec_time

    def cancel(self):
        """Cancel the delay."""
        tick_delays.cancel_delay(self)


class _Times(list):
    """List class used to store delays to be called."""

    def call_delays(self):
        """Call the delays in the list."""
        # Loop through the delays in the list
        for item in self:

            # Call the delay
            item()


class _TickDelays(dict):
    """Class used to store delays to be called by a tick listener."""

    def __init__(self):
        """Store an ordered list to sort delays."""
        super().__init__()
        self._order = list()

    def __missing__(self, item):
        """Called when first adding a time to the dictionary."""
        # Log the missing message
        listeners_tick_logger.log_debug(
            'tick_delays.__missing__ <{0}>'.format(item))

        # Is the tick listener registered?
        if not self:

            # Log the tick listener registration message
            listeners_tick_logger.log_debug(
                'tick_delays - Registering Tick Listener')

            # Register the tick listener
            on_tick_listener_manager.register_listener(self._tick)

        # Add the item to the dictionary as a _Times instance
        self[item] = _Times()

        # Add the time to the ordered list
        self._order.append(item)

        # Sort the ordered list
        self._order.sort()

        # Return the item's instance
        return self[item]

    def __iter__(self):
        """Loop through the ordered list."""
        # Loop through each item in the ordered list
        for item in self._order:

            # Yield the item
            yield item

    def __delitem__(self, item):
        """Call the delays and remove the time from the ordered list."""
        # Log the delitem message
        listeners_tick_logger.log_debug(
            'tick_delays.__delitem__ <{0}>'.format(item))

        # Is the item in the dictionary?
        if item not in self:

            # Log the not in self message
            listeners_tick_logger.log_debug(
                'tick_delays.__delitem__ - Item not in dictionary')

            # If not, simply return
            return

        # Call all delays for the given item
        self[item].call_delays()

        # Remove the item from the ordered list
        self._order.remove(item)

        # Remove the item from the dictionary
        super().__delitem__(item)

    def delay(self, seconds, callback, *args, **kwargs):
        """Create a delay."""
        # Get the Delay instance for the given arguments
        delay_object = Delay(seconds, callback, *args, **kwargs)

        # Add the Delay instance to the dictionary using its execution time
        self[delay_object._exec_time].append(delay_object)

        # Return the object
        return delay_object

    def _tick(self):
        """Called every tick when the listener is registered."""
        # Get the current time
        current_time = time.time()

        # Loop through each item in the ordered list
        for item in self:

            # Should the delays be called?
            if item > current_time:

                # If not, no need to continue looping
                break

            # Remove the item from the dictionary
            del self[item]

        # Is the dictionary now empty?
        if not self:

            # Log the tick listener unregistering message
            listeners_tick_logger.log_debug(
                'tick_delays._tick - Unregistering Tick Listener')

            # Unregister the tick listener
            on_tick_listener_manager.unregister_listener(self._tick)

    def cancel_delay(self, delay_object):
        """Cancel a delay."""
        # Log the canceling message
        listeners_tick_logger.log_debug(
            'tick_delays.cancel_delay <{0}>'.format(delay_object))

        # Is the given argument a Delay object?
        if not isinstance(delay_object, Delay):

            # If not, raise an error
            raise TypeError(
                'tick_delays.cancel_delay requires a Delay instance.')

        # Is the given Delay object's time no longer in the dictionary?
        if delay_object._exec_time not in self:

            # If not, raise an error
            raise KeyError('Object is no longer registered.')

        # Log the removing from list message
        listeners_tick_logger.log_debug(
            'tick_delays.cancel_delay - Removing from '
            '<{0}>'.format(delay_object._exec_time))

        # Remove the delay from its time
        self[delay_object._exec_time].remove(delay_object)

        # Does the delay's time have any remaining objects?
        if not self[delay_object._exec_time]:

            # Log the deletion of the time from the dictionary message
            listeners_tick_logger.log_debug(
                'tick_delays.cancel_delay - Removing <{0}> '
                'from dictionary'.format(delay_object._exec_time))

            # If not, remove the delay's time from the dictionary
            del self[delay_object._exec_time]

        # Are there any remaining delays?
        if not self:

            # Log the tick listener unregistering message
            listeners_tick_logger.log_debug(
                'tick_delays.cancel_delay - Unregistering Tick Listener')

            # Unregister the listener
            on_tick_listener_manager.unregister_listener(self._tick)

# The singleton object of the :class:`_TickDelays` class
tick_delays = _TickDelays()


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
        self._delay = tick_delays.delay(self._interval, self._execute)

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
        tick_delays.cancel_delay(self._delay)

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
        tick_delays.cancel_delay(self._delay)

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
        self._delay = tick_delays.delay(self._loop_time, self._execute)

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
            self._delay = tick_delays.delay(self._interval, self._execute)

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
