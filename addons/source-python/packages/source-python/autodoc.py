"""This module provides functions to create documentation for plugins or
custom modules/packages automatically."""

# TODO: Add custom module/package support

# =============================================================================
# >> IMPORTS
# =============================================================================
# Python
import sys

# Source.Python
from paths import PLUGIN_PATH


# =============================================================================
# >> FUNCTIONS
# =============================================================================
def project_exists(name):
    """Return True if a Sphinx project for a plugin exists.

    :param str name: The name of the plugin to test.
    """
    return (PLUGIN_PATH / name / 'doc').isdir()

def create_project(name, author, project_name=None, version='1'):
    """Start a new Sphinx project for a plugin or custom module/package.

    :param str name: The name of the plugin or custom module/package.
    :param str author: The name of the author.
    :param str project_name: The project name that will be displayed in the
        documentation. If None ``name`` will be used.
    :param str version: The project version.
    """
    from sphinx.quickstart import main

    args = [
        '', # Will be skipped.
        '-q', # Don't start the interactive mode
    ]

    if project_name is None:
        project_name = name

    args.append('-p {0}'.format(project_name))
    args.append('-a {0}'.format(author))
    args.append('-v {0}'.format(version))

    # TODO:
    # Also test if the plugin is loaded to avoid site effects, because load()
    # wasn't called
    plugin_dir = PLUGIN_PATH / name
    if not plugin_dir.isdir():
        raise NameError('"{0}" is not a valid plugin.'.format(name))

    project_dir = plugin_dir / 'doc'
    if not project_dir.isdir():
        project_dir.mkdir()

    args.extend([
        str(project_dir),
        '--sep', # Separate rst and build directory
        '--ext-autodoc', # Enable autodoc
        '--no-makefile',
        '--no-batchfile'
    ])

    # Hacky, but required, because sphinx is reading sys.argv even if you pass
    # a list to main()
    old_argv = sys.argv
    sys.argv = args
    try:
        main(sys.argv)
    except:
        raise
    finally:
        sys.argv = old_argv
    sys.argv = old_argv

def generate_rst_files(name):
    """Generate *.rst files for a plugin or custom module/package.

    :param str name: The name of the plugin or custom module/package.
    """
    plugin_dir = PLUGIN_PATH / name
    project_dir = plugin_dir / 'doc' / 'source'
    if not project_dir.isdir():
        raise NameError(
            'Create a project before trying to generate *.rst files.')

    from sphinx.apidoc import main
    args = [
        '', # Will be skipped.
        '-e', # Separate pages/files for every module
        '-o',
        str(project_dir),
        str(plugin_dir), # Package to document
        str(project_dir), # Exclude the doc directory
    ]

    # Hacky, but required, because sphinx is reading sys.argv even if you pass
    # a list to main()
    old_argv = sys.argv
    sys.argv = args
    try:
        main(sys.argv)
    except:
        raise
    finally:
        sys.argv = old_argv

def build_documentation(name):
    """Build the documentation for a plugin or custom module/package.

    :param str name: The name of the plugin or custom module/package.
    """
    plugin_dir = PLUGIN_PATH / name
    project_dir = plugin_dir / 'doc'
    if not project_dir.isdir():
        raise NameError(
            'Create a project before trying to build a documentation.')

    source_dir = project_dir / 'source'
    build_dir = project_dir / 'build'

    from sphinx import main
    args = [
        '', # Will be skipped.
        str(source_dir),
        str(build_dir),
    ]

    # Hacky, but required, because sphinx is reading sys.argv even if you pass
    # a list to main()
    old_argv = sys.argv
    sys.argv = args
    try:
        main(sys.argv)
    except SystemExit as e:
        if e.code != 0:
            raise
    finally:
        sys.argv = old_argv