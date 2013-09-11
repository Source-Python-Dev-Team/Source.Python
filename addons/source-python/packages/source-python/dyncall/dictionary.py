# ../dyncall/dictionary.py

# =============================================================================
# >> IMPORTS
# =============================================================================
# Site Package Imports
#   Configobj
from configobj import ConfigObj

# Source.Python Imports
from core import GAME_NAME
from paths import DATA_PATH
#   DynCall
from dyncall.signature import Signature


# =============================================================================
# >> GLOBAL VARIABLES
# =============================================================================
# Store the game's ini file's path
_inipath = DATA_PATH.joinpath('dyncall', GAME_NAME)


# =============================================================================
# >> CLASSES
# =============================================================================
class _SignatureDictionary(dict):
    '''Dictionary to store Signature instances by name'''

    def parse_signature_ini(self, inifile):
        '''Parse an ini file and add its signatures to the dictionary'''

        # Get the ConfigObj instance of the file
        sigs = ConfigObj(inifile)

        # Loop through all functions
        for function in sigs:

            # Add the function to the dictionary
            self[sigs[function]['shortname']] = Signature(sigs[function])

# Get the main _SignatureDictionary instance
SignatureDictionary = _SignatureDictionary()

# Does the game's ini directory exist?
if _inipath.isdir():

    # Loop through all files in the directory
    for _filepath in _inipath.files():

        # Parse the ini file
        SignatureDictionary.parse_signature_ini(_filepath)
