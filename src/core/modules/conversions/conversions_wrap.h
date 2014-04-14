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

//-----------------------------------------------------------------------------
// Forward declarations
//-----------------------------------------------------------------------------
CBaseHandle BaseHandleFromEdict( edict_t* edict );
edict_t* EdictFromIndex( unsigned int iEntIndex );
edict_t* EdictFromUserid( unsigned int userid );
edict_t* EdictFromPlayerInfo( IPlayerInfo* playerinfo );
unsigned int UseridFromPlayerInfo( IPlayerInfo* playerinfo );
int IntHandleFromBaseHandle( CBaseHandle basehandle );
CPointer* PointerFromEdict( edict_t* edict );
IPlayerInfo* PlayerInfoFromIndex( unsigned int index );
IPlayerInfo* PlayerInfoFromPointer( CPointer* pPtr );

//-----------------------------------------------------------------------------
// Functions that return an entity's index.
//-----------------------------------------------------------------------------
inline unsigned int IndexFromEdict( edict_t* pEdict )
{
	if (!pEdict || pEdict->IsFree())
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to retrieve index from edict.")

#ifdef ENGINE_ORANGEBOX
	return (unsigned int) engine->IndexOfEdict(pEdict);
#else
	return (unsigned int) (pEdict - gpGlobals->pEdicts);
#endif
}

inline unsigned int IndexFromBaseHandle( CBaseHandle basehandle )
{
	if (!basehandle.IsValid())
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to retrieve index from handle.")

	return basehandle.GetEntryIndex();
}

inline unsigned int IndexFromIntHandle( int iHandle )
{
	CBaseHandle hHandle(iHandle);
	unsigned int iIndex = hHandle.GetEntryIndex();
	const CBaseHandle hTestHandle = BaseHandleFromEdict(EdictFromIndex(iIndex));
	if (!hTestHandle.IsValid() || (hHandle.GetSerialNumber() != hTestHandle.GetSerialNumber()))
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to retrieve index from int handle.")

	return iIndex;
}

inline unsigned int IndexFromPointer( CPointer* pPtr )
{
	if (!pPtr || !pPtr->IsValid())
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to retrieve index from pointer.")

	IServerUnknown *pUnknown = (IServerUnknown *) pPtr->m_ulAddr;
	IServerNetworkable *pNetworkable = pUnknown->GetNetworkable();
	if (!pNetworkable)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to retrieve index from pointer.")

	return IndexFromEdict(pNetworkable->GetEdict());
}

inline unsigned int IndexFromUserid( unsigned int userid )
{
	return IndexFromEdict(EdictFromUserid(userid));
}

inline unsigned int IndexFromPlayerInfo( IPlayerInfo* playerinfo )
{
	if (!playerinfo)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to retrieve index from playerinfo.")

	return IndexFromEdict(EdictFromPlayerInfo(playerinfo));
}

//-----------------------------------------------------------------------------
// Functions that return an entity's edict_t instance.
//-----------------------------------------------------------------------------
inline edict_t* EdictFromIndex( unsigned int iEntIndex )
{
	edict_t *pEdict = NULL;
	if(iEntIndex < (unsigned int) gpGlobals->maxEntities)
	{
#ifdef ENGINE_ORANGEBOX
		pEdict = engine->PEntityOfEntIndex(iEntIndex);
#else
		pEdict = (edict_t *) (gpGlobals->pEdicts + iEntIndex);
#endif
	}

	if (!pEdict || pEdict->IsFree() || !pEdict->GetUnknown())
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to retrieve edict from index.")
	return pEdict;
}

inline edict_t* EdictFromBaseHandle( CBaseHandle basehandle )
{
	if (!basehandle.IsValid())
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to retrieve edict from base handle.")

	return EdictFromIndex(IndexFromBaseHandle(basehandle));
}

inline edict_t* EdictFromIntHandle( int inthandle )
{
	return EdictFromIndex(IndexFromIntHandle(inthandle));
}

inline edict_t* EdictFromPointer( CPointer* pPtr )
{
	if (!pPtr || !pPtr->IsValid())
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to retrieve edict from pointer.")

	return EdictFromIndex(IndexFromPointer(pPtr));
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
	
	BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to retrieve edict from userid.")
	return NULL; // TO fix a warning
}

inline edict_t* EdictFromPlayerInfo( IPlayerInfo* playerinfo )
{
	if (!playerinfo)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to retrieve edict from playerinfo.")

	return EdictFromUserid(UseridFromPlayerInfo(playerinfo));
}

//-----------------------------------------------------------------------------
// Functions that return an entity's CBaseHandle instance.
//-----------------------------------------------------------------------------
inline CBaseHandle BaseHandleFromIndex( unsigned int index )
{
	return BaseHandleFromEdict(EdictFromIndex(index));
}

inline CBaseHandle BaseHandleFromEdict( edict_t* edict )
{
	if (!edict || edict->IsFree())
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to retrieve base handle from edict.")

	IServerNetworkable* pNetworkable = edict->GetNetworkable();
	if (!pNetworkable)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to retrieve base handle from edict.")

	IHandleEntity* pHandleEntity = pNetworkable->GetEntityHandle();
	if (!pHandleEntity)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to retrieve base handle from edict.")

	const CBaseHandle& handle = pHandleEntity->GetRefEHandle();
	if (!handle.IsValid())
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to retrieve base handle from edict.")

	return handle;
}

inline CBaseHandle BaseHandleFromIntHandle( int inthandle )
{
	return CBaseHandle(inthandle);
}

inline CBaseHandle BaseHandleFromPointer( CPointer* pPtr )
{
	if (!pPtr || !pPtr->IsValid())
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to retrieve base handle from pointer.")

	return BaseHandleFromEdict(EdictFromPointer(pPtr));
}

inline CBaseHandle BaseHandleFromUserid( unsigned int userid )
{
	return BaseHandleFromEdict(EdictFromUserid(userid));
}

inline CBaseHandle BaseHandleFromPlayerInfo( IPlayerInfo* playerinfo )
{
	if (!playerinfo)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to retrieve base handle from playerinfo.")

	return BaseHandleFromEdict(EdictFromPlayerInfo(playerinfo));
}

//-----------------------------------------------------------------------------
// Functions that return an entity's integer handle.
//-----------------------------------------------------------------------------
inline int IntHandleFromIndex( unsigned int index )
{
	return IntHandleFromBaseHandle(BaseHandleFromIndex(index));
}

inline int IntHandleFromEdict( edict_t* edict )
{
	if (!edict || edict->IsFree())
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to retrieve int handle from edict.")

	return IntHandleFromBaseHandle(BaseHandleFromEdict(edict));
}

inline int IntHandleFromBaseHandle( CBaseHandle basehandle )
{
	if (!basehandle.IsValid())
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to retrieve int handle from base handle.")

	return basehandle.ToInt();
}

inline int IntHandleFromPointer( CPointer* pPtr )
{
	if (!pPtr || !pPtr->IsValid())
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to retrieve int handle from pointer.")

	return IntHandleFromEdict(EdictFromPointer(pPtr));
}

inline int IntHandleFromUserid( unsigned int userid )
{
	return IntHandleFromEdict(EdictFromUserid(userid));
}

inline int IntHandleFromPlayerInfo( IPlayerInfo* playerinfo )
{
	if (!playerinfo)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to retrieve int handle from playerinfo.")

	return IntHandleFromEdict(EdictFromPlayerInfo(playerinfo));
}

//-----------------------------------------------------------------------------
// Functions that return an entity's CPointer instance.
//-----------------------------------------------------------------------------
inline CPointer* PointerFromIndex( unsigned int index )
{
	return PointerFromEdict(EdictFromIndex(index));
}

inline CPointer* PointerFromEdict( edict_t* edict )
{
	if (!edict || edict->IsFree())
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to retrieve pointer from edict.")

	return new CPointer((unsigned long)(edict->GetNetworkable()->GetBaseEntity()));
}

inline CPointer* PointerFromBaseHandle( CBaseHandle basehandle )
{
	if (!basehandle.IsValid())
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to retrieve pointer from base handle.")

	return PointerFromEdict(EdictFromBaseHandle(basehandle));
}

inline CPointer* PointerFromIntHandle( int inthandle )
{
	return PointerFromEdict(EdictFromIntHandle(inthandle));
}

inline CPointer* PointerFromUserid( unsigned int userid )
{
	return PointerFromEdict(EdictFromUserid(userid));
}

inline CPointer* PointerFromPlayerInfo( IPlayerInfo* playerinfo )
{
	if (!playerinfo)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to retrieve pointer from playerinfo.")

	return PointerFromEdict(EdictFromPlayerInfo(playerinfo));
}

//-----------------------------------------------------------------------------
// Functions that return an player's userid.
//-----------------------------------------------------------------------------
inline unsigned int UseridFromIndex( unsigned int index )
{
	return UseridFromPlayerInfo(PlayerInfoFromIndex(index));
}

inline unsigned int UseridFromEdict( edict_t* edict )
{
	if (!edict || edict->IsFree())
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to retrieve userid from edict.")

	return UseridFromIndex(IndexFromEdict(edict));
}

inline unsigned int UseridFromBaseHandle( CBaseHandle basehandle )
{
	if (!basehandle.IsValid())
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to retrieve userid from base handle.")

	return UseridFromIndex(IndexFromBaseHandle(basehandle));
}

inline unsigned int UseridFromIntHandle( int inthandle )
{
	return UseridFromBaseHandle(BaseHandleFromIntHandle(inthandle));
}

inline unsigned int UseridFromPointer( CPointer* pPtr )
{
	if (!pPtr || !pPtr->IsValid())
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to retrieve userid from pointer.")

	return UseridFromPlayerInfo(PlayerInfoFromPointer(pPtr));
}

inline unsigned int UseridFromPlayerInfo( IPlayerInfo* playerinfo )
{
	if (!playerinfo)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to retrieve userid from playerinfo.")

	return playerinfo->GetUserID();
}

//-----------------------------------------------------------------------------
// Functions that return an player's IPlayerInfo instance.
//-----------------------------------------------------------------------------
inline IPlayerInfo* PlayerInfoFromIndex( unsigned int index )
{
	if (index < 1 || index > (unsigned int) gpGlobals->maxClients)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to retrieve playerinfo from index.")

	edict_t* pEdict = EdictFromIndex(index);
	if (pEdict && !pEdict->IsFree() &&
		strcmp(pEdict->GetClassName(), "player") == 0)
	{
		return playerinfomanager->GetPlayerInfo(pEdict);
	}
	
	BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to retrieve playerinfo from index.")
	return NULL; // To fix a warning
}

inline IPlayerInfo* PlayerInfoFromEdict( edict_t* pEdict )
{
	if (!pEdict || pEdict->IsFree())
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to retrieve playerinfo from edict.")

	return playerinfomanager->GetPlayerInfo(pEdict);
}

inline IPlayerInfo* PlayerInfoFromBaseHandle( CBaseHandle basehandle )
{
	if (!basehandle.IsValid())
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to retrieve playerinfo from base handle.")

	return PlayerInfoFromIndex(basehandle.GetEntryIndex());
}

inline IPlayerInfo* PlayerInfoFromIntHandle( int inthandle )
{
	return PlayerInfoFromIndex(IndexFromIntHandle(inthandle));
}

inline IPlayerInfo* PlayerInfoFromPointer( CPointer* pPtr )
{
	if (!pPtr || !pPtr->IsValid())
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to retrieve playerinfo from pointer.")

	return PlayerInfoFromIndex(IndexFromPointer(pPtr));
}

inline IPlayerInfo* PlayerInfoFromUserid( unsigned int userid )
{
	return PlayerInfoFromEdict(EdictFromUserid(userid));
}

#endif // _CONVERSIONS_WRAP_H
