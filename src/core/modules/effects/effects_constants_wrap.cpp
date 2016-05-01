/**
* =============================================================================
* Source Python
* Copyright (C) 2012-2015 Source Python Development Team.  All rights reserved.
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
#include "export_main.h"
#include "utilities/wrap_macros.h"
#include "mathlib/vector.h"
#include "tier1/utlvector.h"
#include "string_t.h"
#include "dt_send.h"
#include "game/shared/ehandle.h"
#include "isaverestore.h"
#include "datamap.h"
#include "game/shared/takedamageinfo.h"
#include "game/shared/shareddefs.h"


//-----------------------------------------------------------------------------
// Forward declarations.
//-----------------------------------------------------------------------------
void export_blood_color(scope);


//-----------------------------------------------------------------------------
// Declare the _effects._constants module.
//-----------------------------------------------------------------------------
DECLARE_SP_SUBMODULE(_effects, _constants)
{
	export_blood_color(_constants);
}


//-----------------------------------------------------------------------------
// Exports blood color constants.
//-----------------------------------------------------------------------------
void export_blood_color(scope _constants)
{
	_constants.attr("DONT_BLEED") = (long)DONT_BLEED;
	_constants.attr("BLOOD_COLOR_RED") = (long)BLOOD_COLOR_RED;
	_constants.attr("BLOOD_COLOR_YELLOW") = (long)BLOOD_COLOR_YELLOW;
	_constants.attr("BLOOD_COLOR_GREEN") = (long)BLOOD_COLOR_GREEN;
	_constants.attr("BLOOD_COLOR_MECH") = (long)BLOOD_COLOR_MECH;
}
