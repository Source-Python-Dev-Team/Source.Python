# ../core/update.py

"""Provides functions to update Source.Python and its data files."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Python Imports
import json
import time

from zipfile import ZipFile
from urllib.request import urlopen

# Source.Python Imports
#   Core
from core import PLATFORM
from core import SOURCE_ENGINE_BRANCH
from core.logger import core_logger
#   Paths
from paths import ADDONS_PATH
from paths import GAME_PATH
from paths import UPDATE_PATH
from paths import DATA_PATH
from paths import SP_DATA_PATH
#   KeyValues
from keyvalues import KeyValues


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = (
    'ARTIFACTS_URL',
    'BASE_DOWNLOAD_URL',
    'CHECKSUM_URL',
    'DATA_URL',
    'DATA_ZIP_FILE',
    'download_file',
    'get_build_artifacts',
    'get_download_url',
    'get_latest_data_checksum',
    'is_new_data_available',
    'update_in_progress',
    'update_data',
)


# =============================================================================
# >> GLOBAL VARIABLES
# =============================================================================
# Don't use __getattr__ here. 'update' is a method of the _LogInstance class.
update_logger = core_logger['update']

BINARY_EXT = 'so' if PLATFORM == 'linux' else 'dll'

SP_VDF1 = 'addons/source-python'
SP_VDF2 = 'addons/source-python2'

DATA_ZIP_FILE = DATA_PATH / 'source-python-data.zip'
UPDATE_ZIP_FILE = UPDATE_PATH / 'source-python.zip'
VDF_FILE = ADDONS_PATH / 'source-python.vdf'
LOADER_FILE = ADDONS_PATH / f'source-python.{BINARY_EXT}'
LOADER_UPDATE_FILE = UPDATE_PATH / 'addons' / f'source-python.{BINARY_EXT}'
VDF_UPDATE_FILE = UPDATE_PATH / 'addons' / 'source-python.vdf'

BASE_DATA_URL = 'http://data.sourcepython.com'
CHECKSUM_URL = f'{BASE_DATA_URL}/checksum.txt'
DATA_URL = f'{BASE_DATA_URL}/source-python-data.zip'

BASE_DOWNLOAD_URL = 'http://downloads.sourcepython.com'
ARTIFACTS_URL = f'{BASE_DOWNLOAD_URL}/artifacts.txt'

DEFAULT_TIMEOUT = 3

#: Indicates, whether an update is in progress (stage 1 has been applied).
update_in_progress = False


# =============================================================================
# >> FUNCTIONS
# =============================================================================
def download_file(url_path, file_path, timeout=3):
    """Download a file from an URL to a specific file.

    :param str url_path:
        The URL that should be opened.
    :param str file_path:
        The file where the content of the URL should be stored.
    :param float timeout:
        Number of seconds that need to pass until a timeout occurs.
    """
    update_logger.log_debug(f'Downloading file ({url_path}) to {file_path} ...')
    now = time.time()

    with urlopen(url_path, timeout=timeout) as url:
        data = url.read()

    with file_path.open('wb') as f:
        f.write(data)

    update_logger.log_info(
        'File has been downloaded. Time elapsed: {:0.2f} seconds'.format(
            time.time()-now))


# =============================================================================
# >> FULL SP UPDATE
# =============================================================================
def do_full_update(timeout=DEFAULT_TIMEOUT):
    """Starts a full update of Source.Python.

    A restart of the server and possibly manual work is required. Please see
    the Source.Python log for the instructions.

    :param float timeout:
        Number of seconds that need to pass until a timeout occurs.
    """
    global update_in_progress
    if update_in_progress:
        update_logger.log_message(
            'An update is already in progress. Please follow the instructions '
            'in the log.')
        return

    # Make sure there is a clean update directory
    _clean_update_dir()
    try:
        _download_latest_version(timeout)
        _apply_update_stage1()
        update_in_progress = True
    except:
        # Make sure to leave a clean update directory, so the loader doesn't
        # get confused.
        _clean_update_dir()
        raise

def get_build_artifacts(timeout=DEFAULT_TIMEOUT):
    """Return the artifacts of the latest Source.Python build.

    :param float timeout:
        Number of seconds that need to pass until a timeout occurs.
    """
    update_logger.log_debug('Getting artifacts...')
    with urlopen(ARTIFACTS_URL, timeout=timeout) as url:
        return url.read().decode('utf-8').split('\n')

def get_download_url(game=SOURCE_ENGINE_BRANCH, timeout=DEFAULT_TIMEOUT):
    """Get the latest Source.Python download URL for a specific game.

    :param str game:
        The game game to look for (e.g. ``css``).
    :param float timeout:
        Number of seconds that need to pass until a timeout occurs.
    :rtype: str
    :raise ValueError:
        Raised if the game wasn't found.
    """
    for relative_path in get_build_artifacts(timeout):
        if f'-{game}-' in relative_path:
            return f'{BASE_DOWNLOAD_URL}/{relative_path}'

    raise ValueError(f'Unable to find a download URL for game "{game}".')

def _clean_update_dir():
    """Clear or create the update directory."""
    if UPDATE_PATH.exists():
        for f in UPDATE_PATH.listdir():
            if f.isfile():
                f.remove()
            else:
                f.rmtree()
    else:
        UPDATE_PATH.mkdir()

def _download_latest_version(timeout=DEFAULT_TIMEOUT):
    """Download the latest Source.Python version.

    :param float timeout:
        Number of seconds that need to pass until a timeout occurs.
        """
    download_file(get_download_url(), UPDATE_ZIP_FILE, timeout)

def _apply_update_stage1():
    """Apply stage 1 of the version update."""
    update_logger.log_message('Applying Source.Python update stage 1...')

    # Extract all files to the update directory
    with ZipFile(UPDATE_ZIP_FILE) as zip:
        zip.extractall(UPDATE_PATH)

    UPDATE_ZIP_FILE.remove()
    VDF_UPDATE_FILE.remove()

    if PLATFORM == 'windows':
        _apply_update_stage1_windows()
    else:
        _apply_update_stage1_linux()

def _apply_update_stage1_windows():
    """Apply the Windows specific part of stage 1.

    On Windows files that are currently in use (``source-python.dll``) can't be
    replaced. Thus, this function checks if ``source-python.vdf`` exists. If it
    does, the new ``source-python.dll`` is copied to the addons directory with
    a new name (``source-python2.dll``). After that the VDF entry is modified
    to point to the new loader.
    If ``source-python.vdf`` does not exist, manual action is required.
    """
    if not VDF_FILE.isfile():
        update_logger.log_message(
            f'Stage 1 has been applied. Please shutdown your server and move '
            f'(do not copy) {LOADER_UPDATE_FILE} to {LOADER_FILE}. After that '
            f'start your server to apply stage 2.')
    else:
        update_logger.log_debug('Determining current VDF entry...')
        kv = KeyValues.from_file(VDF_FILE)

        # Get the current and new entry for the VDF file
        current_entry = kv.get_string('file')
        if current_entry == SP_VDF2:
            new_entry = SP_VDF1
        elif current_entry == SP_VDF1:
            new_entry = SP_VDF2
        else:
            raise ValueError(f'Unexpected entry in VDF: {current_entry}')

        update_logger.log_debug(f'Current VDF entry: {current_entry}')
        update_logger.log_debug(f'New VDF entry: {new_entry}')

        update_logger.log_debug('Moving new loader binary to game directory...')
        LOADER_UPDATE_FILE.move(GAME_PATH / f'{new_entry}.{BINARY_EXT}')

        kv.set_string('file', new_entry)
        kv.save_to_file(VDF_FILE)

        update_logger.log_message(
            'Stage 1 has been applied. Restart your server to apply stage 2.')

def _apply_update_stage1_linux():
    """Apply the Linux specific part of stage 1."""
    update_logger.log_debug('Moving new loader binary to game directory...')
    LOADER_UPDATE_FILE.move(LOADER_FILE)

    update_logger.log_message(
        'Stage 1 has been applied. Restart your server to apply stage 2.')


# =============================================================================
# >> SP DATA UPDATE
# =============================================================================
def update_data(timeout=DEFAULT_TIMEOUT):
    """Download and unpack the latest data from the build server.

    Old data gets deleted before unpacking.

    :param float timeout:
        Number of seconds that need to pass until a timeout occurs.
    """
    _download_latest_data(timeout)
    if SP_DATA_PATH.isdir():
        update_logger.log_debug('Removing {} ...'.format(SP_DATA_PATH))
        SP_DATA_PATH.rmtree()

    _unpack_data()

def is_new_data_available(timeout=DEFAULT_TIMEOUT):
    """Return ``True`` if new data is available.

    :param float timeout:
        Number of seconds that need to pass until a timeout occurs.
    :rtype: bool
    """
    if not DATA_ZIP_FILE.isfile():
        return True

    return DATA_ZIP_FILE.read_hexhash('md5') != get_latest_data_checksum(timeout)

def get_latest_data_checksum(timeout=DEFAULT_TIMEOUT):
    """Return the MD5 checksum of the latest data from the build server.

    :param float timeout:
        Number of seconds that need to pass until a timeout occurs.
    :rtype: str
    """
    with urlopen(CHECKSUM_URL, timeout=timeout) as url:
        return url.read().decode()

def _download_latest_data(timeout=DEFAULT_TIMEOUT):
    """Download the latest data from the build server.

    :param float timeout:
        Number of seconds that need to pass until a timeout occurs.
    """
    download_file(DATA_URL, DATA_ZIP_FILE, timeout)

def _unpack_data():
    """Unpack ``source-python-data.zip``."""
    update_logger.log_debug('Extracting data in {} ...'.format(DATA_PATH))
    with ZipFile(DATA_ZIP_FILE) as zip:
        zip.extractall(DATA_PATH)
