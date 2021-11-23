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
#include "icvar.h"
#include "utilities/convar.h"
#include "utilities/sp_util.h"
#include "modules/commands/commands_server.h"


//-----------------------------------------------------------------------------
// ICvar shared extension class.
//-----------------------------------------------------------------------------
class ICVarSharedExt
{
public:
	static ConVar *FindVar(ICvar *pCVar, const char *szName)
	{
		ConCommandBase *pBase = pCVar->FindCommandBase(szName);
		if (pBase)
		{
			if (pBase->IsCommand()) {
				CServerCommandManager *pManager = dynamic_cast<CServerCommandManager *>(pBase);
				if (pManager && pManager->m_pOldCommand && !pManager->m_pOldCommand->IsCommand()) {
					return static_cast<ConVar *>(pManager->m_pOldCommand);
				}
			}
			else {
				return static_cast<ConVar *>(pBase);
			}
		}

		return NULL;
	};
};


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

		ConVar *pConVar = ICVarSharedExt::FindVar(g_pCVar, name);
		if (!pConVar)
		{
			ConVar* pConVar = new ConVar(strdup(name), strdup(value), flags,
				strdup(description), !min_value.is_none(), fMin, !max_value.is_none(), fMax);

			return boost::shared_ptr<ConVar>(pConVar, &Deleter);
		}

		return boost::shared_ptr<ConVar>(pConVar, &NeverDeleteDeleter<ConVar *>);
	}

	static void Deleter(ConVar *pConVar)
	{
		ConCommandBase *pBase = g_pCVar->FindCommandBase(pConVar->GetName());
		if (pBase) {
			CServerCommandManager *pManager = dynamic_cast<CServerCommandManager *>(pBase);
			if (pManager && pManager->m_pOldCommand == pConVar) {
				pManager->m_pOldCommand = NULL;
			}
			else if (pBase == pConVar) {
				g_pCVar->UnregisterConCommand(pConVar);
			}
		}

		delete pConVar;
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
		AddConCommandFlags(pConVar, FCVAR_NOTIFY);
		g_pCVar->CallGlobalChangeCallbacks(pConVar, pConVar->GetString(), pConVar->GetFloat());
	}

	static void RemovePublic(ConVar* pConVar)
	{
		RemoveConCommandFlags(pConVar, FCVAR_NOTIFY);
		g_pCVar->CallGlobalChangeCallbacks(pConVar, pConVar->GetString(), pConVar->GetFloat());
	}
};


#endif // _CVARS_H
