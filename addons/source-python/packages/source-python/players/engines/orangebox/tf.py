# ../players/engines/orangebox/cstrike.py

"""Provides Team Fortress 2 specific Player based functionality."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
from . import Player as _Player
from configobj import ConfigObj
from core import GAME_NAME
from paths import SP_DATA_PATH

# =============================================================================
# >> CLASSES
# =============================================================================
class Player(_Player):
    caching = True
    
    # Team
    _team_data = ConfigObj(SP_DATA_PATH / 'teams' / GAME_NAME + '.ini', unrepr=True)
    teams_by_name = _team_data.get('names')
    teams_by_number = {number: alias for alias, number in teams_by_name.items()}

    @property
    def team_name(self):
        return self.teams_by_number[self.team]
    
    @team_name.setter
    def team_name(self, team_name):
        if team_id not in self.team_by_name.keys():
            raise ValueError(f"Invalid team_name: {team_name}")
        
        self.team = self.teams_by_name[team_name]

    # Mercanary class
    class_by_name = _team_data.get('classes')
    class_by_number = {number: alias for alias, number in class_by_name.items()}
    
    @property
    def player_class_name(self):
        return self.class_by_number[self.player_class]

    @player_class_name.setter
    def player_class_name(self, class_name):
        if team_id not in self.class_by_name.keys():
            raise ValueError(f"Invalid class_name: {class_name}")
        
        # Does this need to be desired_player_class?
        self.player_class = self.class_by_name[team_name]
