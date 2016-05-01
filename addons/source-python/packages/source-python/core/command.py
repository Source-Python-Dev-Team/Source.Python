# ../core/command.py

"""Registers the "sp" server command and all of its sub-commands."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Site-Package Imports
#   Configobj
from configobj import ConfigObj

# Source.Python Imports
#   Auth
from auth.commands import _auth_commands
#   Autodoc
from autodoc import SphinxProject
#   Core
from core import core_logger
from core import dumps
from core.manager import core_plugin_manager
from core.version import VERSION
#   Cvars
from cvars import ConVar
#   Engines
from engines.server import engine_server
#   Paths
from paths import SP_DOCS_PATH
from paths import CUSTOM_PACKAGES_DOCS_PATH
from paths import PLUGIN_DOCS_PATH
from paths import SP_PACKAGES_PATH
from paths import CUSTOM_PACKAGES_PATH
from paths import PLUGIN_PATH
from paths import SP_DATA_PATH
#   Plugins
from plugins import _plugin_strings
from plugins.command import SubCommandManager
from plugins.instance import LoadedPlugin
#   Tick
from listeners.tick import Delay


# =============================================================================
# >> GLOBAL VARIABLES
# =============================================================================
# Get the sp.core.command logger
core_command_logger = core_logger.command


# =============================================================================
# >> CLASSES
# =============================================================================
class _CoreLoadedPlugin(LoadedPlugin):
    """Plugin instance class used to create "sp" loaded plugins."""

    logger = core_command_logger


class _CoreCommandManager(SubCommandManager):
    """Class used for executing "sp" sub-command functionality."""

    manager = core_plugin_manager
    instance = _CoreLoadedPlugin
    logger = core_command_logger

    def print_plugins(self):
        """List all currently loaded plugins."""
        # Get header messages
        message = self.prefix + _plugin_strings[
            'Plugins'].get_string() + '\n' + '=' * 61 + '\n\n'

        # Loop through all loaded plugins
        for plugin_name in sorted(self.manager):
            info = self.manager[plugin_name].info

            # Was an PluginInfo instance found?
            if info is not None:

                # Add message with the current plugin's name
                message += plugin_name + ':\n'

                # Loop through all items in the PluginInfo instance
                for item, value in info.items():

                    # Is the value a ConVar?
                    if isinstance(value, ConVar):

                        # Get the ConVar's text
                        value = '{0}:\n\t\t\t{1}: {2}'.format(
                            value.name,
                            value.help_text,
                            value.get_string())

                    # Add message for the current item and its value
                    message += '\t{0}:\n\t\t{1}\n'.format(item, value)

            # Was no PluginInfo instance found?
            else:

                # Add message with the current plugin's name
                message += plugin_name + '\n'

            # Add 1 blank line between each plugin
            message += '\n'

        # Add the ending separator
        message += '=' * 61

        # Print the message
        self.logger.log_message(message)

    @staticmethod
    def delay_execution(*args):
        """Execute a command after the given delay."""
        Delay(
            float(args[0]),
            engine_server.server_command, ' '.join(args[1:]) + '\n')

    def dump_data(self, dump_type, filename):
        """Dump data to logs."""
        # Does the given dump type exist as a function?
        if 'dump_{0}'.format(dump_type) not in dumps.__all__:

            # If not, print message to notify of unknown dump type
            self.logger.log_message(
                'Invalid dump_type "{0}". The valid types are:'.format(
                    dump_type))

            # Loop though all dump functions
            for dump in dumps.__all__:

                # Print the current dump function
                self.logger.log_message(
                    '\t{0}'.format(dump.replace('dump_', '')))

            # No need to go further
            return

        # Call the function
        getattr(dumps, 'dump_{0}'.format(dump_type))(filename)

    # Set the methods arguments
    dump_data.args = ['<dump_type>', '<filename>']

    def print_version(self):
        """Display Source.Python version information."""
        self.logger.log_message(
            'Current Source.Python version: {0}'.format(VERSION))

    def docs_handler(self, action, package):
        """Create, generate or build a Sphinx project."""
        if action == 'create':
            self._create_sphinx_project(package)
        elif action == 'generate':
            self._generate_sphinx_project(package)
        elif action == 'build':
            self._build_sphinx_project(package)
        else:
            self.logger.log_message(
                'Invalid action: "{0}".'.format(action) +
                '  Valid actions are: create, generate and build')
    docs_handler.args = ['<action>', '<package>']

    def _create_sphinx_project(self, package):
        """Create a Sphinx project."""
        if self.is_source_python(package):
            self._create_source_python_docs()
        elif self.is_custom_package(package):
            self._create_custom_package_docs(package)
        elif self.is_plugin(package):
            self._create_plugin_docs(package)
        else:
            self.logger.log_message(
                'Invalid project name: "{0}".'.format(package) +
                '  Project name must be source-python, ' +
                'a custom package name, or a plugin name.')

    def _generate_sphinx_project(self, package):
        """Generate project files for a Sphinx project."""
        if self.is_source_python(package):
            self._generate_source_python_docs()
        elif self.is_custom_package(package):
            self._generate_custom_package_docs(package)
        elif self.is_plugin(package):
            self._generate_plugin_docs(package)
        else:
            self.logger.log_message(
                'Invalid project name: "{0}".'.format(package) +
                '  Project name must be source-python, ' +
                'a custom package name, or a plugin name.')

    def _build_sphinx_project(self, package):
        """Build project files for a Sphinx project."""
        if self.is_source_python(package):
            self._build_source_python_docs()
        elif self.is_custom_package(package):
            self._build_custom_package_docs(package)
        elif self.is_plugin(package):
            self._build_plugin_docs(package)
        else:
            self.logger.log_message(
                'Invalid project name: "{0}".'.format(package) +
                '  Project name must be source-python, ' +
                'a custom package name, or a plugin name.')

    def _create_source_python_docs(self):
        """Create a Sphinx project for Source.Python."""
        project = SphinxProject(SP_PACKAGES_PATH, SP_DOCS_PATH)
        if project.project_exists():
            self.logger.log_message(
                'Sphinx project already exists for Source.Python')
        else:
            try:
                project.create(
                    'Source.Python Development Team', 'Source.Python', VERSION)
            except:
                self.logger.log_message(
                    'An error occured while creating Sphinx ' +
                    'project for Source.Python.')
            else:
                self.logger.log_message(
                    'Sphinx project has been created for Source.Python.')

    def _create_custom_package_docs(self, package):
        """Create a Sphinx project for a custom package."""
        project = SphinxProject(
            CUSTOM_PACKAGES_PATH / package,
            CUSTOM_PACKAGES_DOCS_PATH / package)
        if project.project_exists():
            self.logger.log_message(
                'Sphinx project already exists for custom' +
                ' package "{0}".'.format(package))
        else:
            try:
                project.create('Unknown')
            except:
                self.logger.log_message(
                    'An error occured while creating Sphinx project for ' +
                    'custom package "{0}".'.format(package))
            else:
                self.logger.log_message(
                    'Sphinx project has been created for' +
                    ' custom package "{0}".'.format(package))

    def _create_plugin_docs(self, package):
        """Create a Sphinx project for a plugin."""
        project = SphinxProject(
            PLUGIN_PATH / package, PLUGIN_DOCS_PATH / package)
        if project.project_exists():
            self.logger.log_message(
                'Sphinx project already exists for ' +
                'plugin "{0}".'.format(package))
        else:
            try:
                project.create('Unknown')
            except:
                self.logger.log_message(
                    'An error occured while creating Sphinx project ' +
                    'for plugin "{0}".'.format(package))
            else:
                self.logger.log_message(
                    'Sphinx project has been created for' +
                    ' plugin "{0}".'.format(package))

    def _generate_source_python_docs(self):
        """Generate Sphinx project files for Source.Python."""
        project = SphinxProject(SP_PACKAGES_PATH, SP_DOCS_PATH)
        if project.project_exists():
            try:
                project.generate_project_files('developing/modules')
            except:
                self.logger.log_message(
                    'An error occured while generating ' +
                    'project files for Source.Python')
            else:
                modules_dir = project.project_source_dir / 'developing' / 'modules'
                modules_dir.joinpath('modules.rst').remove()
                for file_path in modules_dir.files('source-python.*.rst'):
                    self._prepare_generated_source_python_file(file_path)

                self.logger.log_message(
                    'Project files have been generated for Source.Python.')
        else:
            self.logger.log_message(
                'Sphinx project does not exist for Source.Python.')

    @staticmethod
    def _prepare_generated_source_python_file(file_path):
        """Rename the generated file if it wasn't already renamed.

        If it was already renamed, remove the generated file.

        After renaming the file several patches are applied to the file like
        removing 'source-python.' and adding the ':titlesonly:' directive.
        """
        new_name = file_path.parent / file_path.basename().replace(
            'source-python.', '')
        if new_name.isfile():
            file_path.remove()
            return

        file_path.rename(new_name)

        with new_name.open() as f:
            lines = f.readlines()

        with new_name.open('w') as f:
            # Remove source-python from the title
            title = lines.pop(0).replace('source-python.', '')
            f.write(title)

            # Since the heading underline is now longer than required, we also
            # adapt it to the new title
            f.write(lines.pop(0)[0]*len(title) + '\n')

            def write_next_4_lines(current):
                f.write(current)
                for x in range(3):
                    f.write(lines.pop(0))

            while lines:
                line = lines.pop(0)

                if line.lstrip().startswith(
                        ('.. automodule:: source-python.', 'source-python.')):
                    # We need to get rid of "source-python."
                    f.write(line.replace('source-python.', ''))

                # Who's so funny and added two different indentions?
                # Subpackages are indented with 4 spaces
                elif line == 'Subpackages\n':
                    write_next_4_lines(line)
                    f.write('    :titlesonly:\n')

                # Submodules only need 3 spaces...
                elif line == 'Submodules\n':
                    write_next_4_lines(line)
                    f.write('   :titlesonly:\n')

                else:
                    f.write(line)

    def _generate_custom_package_docs(self, package):
        """Generate Sphinx project files for a custom package."""
        project = SphinxProject(
            CUSTOM_PACKAGES_PATH / package,
            CUSTOM_PACKAGES_DOCS_PATH / package)
        if project.project_exists():
            try:
                project.generate_project_files()
            except:
                self.logger.log_message(
                    'An error occured while generating project ' +
                    'files for custom package "{0}".'.format(package))
            else:
                self.logger.log_message(
                    'Project files have been generated for '
                    'custom package "{0}".'.format(package))
        else:
            self.logger.log_message(
                'Sphinx project does not exist for ' +
                'custom package "{0}".'.format(package))

    def _generate_plugin_docs(self, package):
        """Generate Sphinx project files for a plugin."""
        project = SphinxProject(
            PLUGIN_PATH / package, PLUGIN_DOCS_PATH / package)
        if project.project_exists():
            try:
                project.generate_project_files()
            except:
                self.logger.log_message(
                    'An error occured while generating project ' +
                    'files for plugin "{0}".'.format(package))
            else:
                self.logger.log_message(
                    'Project files have been generated ' +
                    'for plugin "{0}".'.format(package))
        else:
            self.logger.log_message(
                'Sphinx project does not exist for' +
                ' plugin "{0}".'.format(package))

    def _build_source_python_docs(self):
        """Build Sphinx project files for Source.Python."""
        project = SphinxProject(SP_PACKAGES_PATH, SP_DOCS_PATH)
        if project.project_exists():
            # Update version and release
            conf_file = project.project_source_dir / 'conf.py'
            with conf_file.open() as f:
                lines = f.readlines()

            # Get the version string
            version = VERSION if VERSION == 'unversioned' else (
                'v{0}'.format(VERSION))

            with conf_file.open('w') as f:
                for line in lines:
                    if line.startswith(('version', 'release')):
                        line = '{0} = \' {1}\'\n'.format(
                            line.split(maxsplit=1)[0], version)

                    f.write(line)

            # Create/update credits.rst
            with project.project_source_dir.joinpath(
                    'general', 'credits.rst').open('w') as f:
                f.write(self._get_updated_credits_wiki())

            try:
                project.build()
            except:
                self.logger.log_message(
                    'An error occured while building ' +
                    'project files for Source.Python.')
            else:
                self.logger.log_message(
                    'Project files have been built for Source.Python.')
        else:
            self.logger.log_message(
                'Sphinx project does not exist for Source.Python.')

    @staticmethod
    def _get_updated_credits_wiki():
        """Return the content for the credits.rst."""
        groups = ConfigObj(
            SP_DATA_PATH / 'credits.ini', encoding='unicode_escape')

        header = 'Credits'
        column1 = 'Nickname'
        column2 = 'Real name'
        col1_len = len(column1)
        col2_len = len(column2)

        output = '{}\n{}\n\n'.format(header, '='*len(header))
        for group, names in groups.items():
            # Determine the maximum column widths
            max_nickname = len(max(names.keys(), key=len)) or col1_len
            max_realname = len(max(names.values(), key=len)) or col2_len

            # Calculate the row separator
            separator = '{}  {}'.format('='*max_nickname, '='*max_realname)

            # Use the group name as a sub-heading
            output += '{}\n{}\n\n'.format(group, '-'*len(group))

            # Add columns + separators
            output += '{}\n{}{}  {}\n{}\n'.format(
                separator, column1,
                ' ' * (max_nickname - col1_len),
                column2, separator)

            # Add the table content
            for nickname, realname in names.items():
                output += '{}  {}\n'.format(
                    nickname.ljust(max_nickname), realname)

            output += separator
            output += '\n\n\n'

        return output

    def _build_custom_package_docs(self, package):
        """Build Sphinx project files for a custom package."""
        project = SphinxProject(
            CUSTOM_PACKAGES_PATH / package,
            CUSTOM_PACKAGES_DOCS_PATH / package)
        if project.project_exists():
            try:
                project.build()
            except:
                self.logger.log_message(
                    'An error occured while building project ' +
                    'files for custom package "{0}".'.format(package))
            else:
                self.logger.log_message(
                    'Project files have been built for ' +
                    'custom package "{0}".'.format(package))
        else:
            self.logger.log_message(
                'Sphinx project does not exist for ' +
                'custom package "{0}".'.format(package))

    def _build_plugin_docs(self, package):
        """Build Sphinx project files for a plugin."""
        project = SphinxProject(
            PLUGIN_PATH / package, PLUGIN_DOCS_PATH / package)
        if project.project_exists():
            if not self.manager.is_loaded(package):
                self.logger.log_message(
                    'Plugin must be loaded to build the project files.')
                return

            try:
                project.build()
            except:
                self.logger.log_message(
                    'An error occured while building project ' +
                    'files for plugin "{0}".'.format(package))
            else:
                self.logger.log_message(
                    'Project files have been built for ' +
                    'plugin "{0}".'.format(package))
        else:
            self.logger.log_message(
                'Sphinx project does not exist for ' +
                'plugin "{0}".'.format(package))

    @staticmethod
    def is_source_python(package):
        """Return True if the given package name is source-python."""
        return package == 'source-python'

    @staticmethod
    def is_custom_package(package):
        """Return True if the given package name is a custom package."""
        return package in map(
            lambda path: str(path.namebase), CUSTOM_PACKAGES_PATH.listdir())

    @staticmethod
    def is_plugin(package):
        """Return True if the given package name is a plugin."""
        return package in map(
            lambda path: str(path.namebase), PLUGIN_PATH.dirs())

    def print_credits(self):
        """List all credits for Source.Python."""
        # Get header messages
        message = self.prefix + _plugin_strings[
            'Credits'].get_string() + '\n' + '=' * 61 + '\n\n'

        # Get the credits information
        groups = ConfigObj(
            SP_DATA_PATH / 'credits.ini', encoding='unicode_escape')

        # Loop through all groups in the credits
        for group in groups:

            # Add the current group's name
            message += '\t' + group + ':\n'

            # Loop through all names in the current group
            for name in groups[group]:

                # Add the current name
                message += '\t\t' + name + ' ' * (
                    20 - len(name)) + groups[group][name] + '\n'

            # Add 1 blank line between groups
            message += '\n'

        # Print the message
        self.logger.log_message(message + '=' * 61 + '\n\n')

# Get the _CoreCommandManager instance
_core_command = _CoreCommandManager('sp', 'Source.Python base command.')

# Register the load/unload sub-commands
_core_command['load'] = _core_command.load_plugin
_core_command['unload'] = _core_command.unload_plugin
_core_command['reload'] = _core_command.reload_plugin

# Register the 'auth' sub-command
_core_command['auth'] = _auth_commands

# Register the 'delay' sub-command
_core_command['delay'] = _core_command.delay_execution
_core_command['delay'].args = ['<delay>', '<command>', '[arguments]']

# Register the 'dump' sub-command
_core_command['dump'] = _core_command.dump_data

# Register all printing sub-commands
_core_command['list'] = _core_command.print_plugins
_core_command['version'] = _core_command.print_version
_core_command['credits'] = _core_command.print_credits
_core_command['help'] = _core_command.print_help

# Register the 'docs' sub-command
_core_command['docs'] = _core_command.docs_handler
