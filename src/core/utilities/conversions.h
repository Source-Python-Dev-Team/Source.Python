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

#ifndef _CONVERSIONS_H
#define _CONVERSIONS_H

//-----------------------------------------------------------------------------
// Includes.
//-----------------------------------------------------------------------------
#include "basehandle.h"
#include "eiface.h"
#include "public/game/server/iplayerinfo.h"
#include "modules/memory/memory_tools.h"
#include "utilities/baseentity.h"
#include "utilities/wrap_macros.h"


//-----------------------------------------------------------------------------
// External variables.
//-----------------------------------------------------------------------------
extern IVEngineServer *engine;
extern CGlobalVars *gpGlobals;
extern IPlayerInfoManager *playerinfomanager;


//-----------------------------------------------------------------------------
// Invalid results.
//-----------------------------------------------------------------------------
#define INVALID_ENTITY_INDEX -1
#define INVALID_PLAYER_USERID -1


//-----------------------------------------------------------------------------
// Returns an Edict instance from the given index.
//-----------------------------------------------------------------------------
inline edict_t *EdictFromIndex( unsigned int uiEntityIndex, bool bRaiseException = false )
{
	edict_t *pEdict = NULL;

	if (uiEntityIndex < (unsigned int)gpGlobals->maxEntities)
	{
		edict_t *pTempEdict;
#if defined(ENGINE_ORANGEBOX) || defined(ENGINE_BMS)
		pTempEdict = engine->PEntityOfEntIndex(uiEntityIndex);
#else
		pTempEdict = (edict_t *)(gpGlobals->pEdicts + uiEntityIndex);
#endif
		if (pTempEdict && !pTempEdict->IsFree() && pTempEdict->GetUnknown())
			pEdict = pTempEdict;
	}

	if (!pEdict && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get an Edict instance from the given index (%i).", uiEntityIndex);

	return pEdict;
}


//-----------------------------------------------------------------------------
// Returns an Edict instance from the given UserID.
//-----------------------------------------------------------------------------
inline edict_t *EdictFromUserid( unsigned int uiUserID, bool bRaiseException = false )
{
	edict_t *pEdict = NULL;

	for (int iCurrentIndex = 1; iCurrentIndex <= gpGlobals->maxClients; iCurrentIndex++)
	{
		edict_t *pCurrentEdict = EdictFromIndex(iCurrentIndex);
		if (engine->GetPlayerUserId(pCurrentEdict) == uiUserID)
			pEdict = pCurrentEdict;
	}

	if (!pEdict && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get an Edict instance from the given UseriID (%i).", uiUserID);

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
	int iEntityHandle = INVALID_EHANDLE_INDEX;

	if (hBaseHandle.IsValid())
		iEntityHandle = hBaseHandle.ToInt();

	if (iEntityHandle == INVALID_EHANDLE_INDEX && bRaiseException)
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
	CPointer *pEntityPointer = new CPointer((unsigned long)BaseEntityFromEdict(pEdict));

	if (!(pEntityPointer && pEntityPointer->IsValid()) && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get a Pointer instance from the given Edict instance (%x).", pEdict);

	return pEntityPointer;
}


//-----------------------------------------------------------------------------
// Returns a UserID from the given PlayerInfo instance.
//-----------------------------------------------------------------------------
inline int UseridFromPlayerInfo( IPlayerInfo *pPlayerInfo, bool bRaiseException = false )
{
	unsigned int uiUserID = INVALID_PLAYER_USERID;

	if (pPlayerInfo)
		uiUserID = pPlayerInfo->GetUserID();

	if (uiUserID == INVALID_PLAYER_USERID && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get a UserID from the given PlayerInfo instance (%x).", pPlayerInfo);

	return uiUserID;
}


//-----------------------------------------------------------------------------
// Returns a PlayerInfo instance from the given index.
//-----------------------------------------------------------------------------
inline IPlayerInfo *PlayerInfoFromIndex( unsigned int uiEntityIndex, bool bRaiseException = false )
{
	IPlayerInfo *pPlayerInfo = NULL;

	if (uiEntityIndex > 0 && (int)uiEntityIndex < gpGlobals->maxClients)
	{
		edict_t *pEdict = EdictFromIndex(uiEntityIndex);
		if (pEdict)
			pPlayerInfo = playerinfomanager->GetPlayerInfo(pEdict);
	}

	if (!pPlayerInfo && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get a PlayerInfo instance from the given index (%i).", uiEntityIndex);

	return pPlayerInfo;
}


//-----------------------------------------------------------------------------
// Returns an index from the given Edict instance.
//-----------------------------------------------------------------------------
inline int IndexFromEdict( edict_t *pEdict, bool bRaiseException = false )
{
	unsigned int uiEntityIndex = INVALID_ENTITY_INDEX;

	if (pEdict && !pEdict->IsFree())
#if defined(ENGINE_ORANGEBOX) || defined(ENGINE_BMS)
		uiEntityIndex = (unsigned int)engine->IndexOfEdict(pEdict);
#else
		uiEntityIndex = (unsigned int)(pEdict - gpGlobals->pEdicts);
#endif

	if (uiEntityIndex == INVALID_ENTITY_INDEX && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get an index from the given Edict instance (%x).", pEdict);

	return uiEntityIndex;
}


//-----------------------------------------------------------------------------
// Returns an index from the given BaseEntity instance.
//-----------------------------------------------------------------------------
inline int IndexFromBaseEntity( CBaseEntity *pBaseEntity, bool bRaiseException = false )
{
	unsigned int uiEntityIndex = INVALID_ENTITY_INDEX;

	if (pBaseEntity)
	{
		IServerUnknown *pServerUnknown = (IServerUnknown *)pBaseEntity;
		if (pServerUnknown)
		{
			IServerNetworkable *pServerNetworkable = pServerUnknown->GetNetworkable();
			if (pServerNetworkable)
				uiEntityIndex = IndexFromEdict(pServerNetworkable->GetEdict());
		}
	}

	if (uiEntityIndex == INVALID_ENTITY_INDEX && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get an index from the given BaseEntity instance (%x).", pBaseEntity);

	return uiEntityIndex;
}


//-----------------------------------------------------------------------------
// Returns an index from the given Pointer instance.
//-----------------------------------------------------------------------------
inline int IndexFromPointer( CPointer *pEntityPointer, bool bRaiseException = false )
{
	unsigned int uiEntityIndex = INVALID_ENTITY_INDEX;

	if (pEntityPointer && pEntityPointer->IsValid())
		uiEntityIndex = IndexFromBaseEntity((CBaseEntity *)pEntityPointer->m_ulAddr);

	if (uiEntityIndex == INVALID_ENTITY_INDEX && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get an index from the given Pointer instance (%x).", pEntityPointer->m_ulAddr);

	return uiEntityIndex;
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
	unsigned int uiEntityIndex = INVALID_ENTITY_INDEX;

	if (hBaseHandle.IsValid())
		uiEntityIndex = hBaseHandle.GetEntryIndex();

	if (uiEntityIndex == INVALID_ENTITY_INDEX && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get an index from the given BaseHandle instance (%i).", hBaseHandle.ToInt());

	return uiEntityIndex;
}


//-----------------------------------------------------------------------------
// Returns an index from the given IntHandle.
//-----------------------------------------------------------------------------
inline int IndexFromIntHandle( int iEntityHandle, bool bRaiseException = false )
{
	unsigned int uiEntityIndex = INVALID_ENTITY_INDEX;

	if (iEntityHandle != INVALID_EHANDLE_INDEX)
	{
		CBaseHandle hBaseHandle(iEntityHandle);
		const CBaseHandle hTestHandle = BaseHandleFromEdict(EdictFromIndex(hBaseHandle.GetEntryIndex()));
		if (hTestHandle.IsValid() || (hBaseHandle.GetSerialNumber() == hTestHandle.GetSerialNumber()))
			uiEntityIndex = hBaseHandle.GetEntryIndex();
	}

	if (uiEntityIndex == INVALID_ENTITY_INDEX && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get an index from the given IntHandle (%i).", iEntityHandle);

	return uiEntityIndex;
}


//-----------------------------------------------------------------------------
// Returns an index from the given UserID.
//-----------------------------------------------------------------------------
inline int IndexFromUserid( unsigned int uiUserID, bool bRaiseException = false )
{
	unsigned int uiEntityIndex = IndexFromEdict(EdictFromUserid(uiUserID));

	if (uiEntityIndex == INVALID_ENTITY_INDEX && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get an index from the given UseriID (%i).", uiUserID);

	return uiEntityIndex;
}


//-----------------------------------------------------------------------------
// Returns an index instance from the given PlayerInfo instance.
//-----------------------------------------------------------------------------
inline int IndexFromPlayerInfo( IPlayerInfo *pPlayerInfo, bool bRaiseException = false )
{
	unsigned int uiEntityIndex = IndexFromEdict(EdictFromPlayerInfo(pPlayerInfo));

	if (uiEntityIndex == INVALID_ENTITY_INDEX && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get an index from the given PlayerInfo instance (%x).", pPlayerInfo);

	return uiEntityIndex;
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
inline CBaseHandle BaseHandleFromIndex( unsigned int uiEntityIndex, bool bRaiseException = false )
{
	CBaseHandle hBaseHandle = BaseHandleFromEdict(EdictFromIndex(uiEntityIndex));

	if (!hBaseHandle.ToInt() && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get a BaseHandle instance from the given index (%i).", uiEntityIndex);

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
	CBaseHandle hBaseHandle = BaseHandleFromEdict(EdictFromBaseEntity(pBaseEntity));

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
inline CBaseHandle BaseHandleFromUserid( unsigned int uiUserID, bool bRaiseException = false )
{
	CBaseHandle hBaseHandle = BaseHandleFromEdict(EdictFromUserid(uiUserID));

	if (!hBaseHandle.ToInt() && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get a BaseHandle instance from the given UseriID (%i).", uiUserID);

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
inline int IntHandleFromIndex( unsigned int uiEntityIndex, bool bRaiseException = false )
{
	int iEntityHandle = IntHandleFromBaseHandle(BaseHandleFromIndex(uiEntityIndex));

	if (iEntityHandle == INVALID_EHANDLE_INDEX && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get an IntHandle from the given index (%i).", uiEntityIndex);

	return iEntityHandle;
}


//-----------------------------------------------------------------------------
// Returns an IntHandle from the given Edict instance.
//-----------------------------------------------------------------------------
inline int IntHandleFromEdict( edict_t *pEdict, bool bRaiseException = false )
{
	int iEntityHandle = IntHandleFromBaseHandle(BaseHandleFromEdict(pEdict));

	if (iEntityHandle == INVALID_EHANDLE_INDEX && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get an IntHandle from the given Edict instance (%x).", pEdict);

	return iEntityHandle;
}


//-----------------------------------------------------------------------------
// Returns an IntHandle from the given BaseEntity instance.
//-----------------------------------------------------------------------------
inline int IntHandleFromBaseEntity( CBaseEntity *pBaseEntity, bool bRaiseException = false )
{
	int iEntityHandle = IntHandleFromEdict(EdictFromBaseEntity(pBaseEntity));

	if (iEntityHandle == INVALID_EHANDLE_INDEX && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get an IntHandle from the given BaseEntity instance (%x).", pBaseEntity);

	return iEntityHandle;
}


//-----------------------------------------------------------------------------
// Returns an IntHandle from the given Pointer instance.
//-----------------------------------------------------------------------------
inline int IntHandleFromPointer( CPointer *pEntityPointer, bool bRaiseException = false )
{
	int iEntityHandle = IntHandleFromBaseEntity(BaseEntityFromPointer(pEntityPointer));

	if (iEntityHandle == INVALID_EHANDLE_INDEX && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get an IntHandle from the given Pointer instance (%x).", pEntityPointer->m_ulAddr);

	return iEntityHandle;
}


//-----------------------------------------------------------------------------
// Returns an IntHandle from the given UserID.
//-----------------------------------------------------------------------------
inline int IntHandleFromUserid( unsigned int uiUserID, bool bRaiseException = false )
{
	int iEntityHandle = IntHandleFromEdict(EdictFromUserid(uiUserID));

	if (iEntityHandle == INVALID_EHANDLE_INDEX && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get an IntHandle from the given UserID (%i).", uiUserID);

	return iEntityHandle;
}


//-----------------------------------------------------------------------------
// Returns an IntHandle from the given PlayerInfo instance.
//-----------------------------------------------------------------------------
inline int IntHandleFromPlayerInfo( IPlayerInfo *pPlayerInfo, bool bRaiseException = false )
{
	int iEntityHandle = IntHandleFromEdict(EdictFromPlayerInfo(pPlayerInfo));

	if (iEntityHandle == INVALID_EHANDLE_INDEX && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get an IntHandle from the given PlayerInfo instance (%x).", pPlayerInfo);

	return iEntityHandle;
}


//-----------------------------------------------------------------------------
// Returns a BaseEntity instance from the given index.
//-----------------------------------------------------------------------------
inline CBaseEntity *BaseEntityFromIndex( unsigned int uiEntityIndex, bool bRaiseException = false )
{
	CBaseEntity *pBaseEntity = BaseEntityFromEdict(EdictFromIndex(uiEntityIndex));

	if (!pBaseEntity && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get a BaseEntity instance from the given index (%i).", uiEntityIndex);

	return pBaseEntity;
}


//-----------------------------------------------------------------------------
// Returns a Pointer instance from the given index.
//-----------------------------------------------------------------------------
inline CPointer *PointerFromIndex( unsigned int uiEntityIndex, bool bRaiseException = false )
{
	CPointer *pEntityPointer = new CPointer((unsigned long)BaseEntityFromIndex(uiEntityIndex));

	if (!(pEntityPointer && pEntityPointer->IsValid()) && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get a Pointer instance from the given index (%i).", uiEntityIndex);

	return pEntityPointer;
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
	CPointer *pEntityPointer = NULL;

	if (pBaseEntity)
		pEntityPointer = new CPointer((unsigned long)pBaseEntity);

	if (!(pEntityPointer && pEntityPointer->IsValid()) && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get a Pointer instance from the given BaseEntity instance (%x).", pBaseEntity);

	return pEntityPointer;
}


//-----------------------------------------------------------------------------
// Returns a Pointer instance from the given BaseHandle instance.
//-----------------------------------------------------------------------------
inline CPointer *PointerFromBaseHandle( CBaseHandle hBaseHandle, bool bRaiseException = false )
{
	CPointer *pEntityPointer = PointerFromBaseEntity(BaseEntityFromBaseHandle(hBaseHandle));

	if (!(pEntityPointer && pEntityPointer->IsValid()) && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get a Pointer instance from the given BaseHandle instance (%i).", hBaseHandle.ToInt());

	return pEntityPointer;
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
	CPointer *pEntityPointer = PointerFromBaseEntity(BaseEntityFromIntHandle(iEntityHandle));

	if (!(pEntityPointer && pEntityPointer->IsValid()) && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get a Pointer instance from the given IntHandle (%i).", iEntityHandle);

	return pEntityPointer;
}


//-----------------------------------------------------------------------------
// Returns a BaseEntity instance from the given UserID.
//-----------------------------------------------------------------------------
inline CBaseEntity *BaseEntityFromUserid( unsigned int uiUserID, bool bRaiseException = false )
{
	CBaseEntity *pBaseEntity = BaseEntityFromEdict(EdictFromUserid(uiUserID));

	if (!pBaseEntity && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get a BaseEntity instance from the given UserID (%i).", uiUserID);

	return pBaseEntity;
}


//-----------------------------------------------------------------------------
// Returns a Pointer instance from the given UserID.
//-----------------------------------------------------------------------------
inline CPointer *PointerFromUserid( unsigned int uiUserID, bool bRaiseException = false )
{
	CPointer *pEntityPointer = PointerFromBaseEntity(BaseEntityFromUserid(uiUserID));

	if (!(pEntityPointer && pEntityPointer->IsValid()) && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get a Pointer instance from the given UserID (%i).", uiUserID);

	return pEntityPointer;
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
	CPointer *pEntityPointer = PointerFromBaseEntity(BaseEntityFromPlayerInfo(pPlayerInfo));

	if (!(pEntityPointer && pEntityPointer->IsValid()) && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get a Pointer instance from the given PlayerInfo instance (%x).", pPlayerInfo);

	return pEntityPointer;
}


//-----------------------------------------------------------------------------
// Returns a UserID from the given index.
//-----------------------------------------------------------------------------
inline int UseridFromIndex( unsigned int uiEntityIndex, bool bRaiseException = false )
{
	unsigned int uiUserID = UseridFromPlayerInfo(PlayerInfoFromIndex(uiEntityIndex));

	if (uiUserID == INVALID_PLAYER_USERID && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get a UserID from the given index (%i).", uiEntityIndex);

	return uiUserID;
}


//-----------------------------------------------------------------------------
// Returns a UserID from the given Edict instance.
//-----------------------------------------------------------------------------
inline int UseridFromEdict( edict_t *pEdict, bool bRaiseException = false )
{
	unsigned int uiUserID = UseridFromIndex(IndexFromEdict(pEdict));

	if (uiUserID == INVALID_PLAYER_USERID && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get a UserID from the given Edict instance (%x).", pEdict);

	return uiUserID;
}


//-----------------------------------------------------------------------------
// Returns a UserID from the given BaseHandle instance.
//-----------------------------------------------------------------------------
inline int UseridFromBaseHandle( CBaseHandle hBaseHandle, bool bRaiseException = false )
{
	unsigned int uiUserID = UseridFromIndex(IndexFromBaseHandle(hBaseHandle));

	if (uiUserID == INVALID_PLAYER_USERID && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get a UserID from the given BaseHandle instance (%i).", hBaseHandle.ToInt());

	return uiUserID;
}


//-----------------------------------------------------------------------------
// Returns a UserID from the given IntHandle.
//-----------------------------------------------------------------------------
inline int UseridFromIntHandle( int iEntityHandle, bool bRaiseException = false )
{
	unsigned int uiUserID = INVALID_PLAYER_USERID;

	if (iEntityHandle != INVALID_EHANDLE_INDEX)
		uiUserID = UseridFromBaseHandle(BaseHandleFromIntHandle(iEntityHandle));

	if (uiUserID == INVALID_PLAYER_USERID && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get a UserID from the given IntHandle (%i).", iEntityHandle);

	return uiUserID;
}


//-----------------------------------------------------------------------------
// Returns a UserID from the given BaseEntity instance.
//-----------------------------------------------------------------------------
inline int UseridFromBaseEntity( CBaseEntity *pBaseEntity, bool bRaiseException = false )
{
	unsigned int uiUserID = UseridFromPlayerInfo(PlayerInfoFromBaseEntity(pBaseEntity));

	if (uiUserID == INVALID_PLAYER_USERID && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get a UserID from the given BaseEntity instance (%x).", pBaseEntity);

	return uiUserID;
}


//-----------------------------------------------------------------------------
// Returns a UserID from the given Pointer instance.
//-----------------------------------------------------------------------------
inline int UseridFromPointer( CPointer *pEntityPointer, bool bRaiseException = false )
{
	unsigned int uiUserID = UseridFromBaseEntity(BaseEntityFromPointer(pEntityPointer));

	if (uiUserID == INVALID_PLAYER_USERID && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get a UserID from the given Pointer instance (%x).", pEntityPointer->m_ulAddr);

	return uiUserID;
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
inline IPlayerInfo *PlayerInfoFromUserid( unsigned int uiUserID, bool bRaiseException = false )
{
	IPlayerInfo *pPlayerInfo = PlayerInfoFromEdict(EdictFromUserid(uiUserID));

	if (!pPlayerInfo && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get a PlayerInfo instance from the given UserID (%i).", uiUserID);

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
