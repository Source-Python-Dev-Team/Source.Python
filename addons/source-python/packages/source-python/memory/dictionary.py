# ../memory/dictionary.py

# =============================================================================
# >> IMPORTS
# =============================================================================
# Site Package Imports
#   Configobj
from configobj import ConfigObj
#   Sys
import sys

# Source.Python Imports
from core import GAME_NAME
from paths import DATA_PATH
from paths import SP_DATA_PATH
#   Memory
from memory import MemoryLogger
from memory.signature import Signature


# =============================================================================
# >> GLOBAL VARIABLES
# =============================================================================
# Store the game's ini file's path
_inipath = SP_DATA_PATH.joinpath('memory', GAME_NAME)

# Get the sp.memory.dictionary logger
MemoryDictionaryLogger = MemoryLogger.dictionary


# =============================================================================
# >> CLASSES
# =============================================================================
class _SignatureDictionary(dict):
    '''Dictionary to store Signature instances by name'''

    def parse_signature_ini(self, inifile):
        '''Parse an ini file and add its signatures to the dictionary'''

        # Get the ConfigObj instance of the file
        sigs = ConfigObj(inifile)

        # Get the path to the current file
        current_path = '..' + inifile.replace(DATA_PATH, '')

        # Loop through all functions
        for function in sigs:

            # Does the current function have a shortname?
            if not 'shortname' in sigs[function]:

                # If not, log a warning
                MemoryDictionaryLogger.log_warning(
                    'Missing "shortname" in function "{0}"'.format(function) +
                    ' in file "{0}"'.format(current_path))

                # No need to go further on this function
                continue

            # Get the function's shortname
            name = sigs[function]['shortname']

            # Is the function's shortname already registered?
            if name in self:

                # Log a warning
                MemoryDictionaryLogger.log_warning('Shortname ' +
                    '"{0}" already exists in the dictionary'.format(name))

                # No need to go further on this function
                continue

            # Use try/except in case an error is encountered
            try:

                # Get the Signature instance of the current sig
                value = Signature(sigs[function])

            # Was an error encountered?
            except:

                # Get the exception
                exctype, value, trace_back = sys.exc_info()

                # Log the error as a warning
                MemoryDictionaryLogger.log_warning(
                    'Unable to store signature for "{0}"'.format(name) +
                    ' in file "{0}"'.format(current_path) +
                    ' due to the following:\n\t{0}'.format(value))

            # Was no error encountered?
            else:

                # Add the function to the dictionary
                self[name] = value

# Get the main _SignatureDictionary instance
SignatureDictionary = _SignatureDictionary()

# Does the game's ini directory exist?
if _inipath.isdir():

    # Loop through all files in the directory
    for _filepath in _inipath.files():

        # Parse the ini file
        SignatureDictionary.parse_signature_ini(_filepath)
