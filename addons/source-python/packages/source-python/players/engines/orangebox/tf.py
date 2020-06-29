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

    _teams_names = _team_data.get('names')
    _class_names = _team_data.get('classes')

    @property
    def team_name(self):
        teams_by_number = dict(zip(self._teams_names.values(), self._teams_names.keys()))
        return teams_by_number[self.team]
    
    @team_name.setter
    def team_name(self, team_name):
        if team_id not in self._teams_names:
            raise ValueError(f"Invalid team_name: {team_name}")
        
        self.team = self._teams_names[team_name]
    
    @property
    def player_class_name(self):
        class_by_number = dict(zip(self._class_names.values(), self._class_names.keys()))
        return class_by_number[self.player_class]

    @player_class_name.setter
    def player_class_name(self, class_name):
        if team_id not in self._class_names:
            raise ValueError(f"Invalid class_name: {class_name}")
        
        # Does this need to be desired_player_class or should there be a setter for desired class?
        self.player_class = self._class_names[team_name]
