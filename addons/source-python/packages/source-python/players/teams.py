# ../players/teams.py

"""Provides player team functionality."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Site-Package Imports
#   ConfigObj
from configobj import ConfigObj

# Source.Python Imports
#   Core
from core import GAME_NAME
#   Paths
from paths import SP_DATA_PATH


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('team_managers',
           'teams_by_name',
           'teams_by_number',
           )


# =============================================================================
# >> GLOBAL VARIABLES
# =============================================================================
# Get the game's team data
_team_data = ConfigObj(
    SP_DATA_PATH / 'teams' / GAME_NAME + '.ini', unrepr=True)

# Store the game specific player teams by name
teams_by_name = _team_data.get('names', {'un': 0, 'spec': 1, 't': 2, 'ct': 3})

# Store the game specific player teams by number
teams_by_number = {number: alias for alias, number in teams_by_name.items()}

# Store the team manager names
team_managers = _team_data.get('manager', ())

# Make the managers an iterable if it is a string
if isinstance(team_managers, str):
    team_managers = [team_managers]

# Set the managers as a frozenset, since it should never be modified
team_managers = frozenset(team_managers)
