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

#ifndef _CONVERSIONS_WRAP_H
#define _CONVERSIONS_WRAP_H

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "edict.h"
#include "basehandle.h"
#include "eiface.h"
#include "public/game/server/iplayerinfo.h"
#include "modules/memory/memory_tools.h"

//-----------------------------------------------------------------------------
// Externals
//-----------------------------------------------------------------------------
extern IVEngineServer* engine;
extern CGlobalVars*	   gpGlobals;
extern IPlayerInfoManager* playerinfomanager;

extern unsigned long ExtractPyPtr(object obj);

//-----------------------------------------------------------------------------
// Forward declarations
//-----------------------------------------------------------------------------
edict_t* EdictFromIndex( unsigned int iEntIndex );
edict_t* EdictFromUserid( unsigned int userid );
edict_t* EdictFromPlayerInfo( IPlayerInfo* playerinfo );

IPlayerInfo* PlayerInfoFromIndex( unsigned int index );
IPlayerInfo* PlayerInfoFromUserid( unsigned int userid );

//-----------------------------------------------------------------------------
// Functions that return an entity's index.
//-----------------------------------------------------------------------------
inline unsigned int IndexFromEdict( edict_t* pEdict )
{
#if( SOURCE_ENGINE >= 2 )
	return (unsigned int) (pEdict - gpGlobals->pEdicts);
#else
	return (unsigned int) engine->IndexOfEdict(pEdict);
#endif
}

inline unsigned int IndexFromBaseHandle( CBaseHandle* basehandle )
{
	return basehandle->GetEntryIndex();
}

inline unsigned int IndexFromIntHandle( int iHandle )
{
	CBaseHandle hHandle(iHandle);
	unsigned int iIndex = hHandle.GetEntryIndex();
	edict_t *pEntity = EdictFromIndex(iIndex);
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

inline unsigned int IndexFromPointer( object oPtr )
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
	return IndexFromEdict(pEdict);
}

inline unsigned int IndexFromUserid( unsigned int userid )
{
	edict_t* pEdict = EdictFromUserid(userid);
	return pEdict ? IndexFromEdict(pEdict) : NULL;
}

inline unsigned int IndexFromPlayerInfo( IPlayerInfo* playerinfo )
{
	return IndexFromEdict(EdictFromPlayerInfo(playerinfo));
}

//-----------------------------------------------------------------------------
// Functions that return an entity's edict_t instance.
//-----------------------------------------------------------------------------
inline edict_t* EdictFromIndex( unsigned int iEntIndex )
{
#if( SOURCE_ENGINE >= 2 )
	if(iEntIndex < (unsigned int) gpGlobals->maxEntities)
	{
		return (edict_t *) (gpGlobals->pEdicts + iEntIndex);
	}

	return NULL;
#else
	return engine->PEntityOfEntIndex(iEntIndex);
#endif
}

inline edict_t* EdictFromBaseHandle( CBaseHandle* basehandle )
{
	return EdictFromIndex(basehandle->GetEntryIndex());
}

inline edict_t* EdictFromIntHandle( int inthandle )
{
	return EdictFromIndex(IndexFromIntHandle(inthandle));
}

inline edict_t* EdictFromPointer( object pointer )
{
	return EdictFromIndex(IndexFromPointer(pointer));
}

inline edict_t* EdictFromUserid( unsigned int userid )
{
	edict_t* pEdict;
	for(int i=1; i <= gpGlobals->maxClients; ++i)
	{
		pEdict = EdictFromIndex(i);
		if (pEdict && !pEdict->IsFree() &&
			strcmp(pEdict->GetClassName(), "player") == 0 &&
			engine->GetPlayerUserId(pEdict) == userid)
		{
			return pEdict;
		}
	}

	return NULL;
}

inline edict_t* EdictFromPlayerInfo( IPlayerInfo* playerinfo )
{
	return playerinfo ? EdictFromUserid(playerinfo->GetUserID()) : NULL;
}

//-----------------------------------------------------------------------------
// Functions that return an entity's CBaseHandle instance.
//-----------------------------------------------------------------------------
inline const CBaseHandle BaseHandleFromIndex( unsigned int index )
{
	return EdictFromIndex(index)->GetNetworkable()->GetEntityHandle()->GetRefEHandle();
}

inline const CBaseHandle BaseHandleFromEdict( edict_t* edict )
{
	return edict->GetNetworkable()->GetEntityHandle()->GetRefEHandle();
}

inline const CBaseHandle BaseHandleFromIntHandle( int inthandle )
{
	return CBaseHandle(inthandle);
}

inline const CBaseHandle BaseHandleFromPointer( object pointer )
{
	return EdictFromIndex(IndexFromPointer(pointer))->GetNetworkable()->GetEntityHandle()->GetRefEHandle();
}

inline const CBaseHandle BaseHandleFromUserid( unsigned int userid )
{
	return EdictFromUserid(userid)->GetNetworkable()->GetEntityHandle()->GetRefEHandle();
}

inline const CBaseHandle BaseHandleFromPlayerInfo( IPlayerInfo* playerinfo )
{
	return EdictFromPlayerInfo(playerinfo)->GetNetworkable()->GetEntityHandle()->GetRefEHandle();
}

//-----------------------------------------------------------------------------
// Functions that return an entity's integer handle.
//-----------------------------------------------------------------------------
inline int IntHandleFromIndex( unsigned int index )
{
	return EdictFromIndex(index)->GetNetworkable()->GetEntityHandle()->GetRefEHandle().ToInt();
}

inline int IntHandleFromEdict( edict_t* edict )
{
	return edict->GetNetworkable()->GetEntityHandle()->GetRefEHandle().ToInt();
}

inline int IntHandleFromBaseHandle( CBaseHandle* basehandle )
{
	return basehandle->ToInt();
}

inline int IntHandleFromPointer( object pointer )
{
	return EdictFromIndex(IndexFromPointer(pointer))->GetNetworkable()->GetEntityHandle()->GetRefEHandle().ToInt();
}

inline int IntHandleFromUserid( unsigned int userid )
{
	return EdictFromUserid(userid)->GetNetworkable()->GetEntityHandle()->GetRefEHandle().ToInt();
}

inline int IntHandleFromPlayerInfo( IPlayerInfo* playerinfo )
{
	return EdictFromPlayerInfo(playerinfo)->GetNetworkable()->GetEntityHandle()->GetRefEHandle().ToInt();
}

//-----------------------------------------------------------------------------
// Functions that return an entity's CPointer instance.
//-----------------------------------------------------------------------------
inline CPointer* PointerFromIndex( unsigned int index )
{
	return new CPointer((unsigned long)(EdictFromIndex(index)->GetNetworkable()->GetBaseEntity()));
}

inline CPointer* PointerFromEdict( edict_t* edict )
{
	return new CPointer((unsigned long)(edict->GetNetworkable()->GetBaseEntity()));
}

inline CPointer* PointerFromBaseHandle( CBaseHandle* basehandle )
{
	return new CPointer((unsigned long)(EdictFromIndex(basehandle->GetEntryIndex())->GetNetworkable()->GetBaseEntity()));
}

inline CPointer* PointerFromIntHandle( int inthandle )
{
	return new CPointer((unsigned long)(EdictFromIndex(IndexFromIntHandle(inthandle))->GetNetworkable()->GetBaseEntity()));
}

inline CPointer* PointerFromUserid( unsigned int userid )
{
	return new CPointer((unsigned long)(EdictFromPlayerInfo(PlayerInfoFromUserid(userid))->GetNetworkable()->GetBaseEntity()));
}

inline CPointer* PointerFromPlayerInfo( IPlayerInfo* playerinfo )
{
	return new CPointer((unsigned long)(EdictFromPlayerInfo(playerinfo)->GetNetworkable()->GetBaseEntity()));
}

//-----------------------------------------------------------------------------
// Functions that return an player's userid.
//-----------------------------------------------------------------------------
inline unsigned int UseridFromIndex( unsigned int index )
{
	return PlayerInfoFromIndex(index)->GetUserID();
}

inline unsigned int UseridFromEdict( edict_t* edict )
{
	return PlayerInfoFromIndex(IndexFromEdict(edict))->GetUserID();
}

inline unsigned int UseridFromBaseHandle( CBaseHandle* basehandle )
{
	return PlayerInfoFromIndex(basehandle->GetEntryIndex())->GetUserID();
}

inline unsigned int UseridFromIntHandle( int inthandle )
{
	return PlayerInfoFromIndex(IndexFromIntHandle(inthandle))->GetUserID();
}

inline unsigned int UseridFromPointer( object pointer )
{
	return PlayerInfoFromIndex(IndexFromPointer(pointer))->GetUserID();
}

inline unsigned int UseridFromPlayerInfo( IPlayerInfo* playerinfo )
{
	return playerinfo->GetUserID();
}

//-----------------------------------------------------------------------------
// Functions that return an player's IPlayerInfo instance.
//-----------------------------------------------------------------------------
inline IPlayerInfo* PlayerInfoFromIndex( unsigned int index )
{
	if (index < 1 || index > (unsigned int) gpGlobals->maxClients)
	{
		return NULL;
	}

	edict_t* pEdict = EdictFromIndex(index);
	if (pEdict && !pEdict->IsFree() &&
		strcmp(pEdict->GetClassName(), "player") == 0)
	{
		return playerinfomanager->GetPlayerInfo(pEdict);
	}

	return NULL;
}

inline IPlayerInfo* PlayerInfoFromEdict( edict_t* edict )
{
	return PlayerInfoFromIndex(IndexFromEdict(edict));
}

inline IPlayerInfo* PlayerInfoFromBaseHandle( CBaseHandle* basehandle )
{
	return PlayerInfoFromIndex(basehandle->GetEntryIndex());
}

inline IPlayerInfo* PlayerInfoFromIntHandle( int inthandle )
{
	return PlayerInfoFromIndex(IndexFromIntHandle(inthandle));
}

inline IPlayerInfo* PlayerInfoFromPointer( object pointer )
{
	return PlayerInfoFromIndex(IndexFromPointer(pointer));
}

inline IPlayerInfo* PlayerInfoFromUserid( unsigned int userid )
{
	edict_t* pEdict = EdictFromUserid(userid);
	return pEdict ? playerinfomanager->GetPlayerInfo(pEdict) : NULL;
}

#endif // _CONVERSIONS_WRAP_H
