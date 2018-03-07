# ../core/update.py

"""Provides functions to update Source.Python's data files."""

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
from core import core_logger
from core import SOURCE_ENGINE_BRANCH
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
    'download_latest_data',
    'get_latest_data_checksum',
    'is_new_data_available',
    'unpack_data',
    'update_data'
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
LOADER_UPDATE_FILE = UPDATE_PATH / 'addons' / f'source-python.{BINARY_EXT}'
VDF_UPDATE_FILE = UPDATE_PATH / 'addons' / 'source-python.vdf'

CHECKSUM_URL = 'http://data.sourcepython.com/checksum.txt'
DATA_URL = 'http://data.sourcepython.com/source-python-data.zip'
ARTIFACTS_URL = 'http://builds.sourcepython.com/job/Source.Python/lastSuccessfulBuild/api/json?tree=artifacts[relativePath]'
BASE_DOWNLOAD_URL = 'http://builds.sourcepython.com/job/Source.Python/lastSuccessfulBuild/artifact/'


# =============================================================================
# >> FUNCTIONS
# =============================================================================
def get_latest_data_checksum(timeout=3):
    """Return the MD5 checksum of the latest data from the build server.

    :param float timeout:
        Number of seconds that need to pass until a timeout occurs.
    :rtype: str
    """
    with urlopen(CHECKSUM_URL, timeout=timeout) as url:
        return url.read().decode()

def download_latest_data(timeout=3):
    """Download the latest data from the build server.

    :param float timeout:
        Number of seconds that need to pass until a timeout occurs.
    """
    download_file(DATA_URL, DATA_ZIP_FILE, timeout)

def unpack_data():
    """Unpack ``source-python-data.zip``."""
    update_logger.log_debug('Extracting data in {} ...'.format(DATA_PATH))
    with ZipFile(DATA_ZIP_FILE) as zip:
        zip.extractall(DATA_PATH)

def update_data(timeout=3):
    """Download and unpack the latest data from the build server.

    Old data gets deleted before unpacking.

    :param float timeout:
        Number of seconds that need to pass until a timeout occurs.
    """
    download_latest_data(timeout)
    if SP_DATA_PATH.isdir():
        update_logger.log_debug('Removing {} ...'.format(SP_DATA_PATH))
        SP_DATA_PATH.rmtree()

    unpack_data()

def is_new_data_available(timeout=3):
    """Return ``True`` if new data is available.

    :param float timeout:
        Number of seconds that need to pass until a timeout occurs.
    :rtype: bool
    """
    if not DATA_ZIP_FILE.isfile():
        return True

    return DATA_ZIP_FILE.read_hexhash('md5') != get_latest_data_checksum(timeout)

def get_artifacts():
    """Return the artifacts of the latest build."""
    update_logger.log_debug('Getting artifacts...')
    with urlopen(ARTIFACTS_URL) as url:
        data = json.loads(url.read())

        for d in data['artifacts']:
            yield d.popitem()[1]

def get_download_url(game=SOURCE_ENGINE_BRANCH):
    """Get the latest download URL for a specific game."""
    for relative_path in get_artifacts():
        if f'-{game}-' in relative_path:
            return BASE_DOWNLOAD_URL + relative_path

    raise ValueError(f'Unable to find a download URL for game "{game}".')

def download_file(url_path, file_path, timeout=3):
    """Download a file from an URL to a specific file."""
    update_logger.log_debug(f'Downloading file ({url_path}) to {file_path} ...')
    now = time.time()

    with urlopen(url_path, timeout=timeout) as url:
        data = url.read()

    with file_path.open('wb') as f:
        f.write(data)

    update_logger.log_info(
        'File has been downloaded. Time elapsed: {:0.2f} seconds'.format(
            time.time()-now))

def clean_update_dir():
    """Clear or create the update directory."""
    if UPDATE_PATH.exists():
        for f in UPDATE_PATH.listdir():
            if f.isfile():
                f.remove()
            else:
                f.rmtree()
    else:
        UPDATE_PATH.mkdir()

def download_latest_version(timeout=3):
    """Download the latest version."""
    download_file(get_download_url(), UPDATE_ZIP_FILE, timeout)

def apply_update_stage1():
    """Apply stage 1 of the version update."""
    update_logger.log_message('Applying Source.Python update stage 1...')

    # Extract all files to the update directory
    with ZipFile(UPDATE_ZIP_FILE) as zip:
        zip.extractall(UPDATE_PATH)

    UPDATE_ZIP_FILE.remove()

    if not VDF_FILE.isfile():
        # TODO: Improve this step to update everything except the loader.
        raise ValueError(
            'source-python.vdf does not exist. Update cannot be applied if '
            'Source.Python is not loaded via the VDF file. Please copy the '
            'files from the update directory on your own.')

    update_logger.log_debug('Determining current VDF entry...')
    kv = KeyValues.load_from_file(VDF_FILE)

    # Get the current and new entry for the VDF file
    current_entry = kv.get_string('file')
    if current_entry == SP_VDF2:
        new_entry = SP_VDF1
    elif current_entry == SP_VDF1:
        new_entry = SP_VDF2
    else:
        raise ValueError(f'Unexpected entry in VDF: {current_entry}')

    update_logger.log_debug('Moving new loader binary to game directory...')

    VDF_UPDATE_FILE.remove()
    LOADER_UPDATE_FILE.move(GAME_PATH / f'{new_entry}.{BINARY_EXT}')

    kv.set_string('file', new_entry)
    kv.save_to_file(VDF_FILE)
    
    update_logger.log_message(
        'Stage 1 has been applied. Restart your server to apply stage 2.')
