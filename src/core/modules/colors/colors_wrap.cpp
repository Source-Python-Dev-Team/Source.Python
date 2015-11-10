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
#include "colors.h"
#include "modules/memory/memory_tools.h"
#include "utilities/sp_util.h"


//-----------------------------------------------------------------------------
// Forward declarations.
//-----------------------------------------------------------------------------
void export_color(scope);


//-----------------------------------------------------------------------------
// Declare the _colors module.
//-----------------------------------------------------------------------------
DECLARE_SP_MODULE(_colors)
{
	export_color(_colors);
}


//-----------------------------------------------------------------------------
// Expose Color.
//-----------------------------------------------------------------------------
void export_color(scope _colors)
{
	class_<Color, Color *> Color_(
		"Color",
		"This class is used to store a 32 bit color. The color is stored in its RGBA components.",
		init<unsigned char, unsigned char, unsigned char, unsigned char>(
			(arg("r")=0, arg("g")=0, arg("b")=0, arg("a")=255),
			"Initialize the Color object.\n"
			"\n"
			":param int r: See :attr:`r`.\n"
			":param int g: See :attr:`g`.\n"
			":param int b: See :attr:`b`.\n"
			":param int a: See :attr:`a`.\n"
		)
	);
	
	// Properties...
	Color_.add_property(
		"r",
		&Color::r, 
		&IndexSetter<Color, unsigned char, 0>,
		"A value between 0 and 255 that defines the red component of the color."
	);

	Color_.add_property(
		"g", 
		&Color::g,
		&IndexSetter<Color, unsigned char, 1>,
		"A value between 0 and 255 that defines the green component of the color."
	);

	Color_.add_property(
		"b",
		&Color::b,
		&IndexSetter<Color, unsigned char, 2>,
		"A value between 0 and 255 that defines the blue component of the color."
	);

	Color_.add_property(
		"a", 
		&Color::a,
		&IndexSetter<Color, unsigned char, 3>,
		"A value between 0 and 255 that defines the alpha value (transparency)."
	);

	// Methods
	Color_.def(
		"with_alpha",
		ColorExt::WithAlpha,
		(arg("a")),
		"Return a copy of the color with a new alpha value.\n"
		"\n"
		":param int a: The new alpha value."
	);

	// Special methods...
	Color_.def(
		"__getitem__", 
		&GetItemIndexer<Color, unsigned char, 0, 3>,
		(arg("index")),
		"Return the color component at the given index (0 - 3).\n"
		"\n"
		":param int index: An index between 0 and 3 that defines the color component to get.\n"
		":return: The color component value.\n"
		":rtype: int"
	);

	Color_.def(
		"__setitem__", 
		&SetItemIndexer<Color, unsigned char, 0, 3>,
		(arg("index"), arg("value")),
		"Set the color component at the given index.\n"
		"\n"
		":param int index: An index between 0 and 3 that defines the color component to set.\n"
		":param int value: A value between 0 and 255 that defines the new color component value."
	);

	Color_.def(
		"__str__",
		&ColorExt::GetHexString,
		"Return the color as a hexadecimal string."
	);

	Color_.def(
		"__repr__",
		&ColorExt::GetRepr,
		"Return the color as a stringified tuple."
	);

	// Operators...
	Color_.def(self == self);
	Color_.def(self != self);

	// Add memory tools...
	Color_ ADD_MEM_TOOLS(Color);
}
