/**
* =============================================================================
* Source Python
* Copyright (C) 2012 Source Python Development Team.  All rights reserved.
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
#ifndef _SP_UTIL_H
#define _SP_UTIL_H

//---------------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------------
#include "eiface.h"
#include "public/game/server/iplayerinfo.h"
#include "basehandle.h"
#include "modules/memory/memory_tools.h"

// Externals
extern IVEngineServer* engine;
extern CGlobalVars*         gpGlobals;
extern IPlayerInfoManager* playerinfomanager;


//---------------------------------------------------------------------------------
// Returns True if the class name equals the given string.
//---------------------------------------------------------------------------------
inline bool CheckClassname(object obj, char* name)
{
	return strcmp(extract<char *>(obj.attr("__class__").attr("__name__")), name) == 0;
}

//---------------------------------------------------------------------------------
// Converts a Python CPointer object or an integer to an unsigned long.
//---------------------------------------------------------------------------------
inline unsigned long ExtractPyPtr(object obj)
{
	if (CheckClassname(obj, "CPointer"))
	{
		CPointer* pPtr = extract<CPointer *>(obj);
		return pPtr->get_address();
	}
	return extract<unsigned long>(obj);
}

#endif
