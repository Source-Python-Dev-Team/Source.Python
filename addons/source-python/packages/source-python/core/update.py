# ../core/update.py

"""Provides functions to update Source.Python's data files."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Python Imports
from zipfile import ZipFile
from urllib.request import urlopen

# Source.Python Imports
#   Core
from core import core_logger
#   Paths
from paths import DATA_PATH
from paths import SP_DATA_PATH


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = (
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

DATA_ZIP_FILE = DATA_PATH / 'source-python-data.zip'
CHECKSUM_URL = 'http://data.sourcepython.com/checksum.txt'
DATA_URL = 'http://data.sourcepython.com/source-python-data.zip'


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
    update_logger.log_debug('Downloading data to {} ...'.format(DATA_ZIP_FILE))
    with urlopen(DATA_URL, timeout=timeout) as url:
        data = url.read()

    with DATA_ZIP_FILE.open('wb') as f:
        f.write(data)

def unpack_data():
    """Unpack ``source-python-data.zip``."""
    update_logger.log_debug('Extracting data in {} ...'.format(DATA_PATH))
    with ZipFile(DATA_ZIP_FILE) as zip:
        zip.extractall(DATA_PATH)

def update_data(timeout=3):
    """Download and unpack the latest data from the build server.

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
