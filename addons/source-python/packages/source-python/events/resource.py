# ../events/resource.py

"""Provides a way to create resource (.res) files using custom events."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Python Imports
#   Collections
from collections import OrderedDict

# Source.Python Imports
#   Events
from events import events_logger
from events.custom import CustomEvent
from events.manager import game_event_manager
#   Paths
from paths import EVENT_PATH


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('ResourceFile',
           )


# =============================================================================
# >> GLOBAL VARIABLES
# =============================================================================
# Get the sp.events.resource logger
events_resource_logger = events_logger.resource


# =============================================================================
# >> CLASSES
# =============================================================================
class ResourceFile(OrderedDict):
    """Class used to write and load custom event .res files."""

    def __init__(self, filepath, *events):
        """Called on instatiation."""
        # Create the OrderedDict instance
        super().__init__()

        # Does the given path end with the extension?
        if filepath.endswith('.res'):

            # Remove the extension
            filepath = filepath[:~3]

        # Store the filepath
        self._filepath = filepath

        # Loop through all given events
        for event in events:

            # Is the given class not a CustomEvent class?
            if not issubclass(event, CustomEvent):

                # Clear the dictionary
                self.clear()

                # Raise an error
                raise TypeError(
                    'Event classes must be sub-classes of CustomEvent')

            # Store the event with its variables in the dictionary
            self[event.__name__.lower()] = event

    @property
    def filepath(self):
        """Return the path to the .res file."""
        return self._filepath

    @property
    def fullpath(self):
        """Return the full path of the .res file."""
        return EVENT_PATH / self.filepath + '.res'

    def write(self):
        """Write the .res file to disk."""
        # Does the director for the .res file exist?
        if not self.fullpath.parent.isdir():

            # Create the directory
            self.fullpath.parent.makedirs()

        # Open the .res file to write to it
        with self.fullpath.open('w') as open_file:

            # Write the .res file's name
            open_file.write('"{0}"\n'.format(self.filepath))

            # Create a group of events for the file
            open_file.write('{\n')

            # Loop through all events
            for event in self:

                # Write the event
                open_file.write('    "{0}"\n'.format(event))

                # Create a group of variables for the event
                open_file.write('    {\n')

                # Loop through the event's variables
                for variable in self[event]._odict:

                    # Get the variable's instance
                    instance = self[event]._odict[variable]

                    # Get the variable's comment
                    comment = '\t// {0}'.format(
                        instance._comment) if instance._comment else ''

                    # Write the variable with its type and comment
                    open_file.write('        "{0}"\t"{1}"{2}\n'.format(
                        variable, instance.name, comment))

                # End the group of variables
                open_file.write('    }\n')

            # End the group of events
            open_file.write('}\n')

    def load_events(self):
        """Load events from the .res file."""
        game_event_manager.load_events_from_file(self.fullpath)
