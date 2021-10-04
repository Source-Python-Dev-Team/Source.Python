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

#ifndef _CVARS_H
#define _CVARS_H

//-----------------------------------------------------------------------------
// Includes.
//-----------------------------------------------------------------------------
// C++
#include <unordered_map>
#include <vector>

// This is required for accessing m_nFlags without patching convar.h
#define private public
#include "convar.h"
#undef private

#include "utilities/sp_util.h"
#include "modules/listeners/listeners_manager.h"


//-----------------------------------------------------------------------------
// ConVar extension class.
//-----------------------------------------------------------------------------
class ConVarExt
{
public:
	static boost::shared_ptr<ConVar> __init__(const char* name, const char* value,
		const char* description, int flags, object min_value, object max_value);

	static bool HasMin(ConVar* pConVar);
	static bool HasMax(ConVar* pConVar);

	static float GetMin(ConVar* pConVar);
	static float GetMax(ConVar* pConVar);

	static void SetValue(ConVar* pConVar, bool bValue);

	static void MakePublic(ConVar* pConVar);
	static void RemovePublic(ConVar* pConVar);

	static void ChangedCallback(IConVar* var, const char* pOldValue, float flOldValue);
	static void AddChangedCallback(ConVar* pConVar, PyObject* pCallable);
	static void RemoveChangedCallback(ConVar* pConVar, PyObject* pCallable);
	static void ClearCallback();

	static bool installed;
};


#endif // _CVARS_H
