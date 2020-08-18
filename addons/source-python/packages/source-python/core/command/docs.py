# ../core/command/docs.py

"""Registers the sp docs sub-commands."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Site-Package Imports
#   Configobj
from configobj import ConfigObj

# Source.Python Imports
#   Autodoc
from autodoc import SphinxProject
#   Commands
from commands.typed import TypedServerCommand
#   Core
from core.command import core_command
from core.command import core_command_logger
from core.version import VERSION
from core.version import is_unversioned
#   Hooks
from hooks.exceptions import except_hooks
#   Paths
from paths import SP_DATA_PATH
from paths import SP_DOCS_PATH
from paths import CUSTOM_PACKAGES_DOCS_PATH
from paths import PLUGIN_DOCS_PATH
from paths import SP_PACKAGES_PATH
from paths import CUSTOM_PACKAGES_PATH
from paths import PLUGIN_PATH
#   Plugins
from plugins.manager import plugin_manager


# =============================================================================
# >> GLOBALS
# =============================================================================
logger = core_command_logger.docs


# =============================================================================
# >> sp docs create/generate/build
# =============================================================================
@core_command.server_sub_command(['docs', 'create'])
def _sp_docs_create(command_info, package):
    """Create a Sphinx project."""
    _create_sphinx_project(package)


@core_command.server_sub_command(['docs', 'generate'])
def _sp_docs_generate(command_info, package):
    """Generate a Sphinx project."""
    _generate_sphinx_project(package)


@core_command.server_sub_command(['docs', 'build'])
def _sp_docs_build(command_info, package):
    """Build a Sphinx project."""
    _build_sphinx_project(package)


# =============================================================================
# >> CREATE FUNCTIONS
# =============================================================================
def _create_sphinx_project(package):
    """Create a Sphinx project."""
    if is_source_python(package):
        _create_source_python_docs()
    elif is_custom_package(package):
        _create_custom_package_docs(package)
    elif is_plugin(package):
        _create_plugin_docs(package)
    else:
        logger.log_message(
            'Invalid project name: "{0}".'.format(package) +
            '  Project name must be source-python, ' +
            'a custom package name, or a plugin name.')


def _create_source_python_docs():
    """Create a Sphinx project for Source.Python."""
    project = SphinxProject(SP_PACKAGES_PATH, SP_DOCS_PATH)
    if project.project_exists():
        logger.log_message(
            'Sphinx project already exists for Source.Python')
    else:
        try:
            project.create(
                'Source.Python Development Team', 'Source.Python', VERSION)
        except:
            except_hooks.print_exception()
            logger.log_message(
                'An error occured while creating Sphinx ' +
                'project for Source.Python.')
        else:
            logger.log_message(
                'Sphinx project has been created for Source.Python.')


def _create_custom_package_docs(package):
    """Create a Sphinx project for a custom package."""
    project = SphinxProject(
        CUSTOM_PACKAGES_PATH / package,
        CUSTOM_PACKAGES_DOCS_PATH / package)
    if project.project_exists():
        logger.log_message(
            'Sphinx project already exists for custom' +
            ' package "{0}".'.format(package))
    else:
        try:
            project.create('Unknown')
        except:
            except_hooks.print_exception()
            logger.log_message(
                'An error occured while creating Sphinx project for ' +
                'custom package "{0}".'.format(package))
        else:
            logger.log_message(
                'Sphinx project has been created for' +
                ' custom package "{0}".'.format(package))


def _create_plugin_docs(package):
    """Create a Sphinx project for a plugin."""
    project = SphinxProject(
        PLUGIN_PATH / package, PLUGIN_DOCS_PATH / package)
    if project.project_exists():
        logger.log_message(
            'Sphinx project already exists for ' +
            'plugin "{0}".'.format(package))
    else:
        try:
            project.create('Unknown')
        except:
            except_hooks.print_exception()
            logger.log_message(
                'An error occured while creating Sphinx project ' +
                'for plugin "{0}".'.format(package))
        else:
            logger.log_message(
                'Sphinx project has been created for' +
                ' plugin "{0}".'.format(package))


# =============================================================================
# >> GENERATE FUNCTIONS
# =============================================================================
def _generate_sphinx_project(package):
    """Generate project files for a Sphinx project."""
    if is_source_python(package):
        _generate_source_python_docs()
    elif is_custom_package(package):
        _generate_custom_package_docs(package)
    elif is_plugin(package):
        _generate_plugin_docs(package)
    else:
        logger.log_message(
            'Invalid project name: "{0}".'.format(package) +
            '  Project name must be source-python, ' +
            'a custom package name, or a plugin name.')


def _generate_source_python_docs():
    """Generate Sphinx project files for Source.Python."""
    project = SphinxProject(SP_PACKAGES_PATH, SP_DOCS_PATH)
    if project.project_exists():
        try:
            project.generate_project_files('developing/modules')
        except:
            except_hooks.print_exception()
            logger.log_message(
                'An error occured while generating ' +
                'project files for Source.Python')
        else:
            modules_dir = project.project_source_dir / 'developing' / 'modules'
            modules_dir.joinpath('modules.rst').remove()
            for file_path in modules_dir.files('source-python.*.rst'):
                _prepare_generated_source_python_file(file_path)

            logger.log_message(
                'Project files have been generated for Source.Python.')
    else:
        logger.log_message(
            'Sphinx project does not exist for Source.Python.')


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


def _generate_custom_package_docs(package):
    """Generate Sphinx project files for a custom package."""
    project = SphinxProject(
        CUSTOM_PACKAGES_PATH / package,
        CUSTOM_PACKAGES_DOCS_PATH / package)
    if project.project_exists():
        try:
            project.generate_project_files()
        except:
            except_hooks.print_exception()
            logger.log_message(
                'An error occured while generating project ' +
                'files for custom package "{0}".'.format(package))
        else:
            logger.log_message(
                'Project files have been generated for '
                'custom package "{0}".'.format(package))
    else:
        logger.log_message(
            'Sphinx project does not exist for ' +
            'custom package "{0}".'.format(package))


def _generate_plugin_docs(package):
    """Generate Sphinx project files for a plugin."""
    project = SphinxProject(
        PLUGIN_PATH / package, PLUGIN_DOCS_PATH / package)
    if project.project_exists():
        try:
            project.generate_project_files()
        except:
            except_hooks.print_exception()
            logger.log_message(
                'An error occured while generating project ' +
                'files for plugin "{0}".'.format(package))
        else:
            logger.log_message(
                'Project files have been generated ' +
                'for plugin "{0}".'.format(package))
    else:
        logger.log_message(
            'Sphinx project does not exist for' +
            ' plugin "{0}".'.format(package))


# =============================================================================
# >> BUILD FUNCTIONS
# =============================================================================
def _build_sphinx_project(package):
    """Build project files for a Sphinx project."""
    if is_source_python(package):
        _build_source_python_docs()
    elif is_custom_package(package):
        _build_custom_package_docs(package)
    elif is_plugin(package):
        _build_plugin_docs(package)
    else:
        logger.log_message(
            'Invalid project name: "{0}".'.format(package) +
            '  Project name must be source-python, ' +
            'a custom package name, or a plugin name.')


def _build_source_python_docs():
    """Build Sphinx project files for Source.Python."""
    project = SphinxProject(SP_PACKAGES_PATH, SP_DOCS_PATH)
    if project.project_exists():
        # Update version and release
        conf_file = project.project_source_dir / 'conf.py'
        with conf_file.open() as f:
            lines = f.readlines()

        # Get the version string
        version = '' if is_unversioned() else f'v{VERSION}'

        with conf_file.open('w') as f:
            for line in lines:
                if line.startswith(('version', 'release')):
                    line = '{0} = \' {1}\'\n'.format(
                        line.split(maxsplit=1)[0], version)

                f.write(line)

        # Create/update credits.rst
        with project.project_source_dir.joinpath(
                'general', 'credits.rst').open('w') as f:
            f.write(_get_updated_credits_wiki())

        try:
            project.build()
        except:
            except_hooks.print_exception()
            logger.log_message(
                'An error occured while building ' +
                'project files for Source.Python.')
        else:
            logger.log_message(
                'Project files have been built for Source.Python.')
    else:
        logger.log_message(
            'Sphinx project does not exist for Source.Python.')


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


def _build_custom_package_docs(package):
    """Build Sphinx project files for a custom package."""
    project = SphinxProject(
        CUSTOM_PACKAGES_PATH / package,
        CUSTOM_PACKAGES_DOCS_PATH / package)
    if project.project_exists():
        try:
            project.build()
        except:
            except_hooks.print_exception()
            logger.log_message(
                'An error occured while building project ' +
                'files for custom package "{0}".'.format(package))
        else:
            logger.log_message(
                'Project files have been built for ' +
                'custom package "{0}".'.format(package))
    else:
        logger.log_message(
            'Sphinx project does not exist for ' +
            'custom package "{0}".'.format(package))


def _build_plugin_docs(package):
    """Build Sphinx project files for a plugin."""
    project = SphinxProject(
        PLUGIN_PATH / package, PLUGIN_DOCS_PATH / package)
    if project.project_exists():
        if not plugin_manager.is_loaded(package):
            logger.log_message(
                'Plugin must be loaded to build the project files.')
            return

        try:
            project.build()
        except:
            except_hooks.print_exception()
            logger.log_message(
                'An error occured while building project ' +
                'files for plugin "{0}".'.format(package))
        else:
            logger.log_message(
                'Project files have been built for ' +
                'plugin "{0}".'.format(package))
    else:
        logger.log_message(
            'Sphinx project does not exist for ' +
            'plugin "{0}".'.format(package))


# =============================================================================
# >> HELPER FUNCTIONS
# =============================================================================
def is_source_python(package):
    """Return True if the given package name is source-python."""
    return package == 'source-python'


def is_custom_package(package):
    """Return True if the given package name is a custom package."""
    return package in map(
        lambda path: str(path.namebase), CUSTOM_PACKAGES_PATH.listdir())


def is_plugin(package):
    """Return True if the given package name is a plugin."""
    return package in map(
        lambda path: str(path.namebase), PLUGIN_PATH.dirs())


# =============================================================================
# >> DESCRIPTIONS
# =============================================================================
TypedServerCommand.parser.set_node_description(
    ['sp', 'docs'], 'Documentation specific commands.')
