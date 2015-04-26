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

#ifndef _KEYVALUES_WRAP_H
#define _KEYVALUES_WRAP_H

//-----------------------------------------------------------------------------
// Includes.
//-----------------------------------------------------------------------------
#include "KeyValues.h"
#include "filesystem.h"


//---------------------------------------------------------------------------------
// External variables.
//---------------------------------------------------------------------------------
extern IFileSystem* filesystem;


//-----------------------------------------------------------------------------
// KeyValues extension class.
//-----------------------------------------------------------------------------
class KeyValuesExt
{
public:
	static bool GetBool(KeyValues* pKeyValues, const char * szName = NULL, bool bDefault = false)
	{
		return pKeyValues->GetInt(szName, bDefault);
	}

	static void SetBool(KeyValues* pKeyValues, const char * szName, bool bValue)
	{
		pKeyValues->SetInt(szName, bValue);
	}

	static bool LoadFromFile(KeyValues* pKeyValues, const char * szFile)
	{
		return pKeyValues->LoadFromFile(filesystem, szFile);
	}

	static bool SaveToFile(KeyValues* pKeyValues, const char * szFile)
	{
		return pKeyValues->SaveToFile(filesystem, szFile);
	}

	static Color GetColor(KeyValues* pKeyValues, const char* szKeyName, const Color &defaultColor = Color())
	{
		if (!pKeyValues->FindKey(szKeyName))
			return defaultColor;

		return pKeyValues->GetColor(szKeyName);
	}
};


#endif // _KEYVALUES_WRAP_H
