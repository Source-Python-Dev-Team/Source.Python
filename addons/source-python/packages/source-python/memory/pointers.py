# ../memory/pointers.py

# ============================================================================
# >> IMPORTS
# ============================================================================
# Python Imports
#   Binascii
from binascii import unhexlify
#   ConfigObj
from configobj import ConfigObj
#   Path
from path import path
#   Platform
from platform import system
#   Sys
from sys import modules

# Source.Python Imports
from paths import DATA_PATH
#   Memory
from memory import make_object
from memory.modules import ModuleData


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = [
    'Pointers'
]


# ============================================================================
# >> CLASSES
# ============================================================================
class Pointers(dict):
    '''Class used to parse pointers from an INI file.'''
    
    def __init__(self, file_path, *args, **kwargs):
        '''Parse the given file and store the parsed pointers.'''
        
        # Is the given path not a path instance?
        if not isinstance(file_path, path):
            
            # Get a path instance...
            file_path = path(file_path)
            
        # Is the given path not valid?
        if not file_path.isfile():
            
            # Get the given path from the data directory...
            file_path = DATA_PATH.joinpath(file_path)
            
            # Is the given path still not valid?
            if not file_path.isfile():
                
                # Raise an error...
                raise FileNotFoundError('No such file: {0}'.format(file_path))
                
        # Parse the given file...
        parsed_file = ConfigObj(infile=file_path, *args, **kwargs)
        
        # Loop through all pointers...
        for name in parsed_file:
            
            # Get the current pointer data for the current platform...
            data = parsed_file[name].get(system(), None)
            
            # Was the current platform not available?
            if data is None:
                
                # No need to go further...
                continue
                
            # Find the address of the current pointer into the given binary...
            pointer = ModuleData[data['binary']].find_pointer(
                unhexlify(data['identifier'].replace(' ', '')),
                    int(data['offset']), int(data.get('level', 0)))
                    
            # Get the current pointer converter...
            converter = parsed_file[name].get('converter', None)
            
            # Was no given converter?
            if converter is None:
                
                # Store the pointer as is...
                self[name] = pointer
                
            # Otherwise...
            else:
                
                # Store the converted pointer...
                self[name] = make_object(
                    getattr(modules[converter[0]], converter[1]), pointer)