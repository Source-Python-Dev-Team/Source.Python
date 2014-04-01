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
#include "tier0/basetypes.h"
#include "Color.h"
#include "modules/memory/memory_tools.h"
#include "utility/sp_util.h"


//-----------------------------------------------------------------------------
// Forward declarations.
//-----------------------------------------------------------------------------
void export_interval();
void export_color();


//-----------------------------------------------------------------------------
// Declare the basetypes_c module.
//-----------------------------------------------------------------------------
DECLARE_SP_MODULE(basetypes_c)
{
	export_interval();
	export_color();
}


//-----------------------------------------------------------------------------
// Expose interval_t.
//-----------------------------------------------------------------------------
void export_interval()
{
	class_<interval_t, interval_t *> Interval("Interval");
	
	// Properties...
	Interval.def_readwrite("start", &interval_t::start);
	Interval.def_readwrite("range", &interval_t::range);
	
	// Add memory tools...
	Interval ADD_MEM_TOOLS(interval_t, "Interval");
}


//-----------------------------------------------------------------------------
// Expose Color.
//-----------------------------------------------------------------------------
void export_color()
{
	class_<Color, Color *> Color_("Color", init<int, int, int, optional<int>>());

	// Properties...
	Color_.add_property("r", &Color::r, &IndexSetter<Color, unsigned char, 0>);
	Color_.add_property("g", &Color::g, &IndexSetter<Color, unsigned char, 1>);
	Color_.add_property("b", &Color::b, &IndexSetter<Color, unsigned char, 2>);
	Color_.add_property("a", &Color::a, &IndexSetter<Color, unsigned char, 3>);

	// Special methods...
	Color_.def("__getitem__", &GetItemIndexer<Color, unsigned char, 0, 3>);
	Color_.def("__setitem__", &SetItemIndexer<Color, unsigned char, 0, 3>);

	// Operators...
	Color_.def(self == self);
	Color_.def(self != self);

	// Add memory tools...
	Color_ ADD_MEM_TOOLS(Color, "Color");
}
