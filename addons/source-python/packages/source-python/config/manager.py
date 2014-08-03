# ../config/manager.py

# =============================================================================
# >> IMPORTS
# =============================================================================
# Python Imports
#   Collections
from collections import defaultdict
#   TextWrap
from textwrap import TextWrapper

# Source.Python Imports
#   Config
from config.cvar import _CvarManager
from config.section import _SectionManager
from config.command import _CommandManager
#   Engines
from engines.server import EngineServer
#   Hooks
from hooks.exceptions import ExceptHooks
#   Paths
from paths import CFG_PATH
#   Translations
from translations.strings import LangStrings
from translations.strings import TranslationStrings


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('ConfigManager',
           )


# =============================================================================
# >> GLOBAL VARIABLES
# =============================================================================
# Get the config language strings
_config_strings = LangStrings('_core/config_strings')


# =============================================================================
# >> CLASSES
# =============================================================================
class ConfigManager(object):
    '''Config Management class used to create a config file'''

    def __init__(self, filepath, indention=3, max_line_length=79):
        '''Called on instantiation'''

        # Does the filepath contain the extension?
        if filepath.endswith('.cfg'):

            # Remove the extension from the filepath
            filepath = filepath[:~3]

        # Store the primary attributes
        self._filepath = filepath
        self._indention = indention
        self._max_line_length = max_line_length

        # Store the header and separator
        self.header = ''
        self.separator = '#'

        # Store the section types
        self._cvars = set()
        self._commands = set()

        # Store the section list
        self._sections = list()

    def __enter__(self):
        '''Used when using "with" context management to create the file'''
        return self

    @property
    def filepath(self):
        '''Returns the file path for the config file'''
        return self._filepath

    @property
    def indention(self):
        '''Returns the indention value for the config file'''
        return self._indention

    @property
    def max_line_length(self):
        '''Returns the max line length for the config file'''
        return self._max_line_length

    @property
    def fullpath(self):
        '''Returns the "path" instance of the full path to the file'''
        return CFG_PATH.joinpath(self.filepath + '.cfg')

    def cvar(
            self, name, default='0', flags=0,
            description='', min_value=None, max_value=None):
        '''Adds/returns a cvar instance to add to the config file'''

        # Get the _CvarManager instance for the given arguments
        section = _CvarManager(
            name, default, flags, description, min_value, max_value)

        # Add the cvar to the list of cvars
        self._cvars.add(name)

        # Add the _CvarManager instance to the list of sections
        self._sections.append(section)

        # Return the _CvarManager instance
        return section

    def section(self, name, separator='#'):
        '''Adds/returns a section instance to add to the config file'''

        # Get the _SectionManager instance for the given arguments
        section = _SectionManager(name, separator)

        # Add the _SectionManager instance to the list of sections
        self._sections.append(section)

        # Return the _SectionManager instance
        return section

    def command(self, name, description=''):
        '''Adds/returns a command instance to add to the config file'''

        # Get the _CommandManager instance for the given arguments
        section = _CommandManager(name, description)

        # Add the command to the list of commands
        self._commands.add(name)

        # Add the _CommandManager instance to the list of sections
        self._sections.append(section)

        # Return the _CommandManager instance
        return section

    def text(self, text):
        '''Adds text to the config file'''

        # Is the given text a TranslationStrings instance?
        if isinstance(text, TranslationStrings):

            # Get the proper language string
            text = text.get_string()

        # Add the text
        self._sections.append(text)

    def __exit__(self, exctype, value, trace_back):
        '''Used when exiting "with" context management to create file'''

        # Was an exception raised?
        if trace_back:

            # Print the exception
            ExceptHooks.print_exception(exctype, value, trace_back)

            # Return
            return False

        # Write the file
        self.write()

        # Execute the file
        self.execute()

        # Return
        return True

    def write(self):
        '''Writes the config file'''

        # Get any old values from the existing file
        _old_config = self._parse_old_file()

        # Is the indention too small?
        if self.indention < 3:

            # Set the indention to the lowest amount
            self._indention = 3

        # Do all directories to the file exist?
        if not self.fullpath.parent.isdir():

            # Create the directories
            self.fullpath.parent.makedirs()

        # Open/close the file to write to it
        with self.fullpath.open('w') as open_file:

            # Get the number of spaces to indent after //
            spaces = ' ' * (self.indention - 2)

            # Is there a header for the file?
            if self.header:

                # Get the length of the header's separator
                length = len(self.separator)

                # Get the number of times to repeat the separator
                times, remainder = divmod(
                    self.max_line_length - (2 * self.indention), length)

                # Get the string separator value
                separator = (
                    '//' + spaces + self.separator * times +
                    self.separator[:remainder] + spaces + '//\n')

                # Write the separator
                open_file.write(separator)

                # Is the header a TranslationStrings instance?
                if isinstance(self.header, TranslationStrings):

                    # Get the proper text for the header
                    self.header = self.header.get_string()

                # Loop through each line in the header
                for lines in self.header.splitlines():

                    # Loop through the current line to get valid
                    # lines with length less than the max line length
                    for line in self._get_lines(lines):

                        # Strip the // and new line characters from the line
                        line = line.lstrip('/ ').replace('\n', '')

                        # Write the current line
                        open_file.write('//{0}//\n'.format(
                            line.center(self.max_line_length - 4)))

                # Write the separator to end the header
                open_file.write(separator)

            # Loop through all sections in the file
            for section in self._sections:

                # Is the current section a Cvar?
                if isinstance(section, _CvarManager):

                    # Has any text been added to the file?
                    if open_file.tell():

                        # If so, add a blank line prior to section
                        open_file.write('\n')

                    # Loop through all lines in the section
                    for lines, indent in section:

                        # Loop through the current line to get valid
                        # lines with length less than the max line length
                        for line in self._get_lines(lines, indent):

                            # Write the current line
                            open_file.write(line + '\n')

                    # Does the default value need written?
                    if section.show_default:

                        # Write the cvar's default value
                        open_file.write(
                            '//' + spaces +
                            _config_strings['Default'].get_string() +
                            ' {0}\n'.format(section.default))

                    # Loop through the description to get valid
                    # lines with length less than the max line length
                    for line in self._get_lines(section.description):

                        # Write the current line
                        open_file.write(line + '\n')

                    # Does the cvar exist in the old config file?
                    if section.name in _old_config:

                        # Write the old config file's value
                        open_file.write(
                            ' ' * self.indention +
                            _old_config[section.name][0] + '\n\n\n')

                        # Remove the cvar from the old config file dictionary
                        del _old_config[section.name]

                    # Does the cvar not exist in the old config file
                    # and the value is a float or integer?
                    elif isinstance(section.default, (float, int)):

                        # Write the cvar line using the default value
                        open_file.write(
                            ' ' * self.indention + section.name +
                            ' {0}\n\n\n'.format(section.default))

                    # Does the cvar not exist in the old config file
                    # And the value is not a float or integer?
                    else:

                        # Write the cvar line using the default
                        # value with quotes around the value
                        open_file.write(
                            ' ' * self.indention + section.name +
                            ' "{0}"\n\n\n'.format(section.default))

                # Is the current section a Section?
                elif isinstance(section, _SectionManager):

                    # Has any text been added to the file?
                    if open_file.tell():

                        # If so, add a blank line prior to section
                        open_file.write('\n')

                    # Get the length of the section's separator
                    length = len(section.separator)

                    # Get the number of times to repeat the separator
                    times, remainder = divmod(
                        self.max_line_length - (2 * self.indention), length)

                    # Get the string separator value
                    separator = (
                        '//' + spaces + section.separator * times +
                        section.separator[:remainder] + spaces + '//\n')

                    # Write the separator
                    open_file.write(separator)

                    # Loop through each line in the section
                    for lines in section.name.splitlines():

                        # Loop through the current line to get valid
                        # lines with length less than the max line length
                        for line in self._get_lines(lines):

                            # Strip the // from the line and remove newline
                            line = line.lstrip('/ ').replace('\n', '')

                            # Write the current line
                            open_file.write('//{0}//\n'.format(
                                line.center(self.max_line_length - 4)))

                    # Write the separator to end the section
                    open_file.write(separator)

                # Is the current section a Command?
                elif isinstance(section, _CommandManager):

                    # Has any text been added to the file?
                    if open_file.tell():

                        # If so, add a blank line prior to section
                        open_file.write('\n')

                    # Does the command have a description?
                    if section.description:

                        # Loop through description to get valid lines
                        # with length less than the max line length
                        for line in self._get_lines(section.description):

                            # Write the current line
                            open_file.write(line + '\n')

                    # Does the command exist in the old config file?
                    if section.name in _old_config:

                        # Loop through each line in the
                        # old config for the command
                        for line in _old_config[section.name]:

                            # Write the line to the file
                            open_file.write(line + '\n')

                        # Remove the command from the old config
                        del _old_config[section.name]

                # Is the current section just text?
                else:

                    # Loop through the current line to get valid
                    # lines with length less than the max line length
                    for line in self._get_lines(section):

                        # Write the current line
                        open_file.write(line + '\n')

            # Are there any more values not used from the old config file?
            if _old_config:

                # Add a blank line
                open_file.write('\n')

                # Loop through the items in the old config
                for name in sorted(_old_config):

                    # Loop through each line for the current item
                    for line in _old_config[name]:

                        # Write the line to the config file
                        open_file.write('// {0}\n'.format(line))

    def execute(self):
        '''Executes the config file'''
        EngineServer.server_command(
            'exec source-python/{0}\n'.format(self.filepath))

    def _parse_old_file(self):
        '''Parses the old config file to get any values already set'''

        # Get a defaultdict instance to store a list of lines
        _old_config = defaultdict(list)

        # Does the file exist?
        if not self.fullpath.isfile():

            # If not, simply return the empty dictionary
            return _old_config

        # Open/close the file
        with self.fullpath.open() as open_file:

            # Get all lines from the file
            _all_lines = [line.strip() for line in open_file.readlines()]

        # Loop through each line in the old config
        for line in _all_lines:

            # Is the line a command or cvar?
            if line.startswith('//') or not line:

                # If not, continue to the next line
                continue

            # Get the command's or cvar's name
            name = line.split(' ', 1)[0]

            # Is the command/cvar valid, but have to value/arguments?
            if name in self._commands | self._cvars and not line.count(' '):

                # If not, continue to the next line
                continue

            # Make sure the line has the proper number of quotes
            line = line + ('"' if line.count('"') % 2 else '')

            # Add the line to the old config
            _old_config[name].append(line)

        # Return the dictionary
        return _old_config

    def _get_lines(self, lines, indention=0):
        '''Yields a list of lines that are
            less than the file's max line length'''
        return TextWrapper(
            self.max_line_length, '//' + ' ' * (self.indention - 2),
            '//' + ' ' * (
                self.indention if indention < 3
                else indention - 2)).wrap(lines)
