# ../entities/attributes.py

# =============================================================================
# >> IMPORTS
# =============================================================================
# Site-Package Imports
#   ConfigObj
from configobj import ConfigObj

# Source.Python Imports
from core import GAME_NAME
from paths import SP_DATA_PATH


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = []


# =============================================================================
# >> CLASSES
# =============================================================================
class EntityAttributes(dict):
    '''Base Attribute class used to interact with
        entity's based off of ini data files.'''

    '''Each class that inherits from EntityAttributes
                    must have the following attributes:
        type        -   used to know which directory within data to get values
        unrepr      -   used to know what to have ConfigObj unrepr set to
        instance    -   used to know which class to use to create the objects
    '''

    def __missing__(self, entity):
        '''Called the first time an entity is added to the dictionary'''

        # Get all attributes for the given entity
        values = self[entity] = self._retrieve_attributes(entity)

        # Return the attributes and their values
        return values

    def get_game_attributes(self, args):
        '''Returns all attributes for the given entities'''

        # Create an empty dictionary
        values = dict()

        # Loop through all given entities
        for arg in args:

            # Add the entities to the dictionary
            values.update(self[arg])

        # Return all attributes for the given entities
        return values

    def _retrieve_attributes(self, entity):
        '''Retrieves all attributes for the given entity'''

        # Create an empty dictionary
        game_attributes = dict()

        # Get the path to the entity's attributes
        inifile = SP_DATA_PATH.joinpath(self.type, entity, GAME_NAME + '.ini')

        # Does the file exist?
        if not inifile.isfile():

            # Return the empty dictionary
            return game_attributes

        # Get the file's contents
        ini = ConfigObj(inifile, unrepr=self.unrepr)

        # Loop through all items in the file
        for key in ini:

            # Add the item to the dictionary
            game_attributes[key] = self.instance(ini[key])

        # Return the dictionary
        return game_attributes
