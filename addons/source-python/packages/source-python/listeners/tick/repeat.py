# ../tick/repeat.py

# =============================================================================
# >> IMPORTS
# =============================================================================
# Python Imports
#   Time
import time

# Source.Python Imports
from core import AutoUnload
#   Tick
from listeners.tick import TickDelays
from listeners.tick.logger import TickLogger


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
# Add all the global variables to __all__
__all__ = [
    'TickRepeat',
    'TickRepeatStatus',
]


# =============================================================================
# >> GLOBAL VARIABLES
# =============================================================================
# Get the sp.tick.repeat logger
TickRepeatLogger = TickLogger.repeat


# =============================================================================
# >> CLASSES
# =============================================================================
class TickRepeatStatus(object):
    '''Class used to store TickRepeatStatus values'''
    STOPPED, RUNNING, PAUSED = range(1, 4)


class TickRepeat(AutoUnload):
    '''Class used to create and call repeats'''

    def __init__(self, callback, *args, **kwargs):
        '''Stores all attributes'''

        # Store the base attributes
        self.callback = callback
        self.args = args
        self.kwargs = kwargs

        # Log the __init__ message
        TickRepeatLogger.log_info(
            'TickRepeat.__init__: <{0}> <{1}> <{2}>'.format(
                self.callback, self.args, self.kwargs))

        # Set up private attributes
        self._interval = 0
        self._limit = 0
        self._count = 0
        self._adjusted = 0
        self._status = TickRepeatStatus.STOPPED

    def start(self, interval, limit):
        '''Starts the repeat loop'''

        # Log the start message
        TickRepeatLogger.log_info(
            'TickRepeat.start: <{0}> <{1}>'.format(interval, limit))

        # Is the repeat already running?
        if self._status == TickRepeatStatus.RUNNING:

            # Log the status
            TickRepeatLogger.log_info('TickRepeat.start - TickRepeatStatus.RUNNING')

            # Do not start the repeat
            return

        # Log starting the repeat
        TickRepeatLogger.log_info(
            'TickRepeat.start - !TickRepeatStatus.RUNNING - Starting TickRepeat')

        # Set the status to running
        self._status = TickRepeatStatus.RUNNING

        # Set the given attributes
        self._interval = interval
        self._limit = limit

        # Reset base counting attributes
        self._count = 0
        self._adjusted = 0

        # Start the delay
        self._delay = TickDelays.delay(self._interval, self._execute)

    def stop(self):
        '''Stops the repeat loop'''

        # Log the stop message
        TickRepeatLogger.log_info('TickRepeat.stop')

        # Is the repeat running?
        if self._status != TickRepeatStatus.RUNNING:

            # Log the status
            TickRepeatLogger.log_info('TickRepeat.stop - !TickRepeatStatus.RUNNING')

            # No need to stop it
            return

        # Log stopping the repeat
        TickRepeatLogger.log_info(
            'TickRepeat.stop - TickRepeatStatus.RUNNING - Stopping TickRepeat')

        # Set the status to stopped
        self._status = TickRepeatStatus.STOPPED

        # Cancel the delay
        TickDelays.cancel_delay(self._delay)

    def restart(self):
        '''Restarts the repeat'''

        # Log restarting the repeat
        TickRepeatLogger.log_info('TickRepeat.restart')

        # Stop the repeat
        self.stop()

        # Start the repeat
        self.start(self._interval, self._limit)

    def pause(self):
        '''Pauses the repeat.  Pausing allows the repeat to be resumed'''

        # Log the pause message
        TickRepeatLogger.log_info('TickRepeat.pause')

        # Is the repeat running?
        if self._status != TickRepeatStatus.RUNNING:

            # Log the status
            TickRepeatLogger.log_info('TickRepeat.pause - !TickRepeatStatus.RUNNING')

            # No need to pause
            return

        # Log pausing the repeat
        TickRepeatLogger.log_info(
            'TickRepeat.pause - TickRepeatStatus.RUNNING - Pausing TickRepeat')

        # Set the status to paused
        self._status = TickRepeatStatus.PAUSED

        # Set the remaining time in the current loop
        self._loop_time = self._delay.exec_time - time.time()

        # Cancel the delay
        TickDelays.cancel_delay(self._delay)

    def resume(self):
        '''Resume the repeat.  Can only resume if in paused status'''

        # Log the resume message
        TickRepeatLogger.log_info('TickRepeat.resume')

        # Is the repeat paused?
        if self._status != TickRepeatStatus.PAUSED:

            # Log the status
            TickRepeatLogger.log_info('TickRepeat.resume - !TickRepeatStatus.PAUSED')

            # Do not resume
            return

        # Log resuming the repeat
        TickRepeatLogger.log_info(
            'TickRepeat.resume - TickRepeatStatus.PAUSED - Resuming TickRepeat')

        # Start the delay
        self._delay = TickDelays.delay(self._loop_time, self._execute)

    def extend(self, adjustment):
        '''Adds to the number of loops to be made'''

        # Log the extend message
        TickRepeatLogger.log_info('TickRepeat.extend')

        # Was a positive integer given?
        if adjustment < 1 or not isinstance(adjustment, int):

            # Raise an error
            raise ValueError('Adjusted value must be a positive integer')

        # Add to the adjusted number
        self._adjusted += adjustment

    def reduce(self, adjustment):
        '''Reduces the number of loops to be made'''

        # Log the reduce message
        TickRepeatLogger.log_info('TickRepeat.reduce')

        # Was a positive integer given?
        if adjustment < 1 or not isinstance(adjustment, int):

            # Raise an error
            raise ValueError('Adjusted value must be a positive integer')

        # Subtract from the adjusted number
        self._adjusted -= adjustment

        # Are no more loops to be made?
        if (self.remaining and self._limit and
                self.status == TickRepeatStatus.RUNNING):

            # Log the reduce-stopping message
            TickRepeatLogger.log_info(
                'TickRepeat.reduce - Reduce caused repeat to stop')

            # Stop the repeat
            self.stop()

    def _execute(self):
        '''Executes the repeat's callback with its arguments and keywords'''

        # Log the _execute message
        TickRepeatLogger.log_info('TickRepeat._execute')

        # Add one to the current count
        self._count += 1

        # Are any more loops to be made?
        if self.remaining or not self._limit:

            # Is there no limit?
            if not self._limit:

                # Log continuing the loop
                TickRepeatLogger.log_info('TickRepeat._execute - No limit')

            # Is there a limit?
            else:

                # Log continuing the loop
                TickRepeatLogger.log_info(
                    'TickRepeat._execute - Remaining - {0}'.format(self.remaining))

            # Call the delay again
            self._delay = TickDelays.delay(self._interval, self._execute)

        # Are no more loops to be made?
        else:

            # Log stopping the repeat
            TickRepeatLogger.log_info('TickRepeat._execute - Stopping the loop')

            # Set the status to stopped
            self._status = TickRepeatStatus.STOPPED

        # Call the repeat's callback
        self.callback(*self.args, **self.kwargs)

    @property
    def remaining(self):
        '''Returns the remaining number of loops in the repeat'''

        # Is there no limit?
        if not self._limit:

            # Return the limit
            return self._limit

        # Return the remaining number of loops
        return self.limit - self._count

    @property
    def count(self):
        '''Returns the current number of loops made in the repeat'''
        return self._count

    @property
    def limit(self):
        '''Returns the total number of loops to be made'''

        # Is there no limit?
        if not self._limit:

            # Return the limit
            return self._limit

        # Return the adjusted limit
        return self._limit + self._adjusted

    @property
    def timeleft(self):
        '''Returns the remaining time till the end of the repeat'''
        return self.remaining * self._interval

    @property
    def elapsed(self):
        '''Returns the elapsed time since the repeat started'''
        return self._count * self._interval

    @property
    def total_time(self):
        '''Returns the total time it will take to complete the repeat'''
        return self.limit * self._interval

    @property
    def status(self):
        '''Returns the status of the repeat'''
        return self._status

    def _unload_instance(self):
        '''Stops the repeat'''
        self.stop()
