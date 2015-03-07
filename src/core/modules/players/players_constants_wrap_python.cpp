/**
* =============================================================================
* Source Python
* Copyright (C) 2014 Source Python Development Team.  All rights reserved.
* =============================================================================
*
* This program is free software; you can redistribute it and/or modify it under
* the terms of the GNU General Public License, version 3.0, as published by the
* Free Software Foundation.
*
* This program is distributed in the hope that it will be useful, but WITHOUT
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
* FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
* details.
*
* You should have received a copy of the GNU General Public License along with
* this program.  If not, see <http://www.gnu.org/licenses/>.
*
* As a special exception, the Source Python Team gives you permission
* to link the code of this program (as well as its derivative works) to
* "Half-Life 2," the "Source Engine," and any Game MODs that run on software
* by the Valve Corporation.  You must obey the GNU General Public License in
* all respects for all other code used.  Additionally, the Source.Python
* Development Team grants this exception to all derivative works.
*/

//-----------------------------------------------------------------------------
// Includes.
//-----------------------------------------------------------------------------
#include "modules/export_main.h"
#include "utility/wrap_macros.h"
#include "const.h"


//-----------------------------------------------------------------------------
// Forward declarations.
//-----------------------------------------------------------------------------
void export_players_flags(scope);
void export_life_states(scope);


//-----------------------------------------------------------------------------
// Declare the _entities._constants module.
//-----------------------------------------------------------------------------
DECLARE_SP_SUBMODULE(_players, _constants)
{
	export_players_flags(_constants);
	export_life_states(_constants);
}


//-----------------------------------------------------------------------------
// Expose player flags.
//-----------------------------------------------------------------------------
void export_players_flags(scope _constants)
{
	// Player flags constants...
	_constants.attr("FL_ONGROUND") = FL_ONGROUND;
	_constants.attr("FL_DUCKING") = FL_DUCKING;
	_constants.attr("FL_WATERJUMP") = FL_WATERJUMP;
	_constants.attr("FL_ONTRAIN") = FL_ONTRAIN;
	_constants.attr("FL_INRAIN") = FL_INRAIN;
	_constants.attr("FL_FROZEN") = FL_FROZEN;
	_constants.attr("FL_ATCONTROLS") = FL_ATCONTROLS;
	_constants.attr("FL_CLIENT") = FL_CLIENT;
	_constants.attr("FL_FAKECLIENT") = FL_FAKECLIENT;
	_constants.attr("FL_INWATER") = FL_INWATER;
}


//-----------------------------------------------------------------------------
// Expose life states.
//-----------------------------------------------------------------------------
void export_life_states(scope _constants)
{
	_constants.attr("LIFE_ALIVE") = LIFE_ALIVE;
	_constants.attr("LIFE_DYING") = LIFE_DYING;
	_constants.attr("LIFE_DEAD") = LIFE_DEAD;
	_constants.attr("LIFE_RESPAWNABLE") = LIFE_RESPAWNABLE;
	_constants.attr("LIFE_DISCARDBODY") = LIFE_DISCARDBODY;
}