# ../config/manager.py

"""Provides a way to create and execute configuration files."""

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
#   Cvars
from cvars import ConVar
#   Engines
from engines.server import queue_command_string
#   Hooks
from hooks.exceptions import except_hooks
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
    """Config Management class used to create a config file."""

    def __init__(
            self, filepath, cvar_prefix='', indention=3,
            max_line_length=79, encoding='utf-8'):
        """Initialized the configuration manager.

        :param str filepath:
            Location where the configuration should be stored. The extension
            ``.cfg`` can be skipped. It will be added automatically.
        :param str cvar_prefix:
            A prefix that should be added to every console variable.
        :param int indention:
            Number of spaces used to indent values within the configuration
            file.
        :param int max_line_length:
            Maximum line length within the configuration file.
        """
        # Does the filepath contain the extension?
        if filepath.endswith('.cfg'):

            # Remove the extension from the filepath
            filepath = filepath[:~3]

        # Store the primary attributes
        self._filepath = filepath
        self._cvar_prefix = cvar_prefix
        self._indention = indention
        self._max_line_length = max_line_length
        self._encoding = encoding

        # Store the header and separator
        self.header = ''
        self.separator = '#'

        # Store the section types
        self._cvars = set()
        self._commands = set()

        # Store the section list
        self._sections = list()

    def __enter__(self):
        """Used when using "with" context management to create the file."""
        return self

    @property
    def filepath(self):
        """Return the file path for the config file.

        The path does not include the ``.cfg`` file extension.

        :rtype: str
        """
        return self._filepath

    @property
    def cvar_prefix(self):
        """Return the convar prefix for the config file.

        :rtype: str
        """
        return self._cvar_prefix

    @property
    def indention(self):
        """Return the indention value for the config file.

        :rtype: int
        """
        return self._indention

    @property
    def max_line_length(self):
        """Return the max line length for the config file.

        :rtype: int
        """
        return self._max_line_length

    @property
    def fullpath(self):
        """Return the full path to the configuration file.

        The path include the ``.cfg`` file extension.

        :rtype: path.Path
        """
        return CFG_PATH / self.filepath + '.cfg'

    def cvar(
            self, name, default=0, description='',
            flags=0, min_value=None, max_value=None):
        """Add/return a cvar instance to add to the config file.

        :param str name:
            Name of the console variable.
        :param object default:
            A default value for the console variable. It will be converted to
            a string.
        :param str/TranslationStrings description:
            A description of the console variable.
        :param ConVarFlags flags:
            Flags that should be used for the console variable.
        :param float min_value:
            Minimum value.
        :param float max_value:
            Maximum value.
        :rtype: _CvarManager
        """
        # Add the stored prefix to the given name...
        name = self.cvar_prefix + name

        # Get the _CvarManager instance for the given arguments
        section = _CvarManager(
            name, default, description, flags, min_value, max_value)

        # Add the cvar to the list of cvars
        self._cvars.add(name)

        # Add the _CvarManager instance to the list of sections
        self._sections.append(section)

        # Return the _CvarManager instance
        return section

    def section(self, name, separator='#'):
        """Add/return a section instance to add to the config file.

        :param str/TranslationStrings name:
            Name of the section to add.
        :param str separator:
            A single separator character to use to separate the section.
        :rtype: _SectionManager
        """
        # Get the _SectionManager instance for the given arguments
        section = _SectionManager(name, separator)

        # Add the _SectionManager instance to the list of sections
        self._sections.append(section)

        # Return the _SectionManager instance
        return section

    def command(self, name, description=''):
        """Add/return a command instance to add to the config file.

        :param str name:
            Name of the command to add.
        :param str/TranslationString description:
            Description of the command.
        :rtype: _CommandManager
        """
        # Get the _CommandManager instance for the given arguments
        section = _CommandManager(name, description)

        # Add the command to the list of commands
        self._commands.add(name)

        # Add the _CommandManager instance to the list of sections
        self._sections.append(section)

        # Return the _CommandManager instance
        return section

    def text(self, text):
        """Add text to the config file.

        :param str/TranslationStrings text:
            The text to add.
        """
        # Is the given text a TranslationStrings instance?
        if isinstance(text, TranslationStrings):

            # Get the proper language string
            text = text.get_string()

        # Add the text
        self._sections.append(text)

    def __exit__(self, exctype, value, trace_back):
        """Used when exiting "with" context management to create file."""
        # Was an exception raised?
        if trace_back:

            # Print the exception
            except_hooks.print_exception(exctype, value, trace_back)

            # Return
            return False

        # Write the file
        self.write()

        # Execute the file
        self.execute()

        # Return
        return True

    def write(self):
        """Write the config file."""
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
        with self.fullpath.open('w', encoding=self._encoding) as open_file:

            # Get the number of spaces to indent after //
            spaces = ' ' * (self.indention - 2)

            # Is there a header for the file?
            if self.header:
                self._write_header(open_file, spaces)

            # Loop through all sections in the file
            for section in self._sections:

                # Is the current section a cvar?
                if isinstance(section, _CvarManager):
                    self._write_cvar_section(
                        open_file, section, _old_config, spaces)

                # Is the current section a Section?
                elif isinstance(section, _SectionManager):
                    self._write_section(open_file, section, spaces)

                # Is the current section a Command?
                elif isinstance(section, _CommandManager):
                    self._write_command_section(
                        open_file, section, _old_config)

                # Is the current section a blank line?
                elif not section:

                    # Create a blank line
                    open_file.write('\n')

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
        """Execute the config file."""
        # Does the file exist?
        if not self.fullpath.isfile():
            raise FileNotFoundError(
                'Cannot execute file "{0}", file not found'.format(
                    self.fullpath))

        # Open/close the file
        with self.fullpath.open(encoding=self._encoding) as open_file:

            # Loop through all lines in the file
            for line in open_file.readlines():

                # Strip the line
                line = line.strip()

                # Is the line a command or cvar?
                if line.startswith('//') or not line:
                    continue

                # Get the command's or cvar's name
                name = line.split(' ', 1)[0]

                # Is the command/cvar valid
                if name not in self._commands | self._cvars:
                    continue

                # Does the command/cvar have any value/arguments?
                if not line.count(' '):
                    continue

                # Is this a command?
                if name in self._commands:

                    # Execute the line
                    queue_command_string(line)

                # Is this a cvar
                else:

                    # Get the cvar's value
                    value = line.split(' ', 1)[1]

                    # Do quotes need removed?
                    if value.startswith('"') and line.count('"') >= 2:

                        # Remove the quotes
                        value = value.split('"')[1]

                    # Set the cvar's value
                    ConVar(name).set_string(value)

    def _parse_old_file(self):
        """Parse the old config file to get any values already set."""
        # Get a defaultdict instance to store a list of lines
        _old_config = defaultdict(list)

        # Does the file exist?
        if not self.fullpath.isfile():

            # If not, simply return the empty dictionary
            return _old_config

        # Open/close the file
        with self.fullpath.open(encoding=self._encoding) as open_file:

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
            line += '"' if line.count('"') % 2 else ''

            # Add the line to the old config
            _old_config[name].append(line)

        # Return the dictionary
        return _old_config

    def _write_header(self, open_file, spaces):
        """Write the config's header."""
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

    def _write_cvar_section(self, open_file, section, _old_config, spaces):
        """Write the Cvar section to the config file."""
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
            default = ' "{0}"\n' if isinstance(
                section.default, str) else ' {0}\n'
            open_file.write(
                '//' + spaces +
                _config_strings['Default'].get_string() +
                default.format(section.default))

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
                _old_config[section.name][0] + '\n\n')

            # Remove the cvar from the old config file dictionary
            del _old_config[section.name]

        # Does the cvar not exist in the old config file
        # and the value is a float or integer?
        elif isinstance(section.default, (float, int)):

            # Write the cvar line using the default value
            open_file.write(
                ' ' * self.indention + section.name +
                ' {0}\n\n'.format(section.default))

        # Does the cvar not exist in the old config file
        # And the value is not a float or integer?
        else:

            # Write the cvar line using the default
            # value with quotes around the value
            open_file.write(
                ' ' * self.indention + section.name +
                ' "{0}"\n\n'.format(section.default))

    def _write_section(self, open_file, section, spaces):
        """Write the section to the config file."""
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

    def _write_command_section(self, open_file, section, _old_config):
        """Write the Command section to the config file."""
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

    def _get_lines(self, lines, indention=0):
        """Yield a list of lines less than the file's max line length."""
        return TextWrapper(
            self.max_line_length, '//' + ' ' * (self.indention - 2),
            '//' + ' ' * (
                self.indention if indention < 3
                else indention - 2)).wrap(lines)
