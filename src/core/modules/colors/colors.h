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

#ifndef COLORS_H
#define COLORS_H

//-----------------------------------------------------------------------------
// Includes.
//-----------------------------------------------------------------------------
#include "Color.h"


//-----------------------------------------------------------------------------
// Color extension class.
//-----------------------------------------------------------------------------
class ColorExt
{
public:
	static Color WithAlpha(const Color& color, unsigned char a)
	{
		return Color(color.r(), color.g(), color.b(), a);
	}

	static str GetHexString(const Color& color)
	{
		char buffer[10];
		if (color.a() == 255)
			sprintf(buffer, "\x07%02X%02X%02X", color.r(), color.g(), color.b());
		else
			sprintf(buffer, "\x08%02X%02X%02X%02X", color.r(), color.g(), color.b(), color.a());

		return str(buffer);
	}

	static str GetRepr(const Color& color)
	{
		return str(tuple(color));
	}
};


#endif // _COLORS_WRAP_H
