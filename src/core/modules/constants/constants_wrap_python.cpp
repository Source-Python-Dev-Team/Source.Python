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
#include "constants_const_wrap_python.h"
#include "constants_in_buttons_wrap_python.h"
#include "public/worldsize.h"


//-----------------------------------------------------------------------------
// Forward declarations.
//-----------------------------------------------------------------------------
void export_source_python_constants(scope constants_c);
void export_worldsize(scope constants);


//-----------------------------------------------------------------------------
// Declare the constants_c module.
//-----------------------------------------------------------------------------
DECLARE_SP_MODULE(constants)
{
	scope constants_c = scope();
	export_source_python_constants(constants_c);
	export_const(constants_c);
	export_in_buttons(constants_c);
	export_worldsize(constants_c);
}


//-----------------------------------------------------------------------------
// Expose Source.Python constants.
//-----------------------------------------------------------------------------
void export_source_python_constants(scope constants_c)
{
	constants_c.attr("SOURCE_ENGINE") = XSTRINGIFY(SOURCE_ENGINE);
}


//-----------------------------------------------------------------------------
// Expose Source.Python constants.
//-----------------------------------------------------------------------------
void export_worldsize(scope constants)
{
	constants.attr("MAX_COORD_INTEGER") = MAX_COORD_INTEGER;
	constants.attr("MIN_COORD_INTEGER") = MIN_COORD_INTEGER;

	constants.attr("MAX_COORD_FRACTION") = MAX_COORD_FRACTION;
	constants.attr("MIN_COORD_FRACTION") = MIN_COORD_FRACTION;

	constants.attr("MAX_COORD_FLOAT") = MAX_COORD_FLOAT;
	constants.attr("MIN_COORD_FLOAT") = MIN_COORD_FLOAT;

	constants.attr("COORD_EXTENT") = COORD_EXTENT;

	constants.attr("MAX_TRACE_LENGTH") = MAX_TRACE_LENGTH;

	constants.attr("MAX_COORD_RANGE") = MAX_COORD_RANGE;
}
