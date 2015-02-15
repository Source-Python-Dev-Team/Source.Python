# ../listeners/tick/delays.py

"""Provides delay functionality using a tick listener."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Python Imports
#   Time
import time

# Source.Python Imports
#   Hooks
from hooks.exceptions import except_hooks
#   Listeners
from listeners import tick_listener_manager
from listeners import listeners_logger


# =============================================================================
# >> GLOBAL VARIABLES
# =============================================================================
# Get the sp.tick.delays logger
listeners_tick_delays_logger = listeners_logger.tick.delays


# =============================================================================
# >> CLASSES
# =============================================================================
class Delay(object):

    """Stores a callback to be called at a later time."""

    def __init__(self, seconds, callback, *args, **kwargs):
        """Called when an instance is instantiated."""
        # Log the init message
        listeners_tick_delays_logger.log_debug(
            'Delay.__init__ <{0}> <{1}> <{2}> <{3}>'.format(
                seconds, callback, args, kwargs))

        # Store the time to execute the callback
        self._exec_time = time.time() + seconds

        # Store the callback, arguments, and keywords
        self.callback = callback
        self.args = args
        self.kwargs = kwargs

    def __call__(self):
        """Call the delay with the proper arguments and keywords."""
        # Log the call message
        listeners_tick_delays_logger.log_debug(
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
        self._order = list()

    def __missing__(self, item):
        """Called when first adding a time to the dictionary."""
        # Log the missing message
        listeners_tick_delays_logger.log_debug(
            'tick_delays.__missing__ <{0}>'.format(item))

        # Is the tick listener registered?
        if not self:

            # Log the tick listener registration message
            listeners_tick_delays_logger.log_debug(
                'tick_delays - Registering Tick Listener')

            # Register the tick listener
            tick_listener_manager.register_listener(self._tick)

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
        listeners_tick_delays_logger.log_debug(
            'tick_delays.__delitem__ <{0}>'.format(item))

        # Is the item in the dictionary?
        if item not in self:

            # Log the not in self message
            listeners_tick_delays_logger.log_debug(
                'tick_delays.__delitem__ - Item not in dictionary')

            # If not, simply return
            return

        # Call all delays for the given item
        self[item].call_delays()

        # Remove the item from the ordered list
        self._order.remove(item)

        # Remove the item from the dictionary
        super(_TickDelays, self).__delitem__(item)

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
            listeners_tick_delays_logger.log_debug(
                'tick_delays._tick - Unregistering Tick Listener')

            # Unregister the tick listener
            tick_listener_manager.unregister_listener(self._tick)

    def cancel_delay(self, delay_object):
        """Cancel a delay."""
        # Log the canceling message
        listeners_tick_delays_logger.log_debug(
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
        listeners_tick_delays_logger.log_debug(
            'tick_delays.cancel_delay - Removing from '
            '<{0}>'.format(delay_object._exec_time))

        # Remove the delay from its time
        self[delay_object._exec_time].remove(delay_object)

        # Does the delay's time have any remaining objects?
        if not self[delay_object._exec_time]:

            # Log the deletion of the time from the dictionary message
            listeners_tick_delays_logger.log_debug(
                'tick_delays.cancel_delay - Removing <{0}> '
                'from dictionary'.format(delay_object._exec_time))

            # If not, remove the delay's time from the dictionary
            del self[delay_object._exec_time]

        # Are there any remaining delays?
        if not self:

            # Log the tick listener unregistering message
            listeners_tick_delays_logger.log_debug(
                'tick_delays.cancel_delay - Unregistering Tick Listener')

            # Unregister the listener
            tick_listener_manager.unregister_listener(self._tick)

# Get the _TickDelays instance
tick_delays = _TickDelays()
