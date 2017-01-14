# ../filesystem.py

"""Provides access to engine's filesystem interface."""

# =============================================================================
# >> FORWARD IMPORTS
# =============================================================================
# Source.Python Imports
#   Filesystem
from _filesystem import is_vpk_file
from _filesystem import SeekType
from _filesystem import SourceFile


# =============================================================================
# >> IMPORTS
# =============================================================================
# Python
#   Os
import os
#   tempfile
import tempfile
#   Contextlib
from contextlib import contextmanager


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('is_vpk_file',
           'source_file',
           'SeekType',
           'SourceFile',
           )


# =============================================================================
# >> FUNCTIONS
# =============================================================================
@contextmanager
def source_file(file_path):
    """Ensures that the given file has a visible name on the file system.

    .. note:: If the file was packed in a VPK archive, it gets copied to a
        temporary file. Thus, modifying the file has no effect on the original
        file.

    Example:

    .. code:: python

        with source_file('credits.txt') as file_path:
            print('Visible path:', file_path)
            with open(file_path) as f:
                print('Content:', f.read())
    """
    if is_vpk_file(file_path):
        temp_file = _get_temp_file_name()

        # Copy the file to the real file system
        f = SourceFile.open(file_path, 'rb')
        f.save(temp_file)
        f.close()
    else:
        temp_file = file_path

    try:
        yield temp_file
    finally:
        if temp_file != file_path:
            os.remove(temp_file)


# =============================================================================
# >> FUNCTIONS
# =============================================================================
def _get_temp_file_name():
    """Generate a temporary file name with an absolute path.
    
    :rtype: str
    """
    return (tempfile._get_default_tempdir()
        + os.sep
        + next(tempfile._get_candidate_names()))
