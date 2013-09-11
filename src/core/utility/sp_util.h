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

//---------------------------------------------------------------------------------
// Globals
//---------------------------------------------------------------------------------
extern IVEngineServer* engine;
extern CGlobalVars*	   gpGlobals;
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

//---------------------------------------------------------------------------------
// Returns the index of an entity.
//---------------------------------------------------------------------------------
inline int IndexOfEdict(const edict_t* pEdict)
{
#if( SOURCE_ENGINE >= 2 )
	return (int)(pEdict - gpGlobals->pEdicts);
#else
	return (int)engine->IndexOfEdict(pEdict);
#endif
}

//---------------------------------------------------------------------------------
// Returns the entity of an entity index.
//---------------------------------------------------------------------------------
inline edict_t* PEntityOfEntIndex(int iEntIndex)
{
#if( SOURCE_ENGINE >= 2 )
	if(iEntIndex >= 0 && iEntIndex < gpGlobals->maxEntities)
	{
		return (edict_t *)(gpGlobals->pEdicts + iEntIndex);
	}

	return NULL;
#else
	return engine->PEntityOfEntIndex(iEntIndex);
#endif
}

//---------------------------------------------------------------------------------
// Returns the edict instance given a userid.
//---------------------------------------------------------------------------------
inline edict_t* EdictOfUserid(int userid)
{
	edict_t* pEdict;
	for(int i=1; i <= gpGlobals->maxClients; ++i)
	{
		pEdict = PEntityOfEntIndex(i);
		if (pEdict && !pEdict->IsFree() &&
			strcmp(pEdict->GetClassName(), "player") == 0 &&
			engine->GetPlayerUserId(pEdict) == userid)
		{
			return pEdict;
		}
	}

	return NULL;
}

//---------------------------------------------------------------------------------
// Returns the playerinfo instance given a userid.
//---------------------------------------------------------------------------------
inline IPlayerInfo* PlayerOfUserid(int userid)
{
	edict_t* pEdict = EdictOfUserid(userid);
	if (!pEdict)
	{
		return NULL;
	}

	return playerinfomanager->GetPlayerInfo(pEdict);
}

//---------------------------------------------------------------------------------
// Returns the index of a userid.
//---------------------------------------------------------------------------------
inline int IndexOfUserid(int userid)
{
	edict_t* pEdict = EdictOfUserid(userid);
	if (!pEdict)
	{
		return NULL;
	}

	return IndexOfEdict(pEdict);
}

//---------------------------------------------------------------------------------
// Returns the playerinfo instance given a player(entity) index.
//---------------------------------------------------------------------------------
inline IPlayerInfo* PlayerOfIndex(int index)
{
	if (index < 1 || index > gpGlobals->maxClients)
	{
		return NULL;
	}

	edict_t* pEdict = PEntityOfEntIndex(index);
	if (pEdict && !pEdict->IsFree() &&
		strcmp(pEdict->GetClassName(), "player") == 0)
	{
		return playerinfomanager->GetPlayerInfo(pEdict);
	}

	return NULL;
}

//---------------------------------------------------------------------------------
// Returns the edict instance given a playerinfo instance.
//---------------------------------------------------------------------------------
inline edict_t* EdictOfPlayer(IPlayerInfo* playerInfo)
{
	if (!playerInfo)
	{
		return NULL;
	}
	return EdictOfUserid(playerInfo->GetUserID());
}

//---------------------------------------------------------------------------------
// Returns the index of a handle from integer form
//---------------------------------------------------------------------------------
inline unsigned int IndexOfIntHandle(int iHandle)
{
	CBaseHandle hHandle(iHandle);
	unsigned int iIndex = hHandle.GetEntryIndex();
	edict_t *pEntity = PEntityOfEntIndex(iIndex);
	if (!pEntity || pEntity->IsFree())
	{
		return NULL;
	}
	IServerNetworkable *pNetworkable = pEntity->GetNetworkable();
	IHandleEntity *pEnt = pNetworkable->GetEntityHandle();
	if (!pEnt)
	{
		return NULL;
	}
	const CBaseHandle hTestHandle = pEnt->GetRefEHandle();
	if (!hTestHandle.IsValid())
	{
		return NULL;
	}
	if (hHandle.GetSerialNumber() != hTestHandle.GetSerialNumber())
	{
		return NULL;
	}
	return iIndex;
}

//---------------------------------------------------------------------------------
// Returns the index of a pointer
//---------------------------------------------------------------------------------
inline unsigned int index_of_pointer(object oPtr)
{
	unsigned long ulPointer = ExtractPyPtr(oPtr);
	IServerUnknown *pUnknown = (IServerUnknown *) ulPointer;
	IServerNetworkable *pNetworkable = pUnknown->GetNetworkable();
	if (!pNetworkable)
	{
		return NULL;
	}
	edict_t* pEdict = pNetworkable->GetEdict();
	if (!pEdict)
	{
		return NULL;
	}
	return IndexOfEdict(pEdict);
}

#endif
