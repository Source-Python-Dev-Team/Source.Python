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

#ifndef _CONVERSIONS_H
#define _CONVERSIONS_H

//-----------------------------------------------------------------------------
// Includes.
//-----------------------------------------------------------------------------
#include "utilities/wrap_macros.h"
#include "modules/memory/memory_tools.h"
#include "basehandle.h"
#include "eiface.h"
#include "public/game/server/iplayerinfo.h"
#include "utilities/baseentity.h"


//-----------------------------------------------------------------------------
// External variables.
//-----------------------------------------------------------------------------
extern IVEngineServer *engine;
extern CGlobalVars *gpGlobals;
extern IPlayerInfoManager *playerinfomanager;


//-----------------------------------------------------------------------------
// Constants.
//-----------------------------------------------------------------------------
#define WORLD_ENTITY_INDEX 0


//-----------------------------------------------------------------------------
// Invalid results.
//-----------------------------------------------------------------------------
#define INVALID_ENTITY_INDEX -1
#define INVALID_PLAYER_USERID -1


//-----------------------------------------------------------------------------
// Returns an Edict instance from the given index.
//-----------------------------------------------------------------------------
inline edict_t *EdictFromIndex( int iEntityIndex, bool bRaiseException = false )
{
	edict_t *pEdict = NULL;

	if (iEntityIndex > INVALID_ENTITY_INDEX && iEntityIndex < gpGlobals->maxEntities)
	{
		edict_t *pTempEdict;
#if defined(ENGINE_ORANGEBOX) || defined(ENGINE_BMS)
		pTempEdict = engine->PEntityOfEntIndex(iEntityIndex);
#else
		pTempEdict = (edict_t *)(gpGlobals->pEdicts + iEntityIndex);
#endif
		if (pTempEdict && !pTempEdict->IsFree() && pTempEdict->GetUnknown())
			pEdict = pTempEdict;
	}

	if (!pEdict && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get an Edict instance from the given index (%i).", iEntityIndex);

	return pEdict;
}


//-----------------------------------------------------------------------------
// Returns an Edict instance from the given UserID.
//-----------------------------------------------------------------------------
inline edict_t *EdictFromUserid( int iUserID, bool bRaiseException = false )
{
	edict_t *pEdict = NULL;

	if (iUserID > INVALID_PLAYER_USERID)
	{
		for (int iCurrentIndex = 1; iCurrentIndex <= gpGlobals->maxClients; iCurrentIndex++)
		{
			edict_t *pCurrentEdict = EdictFromIndex(iCurrentIndex);
			if (engine->GetPlayerUserId(pCurrentEdict) == iUserID)
				pEdict = pCurrentEdict;
		}
	}

	if (!pEdict && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get an Edict instance from the given UserID (%i).", iUserID);

	return pEdict;
}


//-----------------------------------------------------------------------------
// Returns an Edict instance from the given PlayerInfo instance.
//-----------------------------------------------------------------------------
inline edict_t *EdictFromPlayerInfo( IPlayerInfo *pPlayerInfo, bool bRaiseException = false )
{
	edict_t *pEdict = NULL;

	if (pPlayerInfo)
		pEdict = EdictFromUserid(pPlayerInfo->GetUserID());

	if (!pEdict && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get an Edict instance from the given PlayerInfo instance (%x).", pPlayerInfo);

	return pEdict;
}


//-----------------------------------------------------------------------------
// Returns an Edict instance from the given BaseEntity instance.
//-----------------------------------------------------------------------------
inline edict_t *EdictFromBaseEntity( CBaseEntity *pBaseEntity, bool bRaiseException = false )
{
	edict_t *pEdict = NULL;

	if (pBaseEntity)
	{
		IServerUnknown *pServerUnknown = (IServerUnknown *)pBaseEntity;
		if (pServerUnknown)
		{
			IServerNetworkable *pServerNetworkable = pServerUnknown->GetNetworkable();
			if (pServerNetworkable)
				pEdict = pServerNetworkable->GetEdict();
		}
	}

	if (!pEdict && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get an Edict instance from the given BaseEntity instance (%x).", pBaseEntity);

	return pEdict;
}


//-----------------------------------------------------------------------------
// Returns a BaseHandle instance from the given Edict instance.
//-----------------------------------------------------------------------------
inline CBaseHandle BaseHandleFromEdict( edict_t *pEdict, bool bRaiseException = false )
{
	CBaseHandle hBaseHandle = NULL;

	if (pEdict && !pEdict->IsFree())
	{
		IServerNetworkable *pServerNetworkable = pEdict->GetNetworkable();
		if (pServerNetworkable)
			hBaseHandle = pServerNetworkable->GetEntityHandle();
	}

	if (!hBaseHandle.ToInt() && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get a BaseHandle instance from the given Edict instance (%x).", pEdict);

	return hBaseHandle;
}


//-----------------------------------------------------------------------------
// Returns an IntHandle from the given BaseHandle instance.
//-----------------------------------------------------------------------------
inline int IntHandleFromBaseHandle( CBaseHandle hBaseHandle, bool bRaiseException = false )
{
	int iEntityHandle = (int) INVALID_EHANDLE_INDEX;

	if (hBaseHandle.IsValid())
		iEntityHandle = hBaseHandle.ToInt();

	if (iEntityHandle == (int) INVALID_EHANDLE_INDEX && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get an IntHandle from the given BaseHandle instance (%i).", hBaseHandle.ToInt());

	return iEntityHandle;
}


//-----------------------------------------------------------------------------
// Returns a BaseEntity instance from the given Edict instance.
//-----------------------------------------------------------------------------
inline CBaseEntity *BaseEntityFromEdict( edict_t *pEdict, bool bRaiseException = false )
{
	CBaseEntity *pBaseEntity = NULL;

	if (pEdict && !pEdict->IsFree())
	{
		IServerUnknown *pServerUnknown = pEdict->GetUnknown();
		if (pServerUnknown)
			pBaseEntity = pServerUnknown->GetBaseEntity();
	}

	if (!pBaseEntity && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get a BaseEntity instance from the given Edict instance (%x).", pEdict);

	return pBaseEntity;
}


//-----------------------------------------------------------------------------
// Returns a Pointer instance from the given Edict instance.
//-----------------------------------------------------------------------------
inline CPointer *PointerFromEdict( edict_t *pEdict, bool bRaiseException = false )
{
	CBaseEntity *pBaseEntity = BaseEntityFromEdict(pEdict);

	if (!pBaseEntity && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get a Pointer instance from the given Edict instance (%x).", pEdict);

	return new CPointer((unsigned long)pBaseEntity);
}


//-----------------------------------------------------------------------------
// Returns a UserID from the given PlayerInfo instance.
//-----------------------------------------------------------------------------
inline int UseridFromPlayerInfo( IPlayerInfo *pPlayerInfo, bool bRaiseException = false )
{
	int iUserID = INVALID_PLAYER_USERID;

	if (pPlayerInfo)
		iUserID = pPlayerInfo->GetUserID();

	if (iUserID == INVALID_PLAYER_USERID && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get a UserID from the given PlayerInfo instance (%x).", pPlayerInfo);

	return iUserID;
}


//-----------------------------------------------------------------------------
// Returns a PlayerInfo instance from the given index.
//-----------------------------------------------------------------------------
inline IPlayerInfo *PlayerInfoFromIndex( int iEntityIndex, bool bRaiseException = false )
{
	IPlayerInfo *pPlayerInfo = NULL;

	if (iEntityIndex > WORLD_ENTITY_INDEX && iEntityIndex < gpGlobals->maxClients)
	{
		edict_t *pEdict = EdictFromIndex(iEntityIndex);
		if (pEdict)
			pPlayerInfo = playerinfomanager->GetPlayerInfo(pEdict);
	}

	if (!pPlayerInfo && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get a PlayerInfo instance from the given index (%i).", iEntityIndex);

	return pPlayerInfo;
}


//-----------------------------------------------------------------------------
// Returns an index from the given Edict instance.
//-----------------------------------------------------------------------------
inline int IndexFromEdict( edict_t *pEdict, bool bRaiseException = false )
{
	int iEntityIndex = INVALID_ENTITY_INDEX;

	if (pEdict && !pEdict->IsFree())
#if defined(ENGINE_ORANGEBOX) || defined(ENGINE_BMS)
		iEntityIndex = engine->IndexOfEdict(pEdict);
#else
		iEntityIndex = pEdict - gpGlobals->pEdicts;
#endif

	if (iEntityIndex == INVALID_ENTITY_INDEX && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get an index from the given Edict instance (%x).", pEdict);

	return iEntityIndex;
}


//-----------------------------------------------------------------------------
// Returns an index from the given BaseEntity instance.
//-----------------------------------------------------------------------------
inline int IndexFromBaseEntity( CBaseEntity *pBaseEntity, bool bRaiseException = false )
{
	int iEntityIndex = INVALID_ENTITY_INDEX;

	if (pBaseEntity)
	{
		IServerUnknown *pServerUnknown = (IServerUnknown *)pBaseEntity;
		if (pServerUnknown)
		{
			IServerNetworkable *pServerNetworkable = pServerUnknown->GetNetworkable();
			if (pServerNetworkable)
				iEntityIndex = IndexFromEdict(pServerNetworkable->GetEdict());
		}
	}

	if (iEntityIndex == INVALID_ENTITY_INDEX && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get an index from the given BaseEntity instance (%x).", pBaseEntity);

	return iEntityIndex;
}


//-----------------------------------------------------------------------------
// Returns an index from the given Pointer instance.
//-----------------------------------------------------------------------------
inline int IndexFromPointer( CPointer *pEntityPointer, bool bRaiseException = false )
{
	int iEntityIndex = INVALID_ENTITY_INDEX;

	if (pEntityPointer && pEntityPointer->IsValid())
		iEntityIndex = IndexFromBaseEntity((CBaseEntity *)pEntityPointer->m_ulAddr);

	if (iEntityIndex == INVALID_ENTITY_INDEX && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get an index from the given Pointer instance (%x).", pEntityPointer->m_ulAddr);

	return iEntityIndex;
}


//-----------------------------------------------------------------------------
// Returns a PlayerInfo instance from the given BaseEntity instance.
//-----------------------------------------------------------------------------
inline IPlayerInfo *PlayerInfoFromBaseEntity( CBaseEntity *pBaseEntity, bool bRaiseException = false )
{
	IPlayerInfo *pPlayerInfo = PlayerInfoFromIndex(IndexFromBaseEntity(pBaseEntity));

	if (!pPlayerInfo && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get a PlayerInfo instance from the given BaseEntity instance (%x).", pBaseEntity);

	return pPlayerInfo;
}


//-----------------------------------------------------------------------------
// Returns a BaseEntity instance from the given Pointer instance.
//-----------------------------------------------------------------------------
inline CBaseEntity *BaseEntityFromPointer( CPointer *pEntityPointer, bool bRaiseException = false )
{
	CBaseEntity *pBaseEntity = NULL;

	if (IndexFromPointer(pEntityPointer) != INVALID_ENTITY_INDEX)
		pBaseEntity = (CBaseEntity *)pEntityPointer->m_ulAddr;

	if (!pBaseEntity && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get a BaseEntity instance from the given Pointer instance (%x).", pEntityPointer->m_ulAddr);

	return pBaseEntity;
}


//-----------------------------------------------------------------------------
// Returns a PlayerInfo instance from the given Pointer instance.
//-----------------------------------------------------------------------------
inline IPlayerInfo *PlayerInfoFromPointer( CPointer *pEntityPointer, bool bRaiseException = false )
{
	IPlayerInfo *pPlayerInfo = PlayerInfoFromBaseEntity(BaseEntityFromPointer(pEntityPointer));

	if (!pPlayerInfo && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get a PlayerInfo instance from the given Pointer instance (%x).", pEntityPointer->m_ulAddr);

	return pPlayerInfo;
}


//-----------------------------------------------------------------------------
// Returns an index from the given BaseHandle instance.
//-----------------------------------------------------------------------------
inline int IndexFromBaseHandle( CBaseHandle hBaseHandle, bool bRaiseException = false )
{
	int iEntityIndex = INVALID_ENTITY_INDEX;

	if (hBaseHandle.IsValid())
		iEntityIndex = hBaseHandle.GetEntryIndex();

	if (iEntityIndex == INVALID_ENTITY_INDEX && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get an index from the given BaseHandle instance (%i).", hBaseHandle.ToInt());

	return iEntityIndex;
}


//-----------------------------------------------------------------------------
// Returns an index from the given IntHandle.
//-----------------------------------------------------------------------------
inline int IndexFromIntHandle( int iEntityHandle, bool bRaiseException = false )
{
	int iEntityIndex = (int) INVALID_ENTITY_INDEX;

	if (iEntityHandle != (int) INVALID_EHANDLE_INDEX)
	{
		CBaseHandle hBaseHandle(iEntityHandle);
		const CBaseHandle hTestHandle = BaseHandleFromEdict(EdictFromIndex(hBaseHandle.GetEntryIndex()));
		if (hTestHandle.IsValid() || (hBaseHandle.GetSerialNumber() == hTestHandle.GetSerialNumber()))
			iEntityIndex = hBaseHandle.GetEntryIndex();
	}

	if (iEntityIndex == INVALID_ENTITY_INDEX && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get an index from the given IntHandle (%i).", iEntityHandle);

	return iEntityIndex;
}


//-----------------------------------------------------------------------------
// Returns an index from the given UserID.
//-----------------------------------------------------------------------------
inline int IndexFromUserid( int iUserID, bool bRaiseException = false )
{
	int iEntityIndex = IndexFromEdict(EdictFromUserid(iUserID));

	if (iEntityIndex == INVALID_ENTITY_INDEX && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get an index from the given UserID (%i).", iUserID);

	return iEntityIndex;
}


//-----------------------------------------------------------------------------
// Returns an index instance from the given PlayerInfo instance.
//-----------------------------------------------------------------------------
inline int IndexFromPlayerInfo( IPlayerInfo *pPlayerInfo, bool bRaiseException = false )
{
	int iEntityIndex = IndexFromEdict(EdictFromPlayerInfo(pPlayerInfo));

	if (iEntityIndex == INVALID_ENTITY_INDEX && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get an index from the given PlayerInfo instance (%x).", pPlayerInfo);

	return iEntityIndex;
}


//-----------------------------------------------------------------------------
// Returns an Edict instance from the given BaseHandle instance.
//-----------------------------------------------------------------------------
inline edict_t *EdictFromBaseHandle( CBaseHandle hBaseHandle, bool bRaiseException = false )
{
	edict_t *pEdict = EdictFromIndex(IndexFromBaseHandle(hBaseHandle));

	if (!pEdict && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get an Edict instance from the given BaseHandle instance (%i).", hBaseHandle.ToInt());

	return pEdict;
}


//-----------------------------------------------------------------------------
// Returns an Edict instance from the given IntHandle.
//-----------------------------------------------------------------------------
inline edict_t *EdictFromIntHandle( int iEntityHandle, bool bRaiseException = false )
{
	edict_t *pEdict = EdictFromIndex(IndexFromIntHandle(iEntityHandle));

	if (!pEdict && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get an Edict instance from the given IntHandle (%i).", iEntityHandle);

	return pEdict;
}


//-----------------------------------------------------------------------------
// Returns an Edict instance from the given Pointer instance.
//-----------------------------------------------------------------------------
inline edict_t *EdictFromPointer( CPointer *pEntityPointer, bool bRaiseException = false )
{
	edict_t *pEdict = NULL;

	if (pEntityPointer && pEntityPointer->IsValid())
		pEdict = EdictFromBaseEntity((CBaseEntity *)pEntityPointer->m_ulAddr);

	if (!pEdict && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get an Edict instance from the given Pointer instance (%x).", pEntityPointer->m_ulAddr);

	return pEdict;
}


//-----------------------------------------------------------------------------
// Returns a BaseHandle instance from the given index.
//-----------------------------------------------------------------------------
inline CBaseHandle BaseHandleFromIndex( int iEntityIndex, bool bRaiseException = false )
{
	CBaseHandle hBaseHandle = BaseHandleFromEdict(EdictFromIndex(iEntityIndex));

	if (!hBaseHandle.ToInt() && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get a BaseHandle instance from the given index (%i).", iEntityIndex);

	return hBaseHandle;
}


//-----------------------------------------------------------------------------
// Returns a BaseHandle instance from the given IntHandle.
//-----------------------------------------------------------------------------
inline CBaseHandle BaseHandleFromIntHandle( int iEntityHandle, bool bRaiseException = false )
{
	CBaseHandle hBaseHandle = CBaseHandle(iEntityHandle);

	if (IndexFromBaseHandle(hBaseHandle) == INVALID_ENTITY_INDEX && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get a BaseHandle instance from the given IntHandle (%i).", iEntityHandle);

	return hBaseHandle;
}


//-----------------------------------------------------------------------------
// Returns a BaseHandle instance from the given BaseEntity instance.
//-----------------------------------------------------------------------------
inline CBaseHandle BaseHandleFromBaseEntity( CBaseEntity *pBaseEntity, bool bRaiseException = false)
{
	CBaseHandle hBaseHandle = pBaseEntity->GetRefEHandle();
	if (!hBaseHandle.ToInt() && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get a BaseHandle instance from the given BaseEntity instance (%x).", pBaseEntity);

	return hBaseHandle;
}


//-----------------------------------------------------------------------------
// Returns a BaseHandle instance from the given Pointer instance.
//-----------------------------------------------------------------------------
inline CBaseHandle BaseHandleFromPointer( CPointer *pEntityPointer, bool bRaiseException = false )
{
	CBaseHandle hBaseHandle = BaseHandleFromBaseEntity(BaseEntityFromPointer(pEntityPointer));

	if (!hBaseHandle.ToInt() && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get a BaseHandle instance from the given Pointer instance (%x).", pEntityPointer->m_ulAddr);

	return hBaseHandle;
}


//-----------------------------------------------------------------------------
// Returns a BaseHandle instance from the given UserID.
//-----------------------------------------------------------------------------
inline CBaseHandle BaseHandleFromUserid( int iUserID, bool bRaiseException = false )
{
	CBaseHandle hBaseHandle = BaseHandleFromEdict(EdictFromUserid(iUserID));

	if (!hBaseHandle.ToInt() && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get a BaseHandle instance from the given UserID (%i).", iUserID);

	return hBaseHandle;
}


//-----------------------------------------------------------------------------
// Returns a BaseHandle instance from the given PlayerInfo instance.
//-----------------------------------------------------------------------------
inline CBaseHandle BaseHandleFromPlayerInfo( IPlayerInfo *pPlayerInfo, bool bRaiseException = false )
{
	CBaseHandle hBaseHandle = BaseHandleFromEdict(EdictFromPlayerInfo(pPlayerInfo));

	if (!hBaseHandle.ToInt() && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get a BaseHandle instance from the given PlayerInfo instance (%x).", pPlayerInfo);

	return hBaseHandle;
}


//-----------------------------------------------------------------------------
// Returns an IntHandle from the given index.
//-----------------------------------------------------------------------------
inline int IntHandleFromIndex( int iEntityIndex, bool bRaiseException = false )
{
	int iEntityHandle = IntHandleFromBaseHandle(BaseHandleFromIndex(iEntityIndex));

	if (iEntityHandle == (int) INVALID_EHANDLE_INDEX && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get an IntHandle from the given index (%i).", iEntityIndex);

	return iEntityHandle;
}


//-----------------------------------------------------------------------------
// Returns an IntHandle from the given Edict instance.
//-----------------------------------------------------------------------------
inline int IntHandleFromEdict( edict_t *pEdict, bool bRaiseException = false )
{
	int iEntityHandle = IntHandleFromBaseHandle(BaseHandleFromEdict(pEdict));

	if (iEntityHandle == (int) INVALID_EHANDLE_INDEX && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get an IntHandle from the given Edict instance (%x).", pEdict);

	return iEntityHandle;
}


//-----------------------------------------------------------------------------
// Returns an IntHandle from the given BaseEntity instance.
//-----------------------------------------------------------------------------
inline int IntHandleFromBaseEntity( CBaseEntity *pBaseEntity, bool bRaiseException = false )
{
	int iEntityHandle = IntHandleFromBaseHandle(BaseHandleFromBaseEntity(pBaseEntity));

	if (iEntityHandle == (int) INVALID_EHANDLE_INDEX && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get an IntHandle from the given BaseEntity instance (%x).", pBaseEntity);

	return iEntityHandle;
}


//-----------------------------------------------------------------------------
// Returns an IntHandle from the given Pointer instance.
//-----------------------------------------------------------------------------
inline int IntHandleFromPointer( CPointer *pEntityPointer, bool bRaiseException = false )
{
	int iEntityHandle = IntHandleFromBaseEntity(BaseEntityFromPointer(pEntityPointer));

	if (iEntityHandle == (int) INVALID_EHANDLE_INDEX && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get an IntHandle from the given Pointer instance (%x).", pEntityPointer->m_ulAddr);

	return iEntityHandle;
}


//-----------------------------------------------------------------------------
// Returns an IntHandle from the given UserID.
//-----------------------------------------------------------------------------
inline int IntHandleFromUserid( int iUserID, bool bRaiseException = false )
{
	int iEntityHandle = IntHandleFromEdict(EdictFromUserid(iUserID));

	if (iEntityHandle == (int) INVALID_EHANDLE_INDEX && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get an IntHandle from the given UserID (%i).", iUserID);

	return iEntityHandle;
}


//-----------------------------------------------------------------------------
// Returns an IntHandle from the given PlayerInfo instance.
//-----------------------------------------------------------------------------
inline int IntHandleFromPlayerInfo( IPlayerInfo *pPlayerInfo, bool bRaiseException = false )
{
	int iEntityHandle = IntHandleFromEdict(EdictFromPlayerInfo(pPlayerInfo));

	if (iEntityHandle == (int) INVALID_EHANDLE_INDEX && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get an IntHandle from the given PlayerInfo instance (%x).", pPlayerInfo);

	return iEntityHandle;
}


//-----------------------------------------------------------------------------
// Returns a BaseEntity instance from the given index.
//-----------------------------------------------------------------------------
inline CBaseEntity *BaseEntityFromIndex( int iEntityIndex, bool bRaiseException = false )
{
	CBaseEntity *pBaseEntity = BaseEntityFromEdict(EdictFromIndex(iEntityIndex));

	if (!pBaseEntity && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get a BaseEntity instance from the given index (%i).", iEntityIndex);

	return pBaseEntity;
}


//-----------------------------------------------------------------------------
// Returns a Pointer instance from the given index.
//-----------------------------------------------------------------------------
inline CPointer *PointerFromIndex( int iEntityIndex, bool bRaiseException = false )
{
	CBaseEntity *pBaseEntity = BaseEntityFromIndex(iEntityIndex);

	if (!pBaseEntity && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get a Pointer instance from the given index (%i).", iEntityIndex);

	return new CPointer((unsigned long)pBaseEntity);
}


//-----------------------------------------------------------------------------
// Returns a BaseEntity instance from the given BaseHandle instance.
//-----------------------------------------------------------------------------
inline CBaseEntity *BaseEntityFromBaseHandle( CBaseHandle hBaseHandle, bool bRaiseException = false )
{
	CBaseEntity *pBaseEntity = BaseEntityFromEdict(EdictFromBaseHandle(hBaseHandle));

	if (!pBaseEntity && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get a BaseEntity instance from the given BaseHandle instance (%i).", hBaseHandle.ToInt());

	return pBaseEntity;
}


//-----------------------------------------------------------------------------
// Returns a Pointer instance from the given BaseEntity instance.
//-----------------------------------------------------------------------------
inline CPointer *PointerFromBaseEntity( CBaseEntity *pBaseEntity, bool bRaiseException = false )
{
	if (!pBaseEntity && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get a Pointer instance from the given BaseEntity instance (%x).", pBaseEntity);

	return new CPointer((unsigned long)pBaseEntity);
}


//-----------------------------------------------------------------------------
// Returns a Pointer instance from the given BaseHandle instance.
//-----------------------------------------------------------------------------
inline CPointer *PointerFromBaseHandle( CBaseHandle hBaseHandle, bool bRaiseException = false )
{
	CBaseEntity *pBaseEntity = BaseEntityFromBaseHandle(hBaseHandle);

	if (!pBaseEntity && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get a Pointer instance from the given BaseHandle instance (%i).", hBaseHandle.ToInt());

	return new CPointer((unsigned long)pBaseEntity);
}


//-----------------------------------------------------------------------------
// Returns a BaseEntity instance from the given IntHandle.
//-----------------------------------------------------------------------------
inline CBaseEntity *BaseEntityFromIntHandle( int iEntityHandle, bool bRaiseException = false )
{
	CBaseEntity *pBaseEntity = BaseEntityFromEdict(EdictFromIntHandle(iEntityHandle));

	if (!pBaseEntity  && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get a BaseEntity instance from the given IntHandle (%i).", iEntityHandle);

	return pBaseEntity;
}


//-----------------------------------------------------------------------------
// Returns a Pointer instance from the given IntHandle.
//-----------------------------------------------------------------------------
inline CPointer *PointerFromIntHandle( int iEntityHandle, bool bRaiseException = false )
{
	CBaseEntity *pBaseEntity = BaseEntityFromIntHandle(iEntityHandle);

	if (!pBaseEntity && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get a Pointer instance from the given IntHandle (%i).", iEntityHandle);

	return new CPointer((unsigned long)pBaseEntity);
}


//-----------------------------------------------------------------------------
// Returns a BaseEntity instance from the given UserID.
//-----------------------------------------------------------------------------
inline CBaseEntity *BaseEntityFromUserid( int iUserID, bool bRaiseException = false )
{
	CBaseEntity *pBaseEntity = BaseEntityFromEdict(EdictFromUserid(iUserID));

	if (!pBaseEntity && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get a BaseEntity instance from the given UserID (%i).", iUserID);

	return pBaseEntity;
}


//-----------------------------------------------------------------------------
// Returns a Pointer instance from the given UserID.
//-----------------------------------------------------------------------------
inline CPointer *PointerFromUserid( int iUserID, bool bRaiseException = false )
{
	CBaseEntity *pBaseEntity = BaseEntityFromUserid(iUserID);

	if (!pBaseEntity && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get a Pointer instance from the given UserID (%i).", iUserID);

	return new CPointer((unsigned long)pBaseEntity);
}


//-----------------------------------------------------------------------------
// Returns a BaseEntity instance from the given PlayerInfo instance.
//-----------------------------------------------------------------------------
inline CBaseEntity *BaseEntityFromPlayerInfo( IPlayerInfo *pPlayerInfo, bool bRaiseException = false )
{
	CBaseEntity *pBaseEntity = BaseEntityFromEdict(EdictFromPlayerInfo(pPlayerInfo));

	if (!pBaseEntity && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get a BaseEntity instance from the given PlayerInfo instance (%x).", pPlayerInfo);

	return pBaseEntity;
}


//-----------------------------------------------------------------------------
// Returns a Pointer instance from the given PlayerInfo instance.
//-----------------------------------------------------------------------------
inline CPointer *PointerFromPlayerInfo( IPlayerInfo *pPlayerInfo, bool bRaiseException = false )
{
	CBaseEntity *pBaseEntity = BaseEntityFromPlayerInfo(pPlayerInfo);

	if (!pBaseEntity && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get a Pointer instance from the given PlayerInfo instance (%x).", pPlayerInfo);

	return new CPointer((unsigned long)pBaseEntity);
}


//-----------------------------------------------------------------------------
// Returns a UserID from the given index.
//-----------------------------------------------------------------------------
inline int UseridFromIndex( int iEntityIndex, bool bRaiseException = false )
{
	int iUserID = UseridFromPlayerInfo(PlayerInfoFromIndex(iEntityIndex));

	if (iUserID == INVALID_PLAYER_USERID && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get a UserID from the given index (%i).", iEntityIndex);

	return iUserID;
}


//-----------------------------------------------------------------------------
// Returns a UserID from the given Edict instance.
//-----------------------------------------------------------------------------
inline int UseridFromEdict( edict_t *pEdict, bool bRaiseException = false )
{
	int iUserID = UseridFromIndex(IndexFromEdict(pEdict));

	if (iUserID == INVALID_PLAYER_USERID && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get a UserID from the given Edict instance (%x).", pEdict);

	return iUserID;
}


//-----------------------------------------------------------------------------
// Returns a UserID from the given BaseHandle instance.
//-----------------------------------------------------------------------------
inline int UseridFromBaseHandle( CBaseHandle hBaseHandle, bool bRaiseException = false )
{
	int iUserID = UseridFromIndex(IndexFromBaseHandle(hBaseHandle));

	if (iUserID == INVALID_PLAYER_USERID && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get a UserID from the given BaseHandle instance (%i).", hBaseHandle.ToInt());

	return iUserID;
}


//-----------------------------------------------------------------------------
// Returns a UserID from the given IntHandle.
//-----------------------------------------------------------------------------
inline int UseridFromIntHandle( int iEntityHandle, bool bRaiseException = false )
{
	int iUserID = INVALID_PLAYER_USERID;

	if (iEntityHandle != (int) INVALID_EHANDLE_INDEX)
		iUserID = UseridFromBaseHandle(BaseHandleFromIntHandle(iEntityHandle));

	if (iUserID == INVALID_PLAYER_USERID && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get a UserID from the given IntHandle (%i).", iEntityHandle);

	return iUserID;
}


//-----------------------------------------------------------------------------
// Returns a UserID from the given BaseEntity instance.
//-----------------------------------------------------------------------------
inline int UseridFromBaseEntity( CBaseEntity *pBaseEntity, bool bRaiseException = false )
{
	int iUserID = UseridFromPlayerInfo(PlayerInfoFromBaseEntity(pBaseEntity));

	if (iUserID == INVALID_PLAYER_USERID && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get a UserID from the given BaseEntity instance (%x).", pBaseEntity);

	return iUserID;
}


//-----------------------------------------------------------------------------
// Returns a UserID from the given Pointer instance.
//-----------------------------------------------------------------------------
inline int UseridFromPointer( CPointer *pEntityPointer, bool bRaiseException = false )
{
	int iUserID = UseridFromBaseEntity(BaseEntityFromPointer(pEntityPointer));

	if (iUserID == INVALID_PLAYER_USERID && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get a UserID from the given Pointer instance (%x).", pEntityPointer->m_ulAddr);

	return iUserID;
}


//-----------------------------------------------------------------------------
// Returns a PlayerInfo instance from the given Edict instance.
//-----------------------------------------------------------------------------
inline IPlayerInfo *PlayerInfoFromEdict( edict_t *pEdict, bool bRaiseException = false )
{
	IPlayerInfo *pPlayerInfo = PlayerInfoFromIndex(IndexFromEdict(pEdict));

	if (!pPlayerInfo && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get a PlayerInfo instance from the given Edict instance (%x).", pEdict);

	return pPlayerInfo;
}


//-----------------------------------------------------------------------------
// Returns a PlayerInfo instance from the given BaseHandle instance.
//-----------------------------------------------------------------------------
inline IPlayerInfo *PlayerInfoFromBaseHandle( CBaseHandle hBaseHandle, bool bRaiseException = false )
{
	IPlayerInfo *pPlayerInfo = PlayerInfoFromIndex(hBaseHandle.GetEntryIndex());

	if (!pPlayerInfo && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get a PlayerInfo instance from the given BaseHandle instance (%i).", hBaseHandle.ToInt());

	return pPlayerInfo;
}


//-----------------------------------------------------------------------------
// Returns a PlayerInfo instance from the given IntHandle.
//-----------------------------------------------------------------------------
inline IPlayerInfo *PlayerInfoFromIntHandle( int iEntityHandle, bool bRaiseException = false )
{
	IPlayerInfo *pPlayerInfo = PlayerInfoFromIndex(IndexFromIntHandle(iEntityHandle));

	if (!pPlayerInfo && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get a PlayerInfo instance from the given IntHandle (%i).", iEntityHandle);

	return pPlayerInfo;
}


//-----------------------------------------------------------------------------
// Returns a PlayerInfo instance from the given UserID.
//-----------------------------------------------------------------------------
inline IPlayerInfo *PlayerInfoFromUserid( int iUserID, bool bRaiseException = false )
{
	IPlayerInfo *pPlayerInfo = PlayerInfoFromEdict(EdictFromUserid(iUserID));

	if (!pPlayerInfo && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get a PlayerInfo instance from the given UserID (%i).", iUserID);

	return pPlayerInfo;
}


//-----------------------------------------------------------------------------
// Helper macro to avoid some redundant typing...
//-----------------------------------------------------------------------------
#define EXPORT_CONVERSION_FUNCTION(to_type, to_name, from_type, from_name, ...) \
	def(extract<const char *>(str(XSTRINGIFY(to_name##_from_##from_name)).lower().ptr()), \
		&to_name##From##from_name, \
		XSTRINGIFY(Returns the to_name (of type #to_type) from the given from_name (of type #from_type).), \
		(XSTRINGIFY(from_name), arg("raise_exception")=true), \
		##__VA_ARGS__ \
	)


#endif // _CONVERSIONS_H
