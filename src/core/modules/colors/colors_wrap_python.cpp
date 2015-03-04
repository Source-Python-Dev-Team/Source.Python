/**
* =============================================================================
* Source Python
* Copyright (C) 2015 Source Python Development Team.  All rights reserved.
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
#include "colors_wrap.h"
#include "modules/memory/memory_tools.h"
#include "utility/sp_util.h"


//-----------------------------------------------------------------------------
// Forward declarations.
//-----------------------------------------------------------------------------
void export_color();


//-----------------------------------------------------------------------------
// Declare the _colors module.
//-----------------------------------------------------------------------------
DECLARE_SP_MODULE(_colors)
{
	export_color();
}


//-----------------------------------------------------------------------------
// Expose Color.
//-----------------------------------------------------------------------------
void export_color()
{
	class_<Color, Color *> Color_(
		"Color",
		init<unsigned char, unsigned char, unsigned char, unsigned char>(
			(arg("r")=0, arg("g")=0, arg("b")=0, arg("a")=255)
		)
	);
	
	// Properties...
	Color_.add_property("r", &Color::r, &IndexSetter<Color, unsigned char, 0>);
	Color_.add_property("g", &Color::g, &IndexSetter<Color, unsigned char, 1>);
	Color_.add_property("b", &Color::b, &IndexSetter<Color, unsigned char, 2>);
	Color_.add_property("a", &Color::a, &IndexSetter<Color, unsigned char, 3>);

	// Methods
	Color_.def(
		"with_alpha",
		ColorExt::WithAlpha,
		"Returns a copy of the color with a new alpha value."
	);

	// Special methods...
	Color_.def("__getitem__", &GetItemIndexer<Color, unsigned char, 0, 3>);
	Color_.def("__setitem__", &SetItemIndexer<Color, unsigned char, 0, 3>);
	Color_.def(
		"__str__",
		&ColorExt::GetHexString,
		"Returns the color as a hex string."
	);

	// Operators...
	Color_.def(self == self);
	Color_.def(self != self);

	// Add memory tools...
	Color_ ADD_MEM_TOOLS(Color, "Color");
}
