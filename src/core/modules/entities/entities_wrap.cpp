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

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <vector>
#include "entities_wrap.h"
#include "dt_common.h"
#include "utility/sp_util.h"
#include "edict.h"
#include "boost/unordered_map.hpp"
#include "boost/algorithm/string.hpp"
#include "boost/foreach.hpp"
#include "utility/wrap_macros.h"
#include "sp_main.h"

//-----------------------------------------------------------------------------
// If these aren't defined, we get linker errors about CBaseEdict.
//-----------------------------------------------------------------------------
IChangeInfoAccessor *CBaseEdict::GetChangeAccessor()
{
	return engine->GetChangeAccessor( (const edict_t *)this );
}

const IChangeInfoAccessor *CBaseEdict::GetChangeAccessor() const
{
	return engine->GetChangeAccessor( (const edict_t *)this );
}


//-----------------------------------------------------------------------------
// IServerUnknown extension
//-----------------------------------------------------------------------------
unsigned long IServerUnknownExt::GetBaseEntity( IServerUnknown* unknown )
{
	return (unsigned long)(unknown->GetBaseEntity());
}

//-----------------------------------------------------------------------------
// edict_t extension
//-----------------------------------------------------------------------------
const char* CEdictExt::GetKeyValueString(edict_t* pEdict, const char* szName)
{
	char szResult[1024];
	CBaseEntity* pEntity = pEdict->GetUnknown()->GetBaseEntity();
	servertools->GetKeyValue(pEntity, szName, szResult, 1024);

	// Fix for field name "model". I think a string_t object is copied to szResult.
	if (strcmp(szName, "model") == 0)
		return *(char **) szResult;

	return szResult;
}

int CEdictExt::GetKeyValueInt(edict_t* pEdict, const char* szName)
{
	const char* szResult = GetKeyValueString(pEdict, szName);

	// Let's handle this with Python for better error checking
	return extract<int>(eval("lambda x: int(x)")(str(szResult)));
}

float CEdictExt::GetKeyValueFloat(edict_t* pEdict, const char* szName)
{
	const char* szResult = GetKeyValueString(pEdict, szName);

	// Let's handle this with Python for better error checking
	return extract<float>(eval("lambda x: float(x)")(str(szResult)));
}

Vector CEdictExt::GetKeyValueVector(edict_t* pEdict, const char* szName)
{
	const char* szResult = GetKeyValueString(pEdict, szName);

	// Let's handle this with Python for better error checking
	object vec = eval("lambda x: tuple(map(float, x.split(' ')))")(str(szResult));
	return Vector(extract<float>(vec[0]), extract<float>(vec[1]), extract<float>(vec[2]));
}

Color CEdictExt::GetKeyValueColor(edict_t* pEdict, const char* szName)
{
	const char* szResult = GetKeyValueString(pEdict, szName);

	object color = eval("lambda x: tuple(map(int, x.split(' ')))")(str(szResult));
	return Color(extract<int>(color[0]), extract<int>(color[1]), extract<int>(color[2]), extract<int>(color[3]));
}

bool CEdictExt::GetKeyValueBool(edict_t* pEdict, const char* szName)
{
	const char* szResult = GetKeyValueString(pEdict, szName);

	return strcmp(szResult, "1") == 0;
}

void CEdictExt::SetKeyValueColor(edict_t* pEdict, const char* szName, Color color)
{
	char string[16];

	Q_snprintf(string,sizeof(string), "%i %i %i %i", color.r(), color.g(), color.b(), color.a());

	SetKeyValue(pEdict, szName, string);
}
