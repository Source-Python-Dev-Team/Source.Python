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

#include "convar.h"

#include "utilities/sp_util.h"
#include "modules/listeners/listeners_manager.h"


//-----------------------------------------------------------------------------
// Global ConVar changed callback mapping.
//-----------------------------------------------------------------------------
typedef std::vector<object> ChangedCallbacks;
typedef std::unordered_map<std::string, ChangedCallbacks> ConVarMap;
ConVarMap g_ConVarMap;


//-----------------------------------------------------------------------------
// ConVar extension class.
//-----------------------------------------------------------------------------
class ConVarExt
{
public:
	static boost::shared_ptr<ConVar> __init__(const char* name, const char* value,
		const char* description, int flags, object min_value, object max_value)
	{
		if (!name || name[0] == '\0')
			BOOST_RAISE_EXCEPTION(PyExc_ValueError, "An empty string is not a valid ConVar name.")

		float fMin = 0;
		float fMax = 0;

		try {
			fMin = extract<float>(min_value);
		}
		catch (...) {
			PyErr_Clear();
		}

		try {
			fMax = extract<float>(max_value);
		}
		catch (...) {
			PyErr_Clear();
		}

		ConVar *pConVar = g_pCVar->FindVar(name);
		if (!pConVar)
		{
			ConVar* pConVar = new ConVar(strdup(name), strdup(value), flags,
				strdup(description), !min_value.is_none(), fMin, !max_value.is_none(), fMax);

			return boost::shared_ptr<ConVar>(pConVar, &NeverDeleteDeleter<ConVar *>);
		}

		return boost::shared_ptr<ConVar>(pConVar, &NeverDeleteDeleter<ConVar *>);
	}

	static bool HasMin(ConVar* pConVar)
	{
		float fMin;
		return pConVar->GetMin(fMin);
	}

	static bool HasMax(ConVar* pConVar)
	{
		float fMax;
		return pConVar->GetMax(fMax);
	}

	static float GetMin(ConVar* pConVar)
	{
		float fMin;
		pConVar->GetMin(fMin);
		return fMin;
	}

	static bool GetMax(ConVar* pConVar)
	{
		float fMax;
		pConVar->GetMax(fMax);
		return fMax;
	}

	static void SetValue(ConVar* pConVar, bool bValue)
	{
		pConVar->SetValue(bValue);
	}

	static void MakePublic(ConVar* pConVar)
	{
		pConVar->m_nFlags |= FCVAR_NOTIFY;
		g_pCVar->CallGlobalChangeCallbacks(pConVar, pConVar->GetString(), pConVar->GetFloat());
	}

	static void RemovePublic(ConVar* pConVar)
	{
		pConVar->m_nFlags &= ~FCVAR_NOTIFY;
		g_pCVar->CallGlobalChangeCallbacks(pConVar, pConVar->GetString(), pConVar->GetFloat());
	}

	static void ChangedCallback(IConVar* var, const char* pOldValue, float flOldValue)
	{
		ConVarMap::iterator map_it = g_ConVarMap.find(var->GetName());
		if (map_it == g_ConVarMap.end())
			return;

		ConVar* pConVar = static_cast<ConVar*>(var);

		ChangedCallbacks& callables = map_it->second;
		for (ChangedCallbacks::iterator it = callables.begin(); it != callables.end(); ++it)
		{
			BEGIN_BOOST_PY()
				(*it)(ptr(pConVar), pOldValue, pConVar->GetString());
			END_BOOST_PY_NORET()
		}
	}

	static void AddChangedCallback(ConVar* pConVar, PyObject* pCallable)
	{
		// Get the object instance of the callable
		object oCallable = object(handle<>(borrowed(pCallable)));

		ChangedCallbacks& callables = g_ConVarMap[pConVar->GetName()];
		if (!callables.size())
		{
			if (!installed)
			{
				g_pCVar->InstallGlobalChangeCallback(ChangedCallback);
				installed = true;
			}
		}
		else
		{
			for (ChangedCallbacks::iterator it = callables.begin(); it != callables.end(); ++it)
			{
				if (is_same_func(oCallable, *it))
					BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Callback already registered.")
			}
		}

		callables.push_back(oCallable);
	}

	static void RemoveChangedCallback(ConVar* pConVar, PyObject* pCallable)
	{
		ConVarMap::iterator map_it = g_ConVarMap.find(pConVar->GetName());
		if (map_it == g_ConVarMap.end())
			BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Callback not registered.")

		// Get the object instance of the callable
		object oCallable = object(handle<>(borrowed(pCallable)));

		ChangedCallbacks& callables = map_it->second;
		for (ChangedCallbacks::iterator it = callables.begin();;)
		{
			if(it == callables.end())
				BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Callback not registered.")

			if (is_same_func(oCallable, *it))
			{
				callables.erase(it);
				break;
			}
			else
			{
				++it;
			}
		}

		if (!callables.size())
		{
			g_ConVarMap.erase(map_it);
			if (!g_ConVarMap.size())
			{
				g_pCVar->RemoveGlobalChangeCallback(ChangedCallback);
				installed = false;
			}
		}
	}

	static bool installed;
};


#endif // _CVARS_H
